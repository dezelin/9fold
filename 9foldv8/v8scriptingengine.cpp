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

#include "v8scriptingengine.h"

#include <v8.h>
#include <v8-debug.h>

#include <QScopedPointer>


namespace _9fold
{
namespace scripting
{
namespace engine
{

using namespace v8;

static void EventCallback2(const Debug::EventDetails& event_details);
static void MessageCallback2(const Debug::Message& message);


class V8ScriptingEngineImpl
{
public:
    class DebuggerClientData : public Debug::ClientData
    {
    public:
        DebuggerClientData(V8ScriptingEngineImpl *engineImpl)
            : Debug::ClientData(), _engineImpl(engineImpl)
        {

        }

        V8ScriptingEngineImpl* engine() const
        {
            return _engineImpl;
        }

    private:
        V8ScriptingEngineImpl *_engineImpl;
    };

    V8ScriptingEngineImpl()
    {
        V8::Initialize();
        _isolate = Isolate::New();
    }

    ~V8ScriptingEngineImpl()
    {
        _isolate->Dispose();
    }

    void EncapsulateGlobal(Local<ObjectTemplate>& global)
    {

    }

    QString run(const QString& script)
    {
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

        // Compile the source code.
        Local<v8::Script> _script = v8::Script::Compile(source);
        if (_script.IsEmpty()) {
            String::Utf8Value exception(trycatch.Exception());
            return QString(*exception);
        }

        // Run the script to get the result.
        Local<Value> _value = _script->Run();
        if (_value.IsEmpty()) {
            String::Utf8Value exception(trycatch.Exception());
            return QString(*exception);
        }

        return QString::fromLatin1(*String::Utf8Value(_value));
    }

    QString version() const
    {
        return QString(V8::GetVersion());
    }

    typedef V8ScriptingEngine::ContinueType ContinueType;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    int breakZ()
    {
        return 0;
    }

    int continueZ(ContinueType type)
    {
        return 0;
    }

    int evaluate(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int lookup(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getBacktrace(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getFrame(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getScope(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getScopes(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getScripts(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getSource(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int setBreakpoint(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int changeBreakpoint(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int clearBreakpoint(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int setExceptionBreak(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getFlags(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getVersion(CommandResponse& response)
    {
        return 0;
    }

    int gc(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int getListOfBreakpoints(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    int setVariableValue(const CommandRequest& request, CommandResponse& response)
    {
        return 0;
    }

    void eventCallback2(const Debug::EventDetails& event_details)
    {
    }

    void messageCallback2(const Debug::Message& message)
    {
    }

private:
    Isolate *_isolate;
    Persistent<Context> _context;
};

class V8ScriptingEngine::V8ScriptingEnginePrivate : public V8ScriptingEngineImpl
{
public:
    V8ScriptingEnginePrivate() : V8ScriptingEngineImpl()
    {

    }
};

V8ScriptingEngine::V8ScriptingEngine(QObject *parent)
    : ScriptingEngine(parent), _p(new V8ScriptingEnginePrivate())
{

}

V8ScriptingEngine::~V8ScriptingEngine()
{

}

//
// Scripting engine interface
//


QString V8ScriptingEngine::version() const
{
    return _p->version();
}

QString V8ScriptingEngine::run(const QString& script)
{
    return _p->run(script);
}

//
// Debugger interface
//

int V8ScriptingEngine::breakZ()
{
    return _p->breakZ();
}

int V8ScriptingEngine::continueZ(ContinueType type)
{
    return _p->continueZ(type);
}

int V8ScriptingEngine::evaluate(const CommandRequest& request, CommandResponse& response)
{
    return _p->evaluate(request, response);
}

int V8ScriptingEngine::lookup(const CommandRequest& request, CommandResponse& response)
{
    return _p->lookup(request, response);
}

int V8ScriptingEngine::getBacktrace(const CommandRequest& request, CommandResponse& response)
{
    return _p->getBacktrace(request, response);
}

int V8ScriptingEngine::getFrame(const CommandRequest& request, CommandResponse& response)
{
    return _p->getFrame(request, response);
}

int V8ScriptingEngine::getScope(const CommandRequest& request, CommandResponse& response)
{
    return _p->getScope(request, response);
}

int V8ScriptingEngine::getScopes(const CommandRequest& request, CommandResponse& response)
{
    return _p->getScopes(request, response);
}

int V8ScriptingEngine::getScripts(const CommandRequest& request, CommandResponse& response)
{
    return _p->getScripts(request, response);
}

int V8ScriptingEngine::getSource(const CommandRequest& request, CommandResponse& response)
{
    return _p->getSource(request, response);
}

int V8ScriptingEngine::setBreakpoint(const CommandRequest& request, CommandResponse& response)
{
    return _p->setBreakpoint(request, response);
}

int V8ScriptingEngine::changeBreakpoint(const CommandRequest& request, CommandResponse& response)
{
    return _p->changeBreakpoint(request, response);
}

int V8ScriptingEngine::clearBreakpoint(const CommandRequest& request, CommandResponse& response)
{
    return _p->clearBreakpoint(request, response);
}

int V8ScriptingEngine::setExceptionBreak(const CommandRequest& request, CommandResponse& response)
{
    return _p->setExceptionBreak(request, response);
}

int V8ScriptingEngine::getFlags(const CommandRequest& request, CommandResponse& response)
{
    return _p->getFlags(request, response);
}

int V8ScriptingEngine::getVersion(CommandResponse& response)
{
    return _p->getVersion(response);
}

int V8ScriptingEngine::gc(const CommandRequest& request, CommandResponse& response)
{
    return _p->gc(request, response);
}

int V8ScriptingEngine::getListOfBreakpoints(const CommandRequest& request, CommandResponse& response)
{
    return _p->getListOfBreakpoints(request, response);
}

int V8ScriptingEngine::setVariableValue(const CommandRequest& request, CommandResponse& response)
{
    return _p->setVariableValue(request, response);
}

//
// Static callback functions
//

static void EventCallback2(const Debug::EventDetails& event_details)
{
    V8ScriptingEngineImpl::DebuggerClientData *clientData =
            static_cast<V8ScriptingEngineImpl::DebuggerClientData*>(event_details.GetClientData());
    if (!clientData)
        return;

    clientData->engine()->eventCallback2(event_details);
}

static void MessageCallback2(const Debug::Message& message)
{
    V8ScriptingEngineImpl::DebuggerClientData *clientData =
            static_cast<V8ScriptingEngineImpl::DebuggerClientData*>(message.GetClientData());
    if (!clientData)
        return;

    clientData->engine()->messageCallback2(message);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold
