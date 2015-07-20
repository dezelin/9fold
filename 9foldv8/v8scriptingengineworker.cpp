//
// 9fold - 3D graphics and animation software
// Copyright (C) 2015  Aleksandar Dezelin <dezelin@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "v8scriptingengineworker.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

namespace _9fold
{
namespace scripting
{
namespace engine
{

using namespace v8;

//
// V8 allocator
//

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
public:
    virtual void* Allocate(size_t length)
    {
        void* data = AllocateUninitialized(length);
        return data == NULL ? data : memset(data, 0, length);
    }

    virtual void* AllocateUninitialized(size_t length)
    {
        return malloc(length);
    }

    virtual void Free(void* data, size_t)
    {
        free(data);
    }
};

//
// Debugger client class
//

class DebuggerClientData : public v8::Debug::ClientData
{
public:
    DebuggerClientData(V8ScriptingEngineWorkerPrivate *priv)
        : _priv(priv)
    {

    }

    V8ScriptingEngineWorkerPrivate* priv() const { return _priv; }

private:
    V8ScriptingEngineWorkerPrivate *_priv;
};

//
// Forward declarations of callbacks
//

static void EventCallback2(const Debug::EventDetails& event_details);
static void MessageCallback2(const Debug::Message& message);


//
// Private class
//

class V8ScriptingEngineWorkerPrivate
{
public:
    typedef V8ScriptingEngine::V8Error V8Error;
    typedef V8ScriptingEngine::ContinueType ContinueType;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    V8ScriptingEngineWorkerPrivate(const QString &scriptAsync,
        V8ScriptingEngineWorker *parent)
        : q_ptr(parent)
        , _seq(1)
        , _debug(false)
        , _scriptAsync(scriptAsync)
    {
        V8::InitializeICU();
        _platform = platform::CreateDefaultPlatform();
        V8::InitializePlatform(_platform);
        V8::Initialize();

        QString flags("--harmony --debugger --expose_debug_as=v8debug");
        V8::SetFlagsFromString(flags.toUtf8().data(), flags.length());

        Isolate::CreateParams createParams;
        createParams.array_buffer_allocator = &_allocator;
        _isolate = Isolate::New(createParams);
        _isolate->Enter();
    }

    ~V8ScriptingEngineWorkerPrivate()
    {
        _isolate->Exit();
        _isolate->Dispose();
        V8::Dispose();
        V8::ShutdownPlatform();
    }

    void EncapsulateGlobal(Local<ObjectTemplate>& /*global*/)
    {

    }

    int initializeDebugging()
    {
        _debug = true;
        return 0;
    }

    QString version() const
    {
        return QString(V8::GetVersion());
    }

    const V8Error& error() const
    {
        return _error;
    }

    const QString& scriptAsync() const
    {
        return _scriptAsync;
    }

    void eventCallback2(const Debug::EventDetails& event_details)
    {
        Q_Q(V8ScriptingEngineWorker);
        switch (event_details.GetEvent()) {
        case Break: {
            emit q->breakOccurred();
            break;
        }
        case Exception: {
            emit q->exceptionOccurred();
            break;
        }
        case NewFunction: {
            emit q->newFunctionOccurred();
            break;
        }
        case BeforeCompile: {
            emit q->beforeCompileOccurred();
            break;
        }
        case AfterCompile: {
            emit q->afterCompileOccurred();
            break;
        }
        case CompileError: {
            emit q->compileErrorOccurred();
            break;
        }
        case PromiseEvent: {
            emit q->promiseEventOccurred();
            break;
        }
        case AsyncTaskEvent: {
            emit q->asyncTaskEventOccurred();
            break;
        }
        default: {
            Q_ASSERT(!"Unknown event type.");
            break;
        }
        }
    }

    void messageCallback2(const Debug::Message& message)
    {
        Q_Q(V8ScriptingEngineWorker);
        if (message.IsEvent()) {
            switch (message.GetEvent()) {
            case Break: {
                emit q->breakOccurred();
                break;
            }
            case Exception: {
                emit q->exceptionOccurred();
                break;
            }
            case NewFunction: {
                emit q->newFunctionOccurred();
                break;
            }
            case BeforeCompile: {
                emit q->beforeCompileOccurred();
                break;
            }
            case AfterCompile: {
                emit q->afterCompileOccurred();
                break;
            }
            case CompileError: {
                emit q->compileErrorOccurred();
                break;
            }
            case PromiseEvent: {
                emit q->promiseEventOccurred();
                break;
            }
            case AsyncTaskEvent: {
                emit q->asyncTaskEventOccurred();
                break;
            }
            default: {
                Q_ASSERT(!"Unknown event type.");
                break;
            }
            }
        }
        else if (message.IsResponse()) {

        }
        else {

        }
    }

