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

#include <singleton.h>

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
using namespace _9fold::patterns;

//
// Forward declarations of callbacks
//

static void EventCallback(const Debug::EventDetails& event_details);
static void MessageCallback(const Debug::Message& message);

//
// Platform singleton
//

class PlatformInitializer
{
public:
    PlatformInitializer()
    {
        V8::InitializeICU();
        _platform = platform::CreateDefaultPlatform();
        V8::InitializePlatform(_platform);
        V8::Initialize();

        QString flags("--harmony --debugger");
        V8::SetFlagsFromString(flags.toUtf8().data(), flags.length());
    }

    ~PlatformInitializer()
    {
        V8::Dispose();
        V8::ShutdownPlatform();
        delete _platform;
    }

private:
    Platform *_platform;
};

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
// Private class (V8 Isolate)
//

class V8ScriptingEngineWorkerPrivate
{
public:
    typedef V8ScriptingEngine::V8Error V8Error;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    V8ScriptingEngineWorkerPrivate(const QString &scriptNameAsync,
        const QString &scriptAsync, V8ScriptingEngineWorker *parent)
        : q_ptr(parent)
        , _seq(0)
        , _debug(false)
        , _scriptAsync(scriptAsync)
        , _scriptNameAsync(scriptNameAsync)
    {
        Singleton<PlatformInitializer>::instance();
    }

