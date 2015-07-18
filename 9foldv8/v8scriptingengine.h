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

class V8ScriptingEngine : public ScriptingEngine
{
    Q_OBJECT
public:
    V8ScriptingEngine(QObject *parent = 0);
    virtual ~V8ScriptingEngine();

    //
    // Scripting engine interface
    //

    virtual QString version() const;

    virtual QString run(const QString& script);

    //
    // Debugger interface
    //

    class V8Breakpoint : public Breakpoint
    {
    public:
        V8Breakpoint();
        virtual ~V8Breakpoint();
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
    virtual int continueZ(ContinueType type);
    virtual int evaluate(const CommandRequest& request, CommandResponse& response);
    virtual int lookup(const CommandRequest& request, CommandResponse& response);
    virtual int getBacktrace(const CommandRequest& request, CommandResponse& response);
    virtual int getFrame(const CommandRequest& request, CommandResponse& response);
    virtual int getScope(const CommandRequest& request, CommandResponse& response);
    virtual int getScopes(const CommandRequest& request, CommandResponse& response);
    virtual int getScripts(const CommandRequest& request, CommandResponse& response);
    virtual int getSource(const CommandRequest& request, CommandResponse& response);
    virtual int setBreakpoint(const CommandRequest& request, CommandResponse& response);
    virtual int changeBreakpoint(const CommandRequest& request, CommandResponse& response);
    virtual int clearBreakpoint(const CommandRequest& request, CommandResponse& response);
    virtual int setExceptionBreak(const CommandRequest& request, CommandResponse& response);
    virtual int getFlags(const CommandRequest& request, CommandResponse& response);
    virtual int getVersion(CommandResponse& response);
    virtual int gc(const CommandRequest& request, CommandResponse& response);
    virtual int getListOfBreakpoints(const CommandRequest& request, CommandResponse& response);
    virtual int setVariableValue(const CommandRequest& request, CommandResponse& response);

private:
    class V8ScriptingEnginePrivate;
    QScopedPointer<V8ScriptingEnginePrivate> _p;
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

#endif // V8SCRIPTINGENGINE_H
