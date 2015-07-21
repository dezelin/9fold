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

    class Error : public QJsonObject
    {
    public:
        virtual ~Error() {}
    };

    virtual const Error& error() const = 0;

    virtual QString version() const = 0;

    virtual QString run(const QString& script) = 0;

    virtual void debugAsync(const QString &script) = 0;
    virtual void runAsync(const QString& script) = 0;

    //
    // Debugger interface
    //

    class Breakpoint : public QJsonObject
    {
    public:
        Breakpoint() : QJsonObject() {}
        Breakpoint(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Breakpoint() {}
    };

    class Frame : public QJsonObject
    {
    public:
        Frame() : QJsonObject() {}
        Frame(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Frame() {}
    };

    class Scope : public QJsonObject
    {
    public:
        Scope() : QJsonObject() {}
        Scope(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Scope() {}
    };

    class Exception : public QJsonObject
    {
    public:
        Exception() : QJsonObject() {}
        Exception(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Exception() {}
    };

    class Script : public QJsonObject
    {
    public:
        Script() : QJsonObject() {}
        Script(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Script() {}
    };

    class Source : public QJsonObject
    {
    public:
        Source() : QJsonObject() {}
        Source(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~Source() {}
    };

    class CommandRequest : public QJsonObject
    {
    public:
        CommandRequest() : QJsonObject() {}
        CommandRequest(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~CommandRequest() {}
    };

    class CommandResponse : public QJsonObject
    {
    public:
        CommandResponse() : QJsonObject() {}
        CommandResponse(const QJsonObject& object) : QJsonObject(object) {}
        virtual ~CommandResponse() {}
    };

    virtual int breakZ() = 0;
    virtual int continueZ(const CommandRequest& request) = 0;
    virtual int evaluate(const CommandRequest& request) = 0;
    virtual int lookup(const CommandRequest& request) = 0;
    virtual int getBacktrace(const CommandRequest& request) = 0;
    virtual int getFrame(const CommandRequest& request) = 0;
    virtual int getScope(const CommandRequest& request) = 0;
    virtual int getScopes(const CommandRequest& request) = 0;
    virtual int getScripts(const CommandRequest& request) = 0;
    virtual int getSource(const CommandRequest& request) = 0;
    virtual int setBreakpoint(const CommandRequest& request) = 0;
    virtual int changeBreakpoint(const CommandRequest& request) = 0;
    virtual int clearBreakpoint(const CommandRequest& request) = 0;
    virtual int setExceptionBreak(const CommandRequest& request) = 0;
    virtual int getFlags(const CommandRequest& request) = 0;
    virtual int getVersion(const CommandRequest& request) = 0;
    virtual int disconnect(const CommandRequest& request) = 0;
    virtual int gc(const CommandRequest& request) = 0;
    virtual int getListOfBreakpoints(const CommandRequest& request) = 0;
    virtual int setVariableValue(const CommandRequest& request) = 0;

signals:
    void response(const CommandResponse& response);

public slots:
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

#endif // SCRIPTINGENGINE_H
