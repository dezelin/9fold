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

#include "actionmanager.h"

namespace _9fold
{
namespace actions
{

class ActionManager::ActionManagerPrivate
{
public:
    ActionManagerPrivate(QMainWindow *mainWindow, CommandManager *commandManager)
        : _mainWindow(mainWindow), _commandManager(commandManager), _workspace(0)
    {

    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

    CommandManager* commandManager() const
    {
        return _commandManager;
    }

    Workspace* workspace() const
    {
        Q_ASSERT(_workspace);
        return _workspace;
    }

    void setWorkspace(Workspace *workspace)
    {
        _workspace = workspace;
    }

private:
    QMainWindow *_mainWindow;
    CommandManager *_commandManager;
    Workspace *_workspace;
};

ActionManager::ActionManager(QMainWindow *mainWindow, CommandManager *commandManager,
        QObject *parent)
    : QObject(parent), _p(new ActionManagerPrivate(mainWindow, commandManager))
{

}

ActionManager::~ActionManager()
{

}

QMainWindow* ActionManager::mainWindow() const
{
    return _p->mainWindow();
}

CommandManager* ActionManager::commandManager() const
{
    return _p->commandManager();
}

Workspace* ActionManager::workspace() const
{
    return _p->workspace();
}

void ActionManager::setWorkspace(Workspace *workspace)
{
    _p->setWorkspace(workspace);
}

} // namespace actions
} // namespace _9fold
