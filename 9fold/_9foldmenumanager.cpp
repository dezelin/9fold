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

#include "_9foldmenumanager.h"

#include <QMenuBar>
#include <QScopedPointer>

namespace _9fold
{
namespace menus
{

_9FoldMenuManager::_9FoldMenuManager(QMainWindow *mainWindow,
    _9FoldActionManager *actionManager, QObject *parent)
    : MenuManager(mainWindow, actionManager, parent)
{

}

_9FoldMenuManager::~_9FoldMenuManager()
{

}

void _9FoldMenuManager::addDefaultMenus()
{
    addMenu(createFileMenu());
    addMenu(createScriptMenu());
    addMenu(createViewMenu());
}

QMenu* _9FoldMenuManager::createFileMenu()
{
    QMenu *menu = new QMenu(tr("&File"), mainWindow());
    menu->insertAction(0, _actionManager()->createNew());
    return menu;
}

QMenu* _9FoldMenuManager::createScriptMenu()
{
    QMenu *menu = new QMenu(tr("&Script"), mainWindow());
    menu->insertAction(0, _actionManager()->createScriptRun());
    menu->insertAction(0, _actionManager()->createScriptDebug());
    return menu;
}

QMenu* _9FoldMenuManager::createViewMenu()
{
    QMenu *menu = new QMenu(tr("&View"), mainWindow());
    menu->insertAction(0, _actionManager()->createViewV8Console());
    return menu;
}

_9FoldActionManager* _9FoldMenuManager::_actionManager() const
{
    return static_cast<_9FoldActionManager*>(actionManager());
}

} // namespace menus
} // namespace _9fold
