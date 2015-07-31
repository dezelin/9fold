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
#include "v8scriptingengineworker.h"

#include <QDebug>
#include <QJsonDocument>
#include <QObject>
#include <QScopedPointer>
#include <QThread>
#include <QVector>


namespace _9fold
{
namespace scripting
{
namespace engine
{

using namespace v8;

class V8ScriptingEnginePrivate
{
public:
    typedef V8ScriptingEngine::V8Error V8Error;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    V8ScriptingEnginePrivate(V8ScriptingEngine *parent)
        : q_ptr(parent), _worker(new V8ScriptingEngineWorker(parent))
    {
        qRegisterMetaType<V8ScriptingEngine::V8Error>("V8ScriptingEngine::V8Error");
        qRegisterMetaType<V8ScriptingEngine::CommandRequest>("V8ScriptingEngine::CommandRequest");
        qRegisterMetaType<V8ScriptingEngine::CommandResponse>("V8ScriptingEngine::CommandResponse");
    }

    ~V8ScriptingEnginePrivate()
    {
    }

    V8ScriptingEngineWorker* worker() const
    {
        return _worker;
    }

    int breakZ()
    {
        Q_ASSERT(_worker);
        return _worker->breakZ();
    }

    int continueZ(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->continueZ(request);
    }

    int evaluate(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->evaluate(request);
    }

    int lookup(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->lookup(request);
    }

    int getBacktrace(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getBacktrace(request);
    }

    int getFrame(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getFrame(request);
    }

    int getScope(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getScope(request);
    }

    int getScopes(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getScopes(request);
    }

    int getScripts(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getScripts(request);
    }

    int getSource(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getSource(request);
    }

    int setBreakpoint(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->setBreakpoint(request);
    }

    int changeBreakpoint(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->changeBreakpoint(request);
    }

    int clearBreakpoint(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->clearBreakpoint(request);
    }

    int setExceptionBreak(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->setExceptionBreak(request);
    }

    int getFlags(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getFlags(request);
    }

    int getVersion(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getVersion(request);
    }

    int disconnect(const CommandRequest &request)
    {
        Q_ASSERT(_worker);
        return _worker->disconnect(request);
    }

    int gc(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->gc(request);
    }

    int getListOfBreakpoints(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->getListOfBreakpoints(request);
    }

    int setVariableValue(const CommandRequest& request)
    {
        Q_ASSERT(_worker);
        return _worker->setVariableValue(request);
    }

    int initializeDebugging()
    {
        Q_ASSERT(_worker);
        return _worker->initializeDebugging();
    }

    const V8Error& error() const
    {
        Q_ASSERT(_worker);
        return _worker->error();
    }

    QString run(const QString& scriptName, const QString& script)
    {
        Q_ASSERT(_worker);
        return _worker->run(scriptName, script);
    }

    void debugAsync(const QString& scriptName, const QString &script)
    {
        runAsync(scriptName, script, true);
    }