    QString run(const QString& script)
    {
        Q_Q(V8ScriptingEngineWorker);

        Isolate::Scope scope(_isolate);

        // Create a stack-allocated handle scope.
        HandleScope handleScope(_isolate);

        // Set debug event callback handler
        Debug::SetDebugEventListener(EventCallback2);

        // Set debug message callback handler
        Debug::SetMessageHandler(MessageCallback2);

        // Create global object
        Local<ObjectTemplate> global = ObjectTemplate::New();
        EncapsulateGlobal(global);

        // Create a new context.
        if (_context.IsEmpty()) {
            _context = Context::New(_isolate, 0, global);
            _context->SetAlignedPointerInEmbedderData(1, this);
        }

        // Enter the context for compiling and running the script.
        Context::Scope contextScope(_context);

        // Create a string containing the JavaScript source code.
        Local<String> source = String::NewFromUtf8(_isolate, script.toUtf8().data());

        // Catch compilation and evaluation errors
        TryCatch trycatch;
        _error = tryCatchToError(trycatch);

        // Compile the source code.
        QString result;
        Local<v8::Script> _script = v8::Script::Compile(source);
        if (_script.IsEmpty()) {
            _error = tryCatchToError(trycatch);
            String::Utf8Value exception(trycatch.Exception());
            result = QString(*exception);
            emit q->errorOccurred(_error);
            emit q->finished(result);
            return result;
        }

        if (_debug)
            Debug::DebugBreak(_isolate);

        // Run the script to get the result.
        Local<Value> _value = _script->Run();
        if (_value.IsEmpty()) {
            _error = tryCatchToError(trycatch);
            String::Utf8Value exception(trycatch.Exception());
            result = QString(*exception);
            emit q->errorOccurred(_error);
            emit q->finished(result);
            return result;
        }
        while(_debug) {
            Debug::ProcessDebugMessages();
            QCoreApplication::processEvents();
            q->thread()->usleep(100);
        }

        result = QString::fromLatin1(*String::Utf8Value(_value));
        emit q->finished(result);

        return result;
    }

    int breakZ()
    {
        return 0;
    }

    int continueZ(ContinueType type)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "continue";
        QJsonObject cmdArguments;
        cmdArguments["stepcount"] = 1;

        if (type == V8ScriptingEngine::ContinueType::In)
            cmdArguments["stepaction"] = "in";
        else if (type == V8ScriptingEngine::ContinueType::Next)
            cmdArguments["stepaction"] = "next";
        else if (type == V8ScriptingEngine::ContinueType::Out)
            cmdArguments["stepaction"] = "out";
        else {
            Q_ASSERT(!"Unknown continue type.");
            return -1;
        }


        cmdJson["arguments"] = cmdArguments;
        return sendDebuggerCommand(cmdJson);
    }

