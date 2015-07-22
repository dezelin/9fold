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

#ifndef V8SCRIPTINGENGINE_H
#define V8SCRIPTINGENGINE_H

#include <scriptingengine.h>

#include <QObject>
#include <QScopedPointer>

namespace _9fold
{
namespace scripting
{
namespace engine
{

class V8ScriptingEnginePrivate;
class V8ScriptingEngine : public ScriptingEngine
{
    Q_OBJECT
public:
    V8ScriptingEngine(QObject *parent = 0);
    virtual ~V8ScriptingEngine();

    //
    // Scripting engine interface
    //

    class V8Error : public Error
    {
    public:
        V8Error();
        V8Error(int start, int end, int line, const QString& message);
        V8Error(const Error& error);
        virtual ~V8Error();

        int start() const;
        int end() const;
        int line() const;
        QString message() const;
    };

    virtual const Error& error() const;

    virtual QString version() const;

    virtual QString run(const QString& script);

    virtual void debugAsync(const QString &script);
    virtual void runAsync(const QString& script);

    //
    // Debugger interface
    //

    class V8Breakpoint : public Breakpoint
    {
    public:
        V8Breakpoint();
        V8Breakpoint(int line);
        V8Breakpoint(const QJsonObject& object);
        virtual ~V8Breakpoint();

        int line() const;
        QJsonObject toArguments();
    };

    class V8Frame : public Frame
    {
    public:
        V8Frame();
        virtual ~V8Frame();
    };

    class V8Scope : public Scope
    {
    public:
        V8Scope();
        virtual ~V8Scope();
    };

    class V8Exception : public Exception
    {
    public:
        V8Exception();
        virtual ~V8Exception();
    };

    class V8Script : public Script
    {
    public:
        V8Script();
        virtual ~V8Script();
    };

    class V8Source : public Source
    {
    public:
        V8Source();
        virtual ~V8Source();
    };

    virtual int breakZ();
    virtual int continueZ(const CommandRequest& request);
    virtual int evaluate(const CommandRequest& request);
    virtual int lookup(const CommandRequest& request);
    virtual int getBacktrace(const CommandRequest& request);
    virtual int getFrame(const CommandRequest& request);
    virtual int getScope(const CommandRequest& request);
    virtual int getScopes(const CommandRequest& request);
    virtual int getScripts(const CommandRequest& request);
    virtual int getSource(const CommandRequest& request);
    virtual int setBreakpoint(const CommandRequest& request);
    virtual int changeBreakpoint(const CommandRequest& request);
    virtual int clearBreakpoint(const CommandRequest& request);
    virtual int setExceptionBreak(const CommandRequest& request);
    virtual int getFlags(const CommandRequest& request);
    virtual int getVersion(const CommandRequest &request);
    virtual int disconnect(const CommandRequest& request);
    virtual int gc(const CommandRequest& request);
    virtual int getListOfBreakpoints(const CommandRequest& request);
    virtual int setVariableValue(const CommandRequest& request);

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

private slots:
    void onBreakOccurred();
    void onExceptionOccurred();
    void onNewFunctionOccurred();
    void onBeforeCompileOccurred();
    void onAfterCompileOccurred();
    void onCompileErrorOccurred();
    void onPromiseEventOccurred();
    void onAsyncTaskEventOccurred();

    void onContinueResponse(const V8ScriptingEngine::CommandResponse& response);
    void onEvaluateResponse(const V8ScriptingEngine::CommandResponse& response);
    void onLookupResponse(const V8ScriptingEngine::CommandResponse& response);
    void onBackTraceResponse(const V8ScriptingEngine::CommandResponse& response);
    void onFrameResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScopeResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScopesResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScriptsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSourceResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onChangeBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onClearBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetExceptionBreakResponse(const V8ScriptingEngine::CommandResponse& response);
    void onV8flagsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onVersionResponse(const V8ScriptingEngine::CommandResponse& response);
    void onGcResponse(const V8ScriptingEngine::CommandResponse& response);
    void onListBreakpointsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetVariableValueResponse(const V8ScriptingEngine::CommandResponse& response);

    void onError(const V8ScriptingEngine::V8Error &error);
    void onFinished(const QString& result);

private:
    V8ScriptingEnginePrivate* const d_ptr;
    Q_DECLARE_PRIVATE(V8ScriptingEngine)
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

Q_DECLARE_METATYPE(_9fold::scripting::engine::V8ScriptingEngine::V8Error)

#endif // V8SCRIPTINGENGINE_H