    void runAsync(const QString& scriptName, const QString &script, bool debug = false)
    {
        Q_Q(V8ScriptingEngine);
        Q_ASSERT(_worker);
        QScopedPointer<QThread> thread(new QThread());
        QScopedPointer<V8ScriptingEngineWorker> worker(new V8ScriptingEngineWorker(scriptName, script));
        if (debug) {
            worker->initializeDebugging();
        }

        worker->moveToThread(thread.data());

        // Connect to Debug events
        q->connect(worker.data(), SIGNAL(breakOccurred()), q, SLOT(onBreakOccurred()));
        q->connect(worker.data(), SIGNAL(exceptionOccurred()), q, SLOT(onExceptionOccurred()));
        q->connect(worker.data(), SIGNAL(newFunctionOccurred()), q, SLOT(onNewFunctionOccurred()));
        q->connect(worker.data(), SIGNAL(beforeCompileOccurred()), q, SLOT(onBeforeCompileOccurred()));
        q->connect(worker.data(), SIGNAL(afterCompileOccurred()), q, SLOT(onAfterCompileOccurred()));
        q->connect(worker.data(), SIGNAL(compileErrorOccurred()), q, SLOT(onCompileErrorOccurred()));
        q->connect(worker.data(), SIGNAL(promiseEventOccurred()), q, SLOT(onPromiseEventOccurred()));
        q->connect(worker.data(), SIGNAL(asyncTaskEventOccurred()), q, SLOT(onAsyncTaskEventOccurred()));

        q->connect(worker.data(), SIGNAL(continueResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onContinueResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(evaluateResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onEvaluateResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(lookupResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onLookupResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(backTraceResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onBackTraceResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(frameResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onFrameResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(scopeResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onScopeResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(scopesResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onScopesResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(scriptsResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onScriptsResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(sourceResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onSourceResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(setBreakpointResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onSetBreakpointResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(changeBreakpointResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onChangeBreakpointResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(clearBreakpointResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onClearBreakpointResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(setExceptionBreakResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onSetExceptionBreakResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(v8flagsResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onV8flagsResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(versionResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onVersionResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(gcResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onGcResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(listBreakpointsResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onListBreakpointsResponse(V8ScriptingEngine::CommandResponse)));
        q->connect(worker.data(), SIGNAL(setVariableValueResponse(V8ScriptingEngine::CommandResponse)),
            q, SLOT(onSetVariableValueResponse(V8ScriptingEngine::CommandResponse)));

        // Connect to Isolate and thread events
        q->connect(worker.data(), SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), q,
            SLOT(onError(V8ScriptingEngine::V8Error)));
        q->connect(worker.data(), SIGNAL(finished(QString)), q, SLOT(onFinished(QString)));

        q->connect(thread.data(), SIGNAL(started()), worker.data(), SLOT(execute()));
        q->connect(worker.data(), SIGNAL(finished(QString)), thread.data(), SLOT(quit()));
        q->connect(worker.data(), SIGNAL(finished(QString)), worker.data(), SLOT(deleteLater()));
        q->connect(thread.data(), SIGNAL(finished()), thread.data(), SLOT(deleteLater()));

        thread->start();

        thread.take();
        worker.take();
    }

    void exposeGlobalQObject(const QString& name, QObject* object)
    {
        Q_ASSERT(_worker);
        _worker->exposeGlobalQObject(name, object);
    }

    QString version() const
    {
        Q_ASSERT(_worker);
        return _worker->version();
    }

private:
    V8ScriptingEngine* const q_ptr;
    Q_DECLARE_PUBLIC(V8ScriptingEngine)

private:
    V8ScriptingEngineWorker *_worker;
};

V8ScriptingEngine::V8ScriptingEngine(QObject *parent)
    : ScriptingEngine(parent), d_ptr(new V8ScriptingEnginePrivate(this))
{
    Q_D(V8ScriptingEngine);
    connect(d->worker(), SIGNAL(breakOccurred()), this, SLOT(onBreakOccurred()));
    connect(d->worker(), SIGNAL(exceptionOccurred()), this, SLOT(onExceptionOccurred()));
    connect(d->worker(), SIGNAL(newFunctionOccurred()), this, SLOT(onNewFunctionOccurred()));
    connect(d->worker(), SIGNAL(beforeCompileOccurred()), this, SLOT(onBeforeCompileOccurred()));
    connect(d->worker(), SIGNAL(afterCompileOccurred()), this, SLOT(onAfterCompileOccurred()));
    connect(d->worker(), SIGNAL(compileErrorOccurred()), this, SLOT(onCompileErrorOccurred()));
    connect(d->worker(), SIGNAL(promiseEventOccurred()), this, SLOT(onPromiseEventOccurred()));
    connect(d->worker(), SIGNAL(asyncTaskEventOccurred()), this, SLOT(onAsyncTaskEventOccurred()));

    connect(d->worker(), SIGNAL(continueResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onContinueResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(evaluateResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onEvaluateResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(lookupResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onLookupResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(backTraceResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onBackTraceResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(frameResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onFrameResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(scopeResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScopeResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(scopesResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScopesResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(scriptsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScriptsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(sourceResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSourceResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(setBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(changeBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onChangeBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(clearBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onClearBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(setExceptionBreakResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetExceptionBreakResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(v8flagsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onV8flagsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(versionResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onVersionResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(gcResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onGcResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(listBreakpointsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onListBreakpointsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->worker(), SIGNAL(setVariableValueResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetVariableValueResponse(V8ScriptingEngine::CommandResponse)));

    connect(d->worker(), SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), this,
        SLOT(onError(V8ScriptingEngine::V8Error)));
    connect(d->worker(), SIGNAL(finished(QString)), this, SLOT(onFinished(QString)));
}

V8ScriptingEngine::~V8ScriptingEngine()
{
    Q_D(V8ScriptingEngine);
    delete d;
}

const ScriptingEngine::Error &V8ScriptingEngine::error() const
{
    Q_D(const V8ScriptingEngine);
    return d->error();
}

//
// Scripting engine interface
//


QString V8ScriptingEngine::version() const
{
    Q_D(const V8ScriptingEngine);
    return d->version();
}

QString V8ScriptingEngine::run(const QString &scriptName, const QString& script)
{
    Q_D(V8ScriptingEngine);
    return d->run(scriptName, script);
}

void V8ScriptingEngine::debugAsync(const QString &scriptName, const QString &script)
{
    Q_D(V8ScriptingEngine);
    d->debugAsync(scriptName, script);
}

void V8ScriptingEngine::runAsync(const QString &scriptName, const QString &script)
{
    Q_D(V8ScriptingEngine);
    d->runAsync(scriptName, script);
}

void V8ScriptingEngine::exposeGlobalQObject(const QString& name, QObject* object)
{
    Q_D(V8ScriptingEngine);
    d->exposeGlobalQObject(name, object);
}

//
// Debugger interface
//

int V8ScriptingEngine::breakZ()
{
    Q_D(V8ScriptingEngine);
    return d->breakZ();
}

int V8ScriptingEngine::continueZ(const CommandRequest &request)
{
    Q_D(V8ScriptingEngine);
    return d->continueZ(request);
}

int V8ScriptingEngine::evaluate(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->evaluate(request);
}

int V8ScriptingEngine::lookup(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->lookup(request);
}

int V8ScriptingEngine::getBacktrace(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getBacktrace(request);
}

int V8ScriptingEngine::getFrame(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getFrame(request);
}

int V8ScriptingEngine::getScope(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getScope(request);
}

int V8ScriptingEngine::getScopes(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getScopes(request);
}

int V8ScriptingEngine::getScripts(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getScripts(request);
}

int V8ScriptingEngine::getSource(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getSource(request);
}

int V8ScriptingEngine::setBreakpoint(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->setBreakpoint(request);
}

int V8ScriptingEngine::changeBreakpoint(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->changeBreakpoint(request);
}

int V8ScriptingEngine::clearBreakpoint(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->clearBreakpoint(request);
}

int V8ScriptingEngine::setExceptionBreak(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->setExceptionBreak(request);
}

int V8ScriptingEngine::getFlags(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getFlags(request);
}

int V8ScriptingEngine::getVersion(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getVersion(request);
}

int V8ScriptingEngine::disconnect(const CommandRequest &request)
{
    Q_D(V8ScriptingEngine);
    return d->disconnect(request);
}

int V8ScriptingEngine::gc(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->gc(request);
}

int V8ScriptingEngine::getListOfBreakpoints(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->getListOfBreakpoints(request);
}

int V8ScriptingEngine::setVariableValue(const CommandRequest& request)
{
    Q_D(V8ScriptingEngine);
    return d->setVariableValue(request);
}

//
// Private slots
//

void V8ScriptingEngine::onBreakOccurred()
{
    emit breakOccurred();
}

void V8ScriptingEngine::onExceptionOccurred()
{
    emit exceptionOccurred();
}

void V8ScriptingEngine::onNewFunctionOccurred()
{
    emit newFunctionOccurred();
}

void V8ScriptingEngine::onBeforeCompileOccurred()
{
    emit beforeCompileOccurred();
}

void V8ScriptingEngine::onAfterCompileOccurred()
{
    emit afterCompileOccurred();
}

void V8ScriptingEngine::onCompileErrorOccurred()
{
    emit compileErrorOccurred();
}

void V8ScriptingEngine::onPromiseEventOccurred()
{
    emit promiseEventOccurred();
}

void V8ScriptingEngine::onAsyncTaskEventOccurred()
{
    emit asyncTaskEventOccurred();
}

void V8ScriptingEngine::onContinueResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit continueResponse(response);
}

void V8ScriptingEngine::onEvaluateResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit evaluateResponse(response);
}

void V8ScriptingEngine::onLookupResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit lookupResponse(response);
}

void V8ScriptingEngine::onBackTraceResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit backTraceResponse(response);
}

void V8ScriptingEngine::onFrameResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit frameResponse(response);
}

void V8ScriptingEngine::onScopeResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit scopeResponse(response);
}

void V8ScriptingEngine::onScopesResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit scopesResponse(response);
}

void V8ScriptingEngine::onScriptsResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit scriptsResponse(response);
}

void V8ScriptingEngine::onSourceResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit sourceResponse(response);
}

void V8ScriptingEngine::onSetBreakpointResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit setBreakpointResponse(response);
}

void V8ScriptingEngine::onChangeBreakpointResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit changeBreakpointResponse(response);
}

void V8ScriptingEngine::onClearBreakpointResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit clearBreakpointResponse(response);
}

void V8ScriptingEngine::onSetExceptionBreakResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit setExceptionBreakResponse(response);
}

void V8ScriptingEngine::onV8flagsResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit v8flagsResponse(response);
}

void V8ScriptingEngine::onVersionResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit versionResponse(response);
}

void V8ScriptingEngine::onGcResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit gcResponse(response);
}

void V8ScriptingEngine::onListBreakpointsResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit listBreakpointsResponse(response);
}

void V8ScriptingEngine::onSetVariableValueResponse(const V8ScriptingEngine::CommandResponse &response)
{
    emit setVariableValueResponse(response);
}

void V8ScriptingEngine::onError(const V8ScriptingEngine::V8Error &error)
{
    emit errorOccurred(error);
}

void V8ScriptingEngine::onFinished(const QString &result)
{
    emit finished(result);
}

//
// Debugger classes
//

const QString kV8BreakpointLine = "line";

V8ScriptingEngine::V8Breakpoint::V8Breakpoint() : Breakpoint()
{

}

V8ScriptingEngine::V8Breakpoint::V8Breakpoint(int line)
    : Breakpoint()
{
    insert(kV8BreakpointLine, line);
}

V8ScriptingEngine::V8Breakpoint::V8Breakpoint(const QJsonObject &object)
    : Breakpoint(object)
{

}

V8ScriptingEngine::V8Breakpoint::~V8Breakpoint()
{

}

int V8ScriptingEngine::V8Breakpoint::line() const
{
    return value(kV8BreakpointLine).toInt();
}

QJsonObject V8ScriptingEngine::V8Breakpoint::toArguments()
{
    QJsonObject json;
    json["type"] = "script";
    json["line"] = value(kV8BreakpointLine);
    return json;
}


const QString kV8ErrorStart = "start";
const QString kV8ErrorEnd = "end";
const QString kV8ErrorLine = "line";
const QString kV8ErrorMessage = "message";

V8ScriptingEngine::V8Error::V8Error() : Error()
{

}

V8ScriptingEngine::V8Error::V8Error(int start, int end, int line, const QString &message)
    : Error()
{
    insert(kV8ErrorStart, start);
    insert(kV8ErrorEnd, end);
    insert(kV8ErrorLine, line);
    insert(kV8ErrorMessage, message);
}

V8ScriptingEngine::V8Error::V8Error(const ScriptingEngine::Error &error)
{
    insert(kV8ErrorStart, error.value(kV8ErrorStart));
    insert(kV8ErrorEnd, error.value(kV8ErrorEnd));
    insert(kV8ErrorLine, error.value(kV8ErrorLine));
    insert(kV8ErrorMessage, error.value(kV8ErrorMessage));
}

V8ScriptingEngine::V8Error::~V8Error()
{

}

int V8ScriptingEngine::V8Error::start() const
{
    return value(kV8ErrorStart).toInt();
}

int V8ScriptingEngine::V8Error::end() const
{
    return value(kV8ErrorEnd).toInt();
}

int V8ScriptingEngine::V8Error::line() const
{
    return value(kV8ErrorLine).toInt();
}

QString V8ScriptingEngine::V8Error::message() const
{
    return value(kV8ErrorMessage).toString();
}

} // namespace engine
} // namespace scripting
} // namespace _9fold
