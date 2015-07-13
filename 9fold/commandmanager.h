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

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QScopedPointer>

namespace _9fold
{

namespace workspaces
{
    class Workspace;
};

namespace commands
{

using namespace _9fold::workspaces;

class CommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = 0);
    virtual ~CommandManager();

    class BackgroundCommand
    {
    public:
        BackgroundCommand() {}
        virtual ~BackgroundCommand() {}

        virtual void execute() = 0;
    };

    class GuiCommand : public BackgroundCommand
    {
    public:
        GuiCommand() : BackgroundCommand()
        {

        }
    };

    Workspace* workspace() const;
    void setWorkspace(Workspace *workspace);

    void enqueue(BackgroundCommand *command);
    void enqueue(GuiCommand *command);

signals:

public slots:

private:
    class CommandManagerPrivate;
    QScopedPointer<CommandManagerPrivate> _p;
};

} // namespace commands
} // namespace _9fold

#endif // COMMANDMANAGER_H
