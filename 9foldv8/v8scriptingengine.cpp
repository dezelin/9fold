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

#include <v8.h>
#include <v8-debug.h>

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
    typedef V8ScriptingEngine::ContinueType ContinueType;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    V8ScriptingEnginePrivate(V8ScriptingEngine *parent)
        : q_ptr(parent), _worker(new V8ScriptingEngineWorker(parent))
    {
        qRegisterMetaType<V8ScriptingEngine::V8Error>("V8ScriptingEngine::V8Error");
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

    int continueZ(ContinueType type)
    {
        Q_ASSERT(_worker);
        return _worker->continueZ(type);
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

    int getVersion(CommandResponse& response)
    {
        Q_ASSERT(_worker);
        return _worker->getVersion(response);
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

    QString run(const QString& script)
    {
        Q_ASSERT(_worker);
        return _worker->run(script);
    }

    void debugAsync()
    {
        Q_ASSERT(_worker);
    }

    void runAsync(const QString &script)
    {
        Q_Q(V8ScriptingEngine);
        Q_ASSERT(_worker);
        QScopedPointer<QThread> thread(new QThread());
        QScopedPointer<V8ScriptingEngineWorker> worker(new V8ScriptingEngineWorker(script));
        worker->moveToThread(thread.data());
        q->connect(worker.data(), SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), q,
            SLOT(onError(V8ScriptingEngine::V8Error)));
        q->connect(thread.data(), SIGNAL(started()), worker.data(), SLOT(execute()));
        q->connect(worker.data(), SIGNAL(finished(QString)), thread.data(), SLOT(quit()));
        q->connect(worker.data(), SIGNAL(finished(QString)), worker.data(), SLOT(deleteLater()));
        q->connect(thread.data(), SIGNAL(finished()), thread.data(), SLOT(deleteLater()));

        thread->start();

        thread.take();
        worker.take();
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
    connect(d->worker(), SIGNAL(finished(QString)), this, SLOT(onFinished(QString)));
    connect(d->worker(), SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), this,
        SLOT(onError(V8ScriptingEngine::V8Error)));
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

QString V8ScriptingEngine::run(const QString& script)
{
    Q_D(V8ScriptingEngine);
    return d->run(script);
}

void V8ScriptingEngine::debugAsync()
{
    Q_D(V8ScriptingEngine);
    d->debugAsync();
}

void V8ScriptingEngine::runAsync(const QString &script)
{
    Q_D(V8ScriptingEngine);
    d->runAsync(script);
}

//
// Debugger interface
//

int V8ScriptingEngine::breakZ()
{
    Q_D(V8ScriptingEngine);
    return d->breakZ();
}

int V8ScriptingEngine::continueZ(ContinueType type)
{
    Q_D(V8ScriptingEngine);
    return d->continueZ(type);
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

int V8ScriptingEngine::getVersion(CommandResponse& response)
{
    Q_D(V8ScriptingEngine);
    return d->getVersion(response);
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

int V8ScriptingEngine::initializeDebugging()
{
    Q_D(V8ScriptingEngine);
    return d->initializeDebugging();
}

//
// Private slots
//

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
