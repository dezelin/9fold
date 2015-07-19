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

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace _9fold
{
namespace scripting
{
namespace engine
{

using namespace v8;

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
        V8ScriptingEngineWorker *parent) : q_ptr(parent), _scriptAsync(scriptAsync)
    {
        V8::Initialize();
        _isolate = Isolate::New();
    }

    ~V8ScriptingEngineWorkerPrivate()
    {
        _isolate->Dispose();
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

    void eventCallback2(const Debug::EventDetails& /*event_details*/)
    {
    }

    void messageCallback2(const Debug::Message& /*message*/)
    {
    }

    QString run(const QString& script)
    {
        Q_Q(V8ScriptingEngineWorker);

        Isolate::Scope scope(_isolate);

        // Create a stack-allocated handle scope.
        HandleScope handleScope;

        // Set debug event callback handler
        Debug::SetDebugEventListener2(EventCallback2);

        // Set debug message callback handler
        Debug::SetMessageHandler2(MessageCallback2);

        // Create global object
        Local<ObjectTemplate> global = ObjectTemplate::New();
        EncapsulateGlobal(global);

        // Create a new context.
        if (_context.IsEmpty())
            _context = Context::New(0, global);

        // Enter the context for compiling and running the script.
        Context::Scope contextScope(_context);

        // Create a string containing the JavaScript source code.
        Local<String> source = String::New(script.toLatin1().data());

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

        result = QString::fromLatin1(*String::Utf8Value(_value));
        emit q->finished(result);
        return result;
    }

    int breakZ()
    {
        return 0;
    }

    int continueZ(ContinueType /*type*/)
    {
        return 0;
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
        return 0;
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

        QScopedPointer<DebuggerClientData> clientData(new DebuggerClientData(this));
        Debug::SendCommand(cmd.utf16(), cmd.length(), clientData.take(), _isolate);
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
    Persistent<Context> _context;
    V8ScriptingEngine::V8Error _error;
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

//
// Static callback functions
//

static void EventCallback2(const Debug::EventDetails& event_details)
{
    DebuggerClientData *clientData = static_cast<DebuggerClientData*>(event_details.GetClientData());
    if (!clientData)
        return;

    clientData->priv()->eventCallback2(event_details);
}

static void MessageCallback2(const Debug::Message& message)
{
    DebuggerClientData *clientData = static_cast<DebuggerClientData*>(message.GetClientData());
    if (!clientData)
        return;

    clientData->priv()->messageCallback2(message);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold


