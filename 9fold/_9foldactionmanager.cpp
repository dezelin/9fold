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

#include "_9foldaction.h"
#include "_9foldactionmanager.h"
#include "_9foldworkspace.h"

namespace _9fold
{
namespace actions
{

_9FoldActionManager::_9FoldActionManager(QMainWindow *mainWindow,
    _9FoldCommandManager *commandManager, QObject *parent)
    : ActionManager(mainWindow, commandManager, parent)
{

}

_9FoldActionManager::~_9FoldActionManager()
{

}

QAction* _9FoldActionManager::createNew()
{
    _9FoldAction *action = new _9FoldAction(_commandManager(),
        _commandManager()->createNewJavaScriptCommand(), tr("&New JavaScript"),
        parent());

    return action;
}

QAction* _9FoldActionManager::createViewV8Console()
{
    _9FoldAction *action = new _9FoldAction(_commandManager(),
        _commandManager()->createViewJavaScriptConsoleCommand(), tr("View &JavaScript console"),
        parent());

    return action;
}

_9FoldCommandManager* _9FoldActionManager::_commandManager() const
{
    return static_cast<_9FoldCommandManager*>(commandManager());
}

_9FoldWorkspace* _9FoldActionManager::_workspace() const
{
    return static_cast<_9FoldWorkspace*>(workspace());
}

} // namespace actions
} // namespace _9fold
