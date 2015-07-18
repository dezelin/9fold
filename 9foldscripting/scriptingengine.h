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

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace _9fold
{
namespace scripting
{
namespace engine
{

class ScriptingEngineDebugger;

class ScriptingEngine : public QObject
{
    Q_OBJECT
public:

    explicit ScriptingEngine(QObject *parent = 0);
    virtual ~ScriptingEngine();

    //
    // Scripting engine interface
    //

    virtual QString version() const = 0;

    virtual QString run(const QString& script) = 0;

    //
    // Debugger interface
    //

    class Breakpoint : public QJsonObject
    {
    public:
        virtual ~Breakpoint() {}
    };

    class Frame : public QJsonObject
    {
    public:
        virtual ~Frame() {}
    };

    class Scope : public QJsonObject
    {
    public:
        virtual ~Scope() {}
    };

    class Exception : public QJsonObject
    {
    public:
        virtual ~Exception() {}
    };

    class Script : public QJsonObject
    {
    public:
        virtual ~Script() {}
    };

    class Source : public QJsonObject
    {
    public:
        virtual ~Source() {}
    };

    class CommandRequest : public QJsonObject
    {
    public:
        virtual ~CommandRequest() {}
    };

    class CommandResponse : public QJsonObject
    {
    public:
        virtual ~CommandResponse() {}
    };

    enum class ContinueType : int {
        In = 0,
        Next,
        Out
    };

    virtual int breakZ() = 0;
    virtual int continueZ(ContinueType type) = 0;
    virtual int evaluate(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int lookup(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getBacktrace(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getFrame(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getScope(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getScopes(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getScripts(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getSource(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int setBreakpoint(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int changeBreakpoint(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int clearBreakpoint(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int setExceptionBreak(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getFlags(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getVersion(CommandResponse& response) = 0;
    virtual int gc(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int getListOfBreakpoints(const CommandRequest& request, CommandResponse& response) = 0;
    virtual int setVariableValue(const CommandRequest& request, CommandResponse& response) = 0;

signals:

public slots:
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

#endif // SCRIPTINGENGINE_H