    ~V8ScriptingEngineWorkerPrivate()
    {
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

    const QString& scriptNameAsync() const
    {
        return _scriptNameAsync;
    }

    void exposeGlobalQObject(const QString &name, QObject *object)
    {

    }

    void eventCallback(const Debug::EventDetails& event_details)
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

    void messageCallback(const Debug::Message& message)
    {
        Q_Q(V8ScriptingEngineWorker);

        Local<String> jsonMessage = message.GetJSON();
        Q_ASSERT(!jsonMessage.IsEmpty());
        if (jsonMessage.IsEmpty()) {
            qWarning() << "Got empty JSON message from V8";
            return;
        }

        QString jsonString(*String::Utf8Value(jsonMessage));
        qDebug() << "V8 message: " << jsonString;

        if (message.IsEvent()) {
            switch (message.GetEvent()) {
            case Break: {
                //emit q->breakOccurred();
                V8ScriptingEngine::CommandRequest args;
                V8ScriptingEngine::CommandRequest req;
                req["arguments"] = args;
                continueZ(req);
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
            QJsonParseError error;
            QJsonDocument json = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
            if (error.error != QJsonParseError::NoError) {
                qCritical() << "Got invalid JSON response from V8: " << error.errorString();
                return;
            }

            CommandResponse jsonResponse(json.object());
            Q_ASSERT(jsonResponse["type"] == "response");

            if (jsonResponse["command"] == "continue")
                emit q->continueResponse(jsonResponse);
            else if (jsonResponse["command"] == "evaluate")
                emit q->evaluateResponse(jsonResponse);
            else if (jsonResponse["command"] == "lookup")
                emit q->lookupResponse(jsonResponse);
            else if (jsonResponse["command"] == "backtrace")
                emit q->backTraceResponse(jsonResponse);
            else if (jsonResponse["command"] == "frame")
                emit q->frameResponse(jsonResponse);
            else if (jsonResponse["command"] == "scope")
                emit q->scopeResponse(jsonResponse);
            else if (jsonResponse["command"] == "scopes")
                emit q->scopesResponse(jsonResponse);
            else if (jsonResponse["command"] == "scripts")
                emit q->scriptsResponse(jsonResponse);
            else if (jsonResponse["command"] == "source")
                emit q->sourceResponse(jsonResponse);
            else if (jsonResponse["command"] == "setbreakpoint")
                emit q->setBreakpointResponse(jsonResponse);
            else if (jsonResponse["command"] == "changebreakpoint")
                emit q->changeBreakpointResponse(jsonResponse);
            else if (jsonResponse["command"] == "clearbreakpoint")
                emit q->clearBreakpointResponse(jsonResponse);
            else if (jsonResponse["command"] == "setexceptionbreak")
                emit q->setExceptionBreakResponse(jsonResponse);
            else if (jsonResponse["command"] == "v8flags")
                emit q->v8flagsResponse(jsonResponse);
            else if (jsonResponse["command"] == "version")
                emit q->versionResponse(jsonResponse);
            else if (jsonResponse["command"] == "gc")
                emit q->gcResponse(jsonResponse);
            else if (jsonResponse["command"] == "disconnect")
                emit q->disconnectResponse(jsonResponse);
            else if (jsonResponse["command"] == "listbreakpoints")
                emit q->listBreakpointsResponse(jsonResponse);
            else if (jsonResponse["command"] == "setvariablevalue")
                emit q->setVariableValueResponse(jsonResponse);
            else
                Q_ASSERT(!"Unsupported command response.");
        }
        else
            Q_ASSERT(!"Unsupported message.");
    }

    QString run(const QString& scriptName, const QString& script)
    {
        Q_Q(V8ScriptingEngineWorker);

        QString result;

        Isolate::CreateParams createParams;
        createParams.array_buffer_allocator = &_allocator;
        _isolate = Isolate::New(createParams);

        {
            Locker lock(_isolate);
            Isolate::Scope scope(_isolate);

            // Create a stack-allocated handle scope.
            HandleScope handleScope(_isolate);

            // Set debug event callback handler
            Debug::SetDebugEventListener(EventCallback);

            // Set debug message callback handler
            Debug::SetMessageHandler(MessageCallback);

            // Create global object
            Local<ObjectTemplate> global = ObjectTemplate::New();
            EncapsulateGlobal(global);

            // Create a new context.
            if (_context.IsEmpty()) {
                Local<Context> localContext = Context::New(_isolate, 0, global);
                localContext->SetAlignedPointerInEmbedderData(1, this);
                _context.Reset(_isolate, localContext);
            }

            // Enter the context for compiling and running the script.
            Context::Scope contextScope(_context.Get(_isolate));

            // Create a string containing the JavaScript source code.
            Local<String> source = String::NewFromUtf8(_isolate, script.toUtf8().data());

            // Catch compilation and evaluation errors
            TryCatch trycatch;
            _error = tryCatchToError(trycatch);

            // Compile the source code.
            Local<String> sourceName = String::NewFromUtf8(_isolate, scriptName.toUtf8().data());
            Local<v8::Script> _script = v8::Script::Compile(source, sourceName);
            if (_script.IsEmpty()) {
                _error = tryCatchToError(trycatch);
                String::Utf8Value exception(trycatch.Exception());
                result = QString(*exception);
                emit q->errorOccurred(_error);
                emit q->finished(result);
                goto _quit;
            }

            if (_debug)
                Debug::DebugBreak(_isolate);

            // Run the script to get the result.
            MaybeLocal<Value> _value = _script->Run(Debug::GetDebugContext());
            if (_value.IsEmpty()) {
                _error = tryCatchToError(trycatch);
                String::Utf8Value exception(trycatch.Exception());
                result = QString(*exception);
                emit q->errorOccurred(_error);
                emit q->finished(result);
                goto _quit;
            }

            result = QString::fromLatin1(*String::Utf8Value(_value.FromMaybe(Local<Value>())));
            emit q->finished(result);
        }

    _quit:
        _isolate->Dispose();
        return result;
    }

    int breakZ()
    {
        Locker locker(_isolate);
        Isolate::Scope scope(_isolate);
        Debug::DebugBreak(_isolate);
        return 0;
    }

    int continueZ(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "continue";
        if (request["arguments"] != QJsonValue::Undefined)
            cmdJson["arguments"] = request["arguments"];

        return sendDebuggerCommand(cmdJson);
    }

    int evaluate(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "evaluate";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int lookup(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "lookup";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getBacktrace(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "backtrace";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getFrame(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "frame";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getScope(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "scope";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getScopes(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "scopes";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getScripts(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "scripts";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getSource(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "source";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int setBreakpoint(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "setbreakpoint";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int changeBreakpoint(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "changebreakpoint";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int clearBreakpoint(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "clearbreakpoint";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int setExceptionBreak(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "setexceptionbreak";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getFlags(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "v8flags";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getVersion(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "version";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int disconnect(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "disconnect";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int gc(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "gc";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int getListOfBreakpoints(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "listbreakpoints";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
    }

    int setVariableValue(const CommandRequest& request)
    {
        QJsonObject cmdJson;
        cmdJson["command"] = "setvariablevalue";
        cmdJson["arguments"] = request["arguments"];
        return sendDebuggerCommand(cmdJson);
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
        qDebug() << "V8 command: " << cmd;

        Locker locker(_isolate);
        Isolate::Scope scope(_isolate);

        QScopedPointer<DebuggerClientData> clientData(new DebuggerClientData(this));

        Debug::SendCommand(_isolate, cmd.utf16(), cmd.length(), clientData.take());
        Debug::ProcessDebugMessages();

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
    QString _scriptNameAsync;
    Isolate *_isolate;
    Persistent<Context> _context;
    V8ScriptingEngine::V8Error _error;
    ArrayBufferAllocator _allocator;
};

V8ScriptingEngineWorker::V8ScriptingEngineWorker(const QString &scriptName, const QString &script)
    : QObject(), d_ptr(new V8ScriptingEngineWorkerPrivate(scriptName, script, this))
{

}

V8ScriptingEngineWorker::V8ScriptingEngineWorker(QObject *parent)
    : QObject(parent), d_ptr(new V8ScriptingEngineWorkerPrivate(QString(), QString(), this))
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

int V8ScriptingEngineWorker::continueZ(const CommandRequest& request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->continueZ(request);
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

int V8ScriptingEngineWorker::getVersion(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->getVersion(request);
}

int V8ScriptingEngineWorker::disconnect(const CommandRequest &request)
{
    Q_D(V8ScriptingEngineWorker);
    return d->disconnect(request);
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

QString V8ScriptingEngineWorker::run(const QString& scriptName, const QString &script)
{
    Q_D(V8ScriptingEngineWorker);
    return d->run(scriptName, script);
}

QString V8ScriptingEngineWorker::version() const
{
    Q_D(const V8ScriptingEngineWorker);
    return d->version();
}

void V8ScriptingEngineWorker::exposeGlobalQObject(const QString &name, QObject *object)
{
    Q_D(V8ScriptingEngineWorker);
    d->exposeGlobalQObject(name, object);
}

void V8ScriptingEngineWorker::execute()
{
    Q_D(V8ScriptingEngineWorker);
    d->run(d->scriptNameAsync(), d->scriptAsync());
}

void V8ScriptingEngineWorker::executeDebug()
{
    Q_D(V8ScriptingEngineWorker);
    d->run(d->scriptNameAsync(), d->scriptAsync());
}

//
// Static callback functions
//

static void EventCallback(const Debug::EventDetails& event_details)
{
    DebuggerClientData *clientData = static_cast<DebuggerClientData*>(event_details.GetClientData());
    if (clientData) {
        clientData->priv()->eventCallback(event_details);
        return;
    }

    Handle<Context> context = event_details.GetEventContext();
    Q_ASSERT(!context.IsEmpty());
    if (context.IsEmpty())
        return;

    V8ScriptingEngineWorkerPrivate *worker =
        static_cast<V8ScriptingEngineWorkerPrivate*>(context->GetAlignedPointerFromEmbedderData(1));
    Q_ASSERT(worker);
    if (!worker)
        return;

    worker->eventCallback(event_details);
}

static void MessageCallback(const Debug::Message& message)
{
    DebuggerClientData *clientData = static_cast<DebuggerClientData*>(message.GetClientData());
    if (clientData) {
        clientData->priv()->messageCallback(message);
        return;
    }

    Locker locker(message.GetIsolate());
    Isolate::Scope scope(message.GetIsolate());

    Handle<Context> context = message.GetEventContext();
    Q_ASSERT(!context.IsEmpty());
    if (context.IsEmpty())
        return;

    V8ScriptingEngineWorkerPrivate *worker =
        static_cast<V8ScriptingEngineWorkerPrivate*>(context->GetAlignedPointerFromEmbedderData(1));
    Q_ASSERT(worker);
    if (!worker)
        return;

    worker->messageCallback(message);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold


