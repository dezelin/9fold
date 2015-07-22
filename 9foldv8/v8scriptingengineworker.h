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

#ifndef V8SCRIPTINGENGINEWORKER_H
#define V8SCRIPTINGENGINEWORKER_H

#include "v8scriptingengine.h"

#include <QObject>

#include <include/v8.h>
#include <include/v8-debug.h>
#include <include/libplatform/libplatform.h>

namespace _9fold
{
namespace scripting
{
namespace engine
{

class V8ScriptingEngineWorkerPrivate;
class V8ScriptingEngineWorker : public QObject
{
    Q_OBJECT
public:
    explicit V8ScriptingEngineWorker(const QString& script);
    explicit V8ScriptingEngineWorker(QObject *parent = 0);
    virtual ~V8ScriptingEngineWorker();

    //
    // Debugging interface
    //

    typedef V8ScriptingEngine::V8Error V8Error;
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    int breakZ();
    int continueZ(const CommandRequest& request);
    int evaluate(const CommandRequest& request);
    int lookup(const CommandRequest& request);
    int getBacktrace(const CommandRequest& request);
    int getFrame(const CommandRequest& request);
    int getScope(const CommandRequest& request);
    int getScopes(const CommandRequest& request);
    int getScripts(const CommandRequest& request);
    int getSource(const CommandRequest& request);
    int setBreakpoint(const CommandRequest& request);
    int changeBreakpoint(const CommandRequest& request);
    int clearBreakpoint(const CommandRequest& request);
    int setExceptionBreak(const CommandRequest& request);
    int getFlags(const CommandRequest& request);
    int getVersion(const CommandRequest& request);
    int disconnect(const CommandRequest &request);
    int gc(const CommandRequest& request);
    int getListOfBreakpoints(const CommandRequest& request);
    int setVariableValue(const CommandRequest& request);

    int initializeDebugging();

    const V8Error& error() const;
    QString run(const QString& script);
    QString version() const;

signals:
    void breakOccurred();
    void exceptionOccurred();
    void newFunctionOccurred();
    void beforeCompileOccurred();
    void afterCompileOccurred();
    void compileErrorOccurred();
    void promiseEventOccurred();
    void asyncTaskEventOccurred();

    void continueResponse(const V8ScriptingEngine::CommandResponse& response);
    void evaluateResponse(const V8ScriptingEngine::CommandResponse& response);
    void lookupResponse(const V8ScriptingEngine::CommandResponse& response);
    void backTraceResponse(const V8ScriptingEngine::CommandResponse& response);
    void frameResponse(const V8ScriptingEngine::CommandResponse& response);
    void scopeResponse(const V8ScriptingEngine::CommandResponse& response);
    void scopesResponse(const V8ScriptingEngine::CommandResponse& response);
    void scriptsResponse(const V8ScriptingEngine::CommandResponse& response);
    void sourceResponse(const V8ScriptingEngine::CommandResponse& response);
    void setBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void changeBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void clearBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void setExceptionBreakResponse(const V8ScriptingEngine::CommandResponse& response);
    void v8flagsResponse(const V8ScriptingEngine::CommandResponse& response);
    void versionResponse(const V8ScriptingEngine::CommandResponse& response);
    void gcResponse(const V8ScriptingEngine::CommandResponse& response);
    void listBreakpointsResponse(const V8ScriptingEngine::CommandResponse& response);
    void setVariableValueResponse(const V8ScriptingEngine::CommandResponse& response);

    void errorOccurred(const V8ScriptingEngine::V8Error& error);
    void finished(const QString& result);

public slots:
    void execute();
    void executeDebug();

private:
    V8ScriptingEngineWorkerPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(V8ScriptingEngineWorker)
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

#endif // V8SCRIPTINGENGINEWORKER_H