    int evaluate(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int lookup(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getBacktrace(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getFrame(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getScope(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getScopes(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getScripts(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getSource(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int setBreakpoint(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "setbreakpoint";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int changeBreakpoint(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int clearBreakpoint(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int setExceptionBreak(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getFlags(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getVersion(CommandResponse& /*response*/)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "version";
        return sendDebuggerCommand(cmdJson);
    }

    int gc(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int getListOfBreakpoints(const CommandRequest& /*request*/)
    {
        return 0;
    }

    int setVariableValue(const CommandRequest& /*request*/)
    {
        return 0;
    }

private:
    V8ScriptingEngine::V8Error tryCatchToError(const TryCatch& trycatch)
    {
        Local<Message> message = trycatch.Message();
        if (message.IsEmpty())
            return V8ScriptingEngine::V8Error();

        String::Utf8Value text(message->Get());
        V8ScriptingEngine::V8Error error(message->GetStartPosition(),
            message->GetEndPosition(), message->GetLineNumber(), QString(*text));
        return error;
    }

    int sendDebuggerCommand(QJsonObject& cmdJson)
    {
        cmdJson["seq"] = seq();
        cmdJson["type"] = "request";

        QJsonDocument json(cmdJson);
        QString cmd(QString::fromLatin1(json.toJson(QJsonDocument::Compact)));
        qDebug() << cmd;

        Locker locker(_isolate);
        Debug::SendCommand(_isolate, cmd.utf16(), cmd.length());
        return 0;
    }

    int seq()
    {
        return _seq++;
    }

private:
    V8ScriptingEngineWorker* const q_ptr;
    Q_DECLARE_PUBLIC(V8ScriptingEngineWorker)

private:
    int _seq;
    bool _debug;
    QString _scriptAsync;
    Isolate *_isolate;
    Platform *_platform;
    Local<Context> _context;
    V8ScriptingEngine::V8Error _error;
    ArrayBufferAllocator _allocator;
};

V8ScriptingEngineWorker::V8ScriptingEngineWorker(const QString &script)
    : QObject(), d_ptr(new V8ScriptingEngineWorkerPrivate(script, this))
{

}

V8ScriptingEngineWorker::V8ScriptingEngineWorker(QObject *parent)
    : QObject(parent), d_ptr(new V8ScriptingEngineWorkerPrivate(QString(), this))
{

}

V8ScriptingEngineWorker::~V8ScriptingEngineWorker()
{
    Q_D(V8ScriptingEngineWorker);
    delete d;
}

int V8ScriptingEngineWorker::breakZ()
{
    Q_D(V8ScriptingEngineWorker);
    return d->breakZ();
}

int V8ScriptingEngineWorker::continueZ(ContinueType type)
{
    Q_D(V8ScriptingEngineWorker);
    return d->continueZ(type);
}

int V8ScriptingEngineWorker::evaluate(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->evaluate(request);
}

int V8ScriptingEngineWorker::lookup(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->lookup(request);
}

int V8ScriptingEngineWorker::getBacktrace(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getBacktrace(request);
}

int V8ScriptingEngineWorker::getFrame(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getFrame(request);
}

int V8ScriptingEngineWorker::getScope(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getScope(request);
}

int V8ScriptingEngineWorker::getScopes(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getScopes(request);
}

int V8ScriptingEngineWorker::getScripts(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getScripts(request);
}

int V8ScriptingEngineWorker::getSource(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getSource(request);
}

int V8ScriptingEngineWorker::setBreakpoint(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->setBreakpoint(request);
}

int V8ScriptingEngineWorker::changeBreakpoint(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->changeBreakpoint(request);
}

int V8ScriptingEngineWorker::clearBreakpoint(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->clearBreakpoint(request);
}

int V8ScriptingEngineWorker::setExceptionBreak(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->setExceptionBreak(request);
}

int V8ScriptingEngineWorker::getFlags(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getFlags(request);
}

int V8ScriptingEngineWorker::getVersion(CommandResponse &response)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getVersion(response);
}

int V8ScriptingEngineWorker::gc(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->gc(request);
}

int V8ScriptingEngineWorker::getListOfBreakpoints(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getListOfBreakpoints(request);
}

int V8ScriptingEngineWorker::setVariableValue(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->setVariableValue(request);
}

int V8ScriptingEngineWorker::initializeDebugging()
{
    Q_D(V8ScriptingEngineWorker);
    return d->initializeDebugging();
}

const V8ScriptingEngineWorker::V8Error &V8ScriptingEngineWorker::error() const
{
    Q_D(const V8ScriptingEngineWorker);
    return d->error();
}

QString V8ScriptingEngineWorker::run(const QString &script)
{
    Q_D(V8ScriptingEngineWorker);
    return d->run(script);
}

QString V8ScriptingEngineWorker::version() const
{
    Q_D(const V8ScriptingEngineWorker);
    return d->version();
}

void V8ScriptingEngineWorker::execute()
{
    Q_D(V8ScriptingEngineWorker);
    d->run(d->scriptAsync());
}

void V8ScriptingEngineWorker::executeDebug()
{
    Q_D(V8ScriptingEngineWorker);
    d->run(d->scriptAsync());
}

//
// Static callback functions
//

static void EventCallback2(const Debug::EventDetails& event_details)
{
    Handle<Context> context = event_details.GetEventContext();
    Q_ASSERT(!context.IsEmpty());
    if (context.IsEmpty())
        return;

    V8ScriptingEngineWorkerPrivate *worker =
        static_cast<V8ScriptingEngineWorkerPrivate*>(context->GetAlignedPointerFromEmbedderData(1));
    Q_ASSERT(worker);
    if (!worker)
        return;

    worker->eventCallback2(event_details);
}

static void MessageCallback2(const Debug::Message& message)
{
    Handle<Context> context = message.GetEventContext();
    Q_ASSERT(!context.IsEmpty());
    if (context.IsEmpty())
        return;

    V8ScriptingEngineWorkerPrivate *worker =
        static_cast<V8ScriptingEngineWorkerPrivate*>(context->GetAlignedPointerFromEmbedderData(1));
    Q_ASSERT(worker);
    if (!worker)
        return;

    worker->messageCallback2(message);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold


