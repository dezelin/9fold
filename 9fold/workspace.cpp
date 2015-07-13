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

#include "workspace.h"

#include "actionmanager.h"
#include "commandmanager.h"
#include "dockmanager.h"
#include "documentmanager.h"
#include "documentviewmanager.h"
#include "menumanager.h"
#include "toolbarmanager.h"

#include <QtGlobal>
#include <QList>

namespace _9fold
{
namespace workspaces
{

class Workspace::WorkspacePrivate
{
public:
    WorkspacePrivate(QMainWindow *mainWindow, CommandManager *commandManager,
        DocumentManager *documentManager, ActionManager *actionManager,
        ToolBarManager *toolBarManager, DockManager *dockManager,
        MenuManager *menuManager)
        : _mainWindow(mainWindow)
        , _actionManager(actionManager)
        , _commandManager(commandManager)
        , _dockManager(dockManager)
        , _documentManager(documentManager)
        , _menuManager(menuManager)
        , _toolBarManager(toolBarManager)
    {
    }

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolBar)
    {
        Q_ASSERT(_toolBarManager);
        _toolBarManager->addToolBar(area, toolBar);
    }

    void addDock(Qt::DockWidgetArea area, QDockWidget *dock)
    {
        Q_ASSERT(_dockManager);
        _dockManager->addDock(area, dock);
    }

    void removeToolBar(QToolBar *toolBar)
    {
        Q_ASSERT(_toolBarManager);
        _toolBarManager->removeToolBar(toolBar);
    }

    void removeDock(QDockWidget *dock)
    {
        Q_ASSERT(_dockManager);
        _dockManager->removeDock(dock);
    }

    void setCentralWidget(CentralWidget *centralWidget)
    {
        Q_ASSERT(_mainWindow);
        if (!_mainWindow)
            return;

        _mainWindow->setCentralWidget(centralWidget);
    }

    const QList<QToolBar*>& toolBars() const
    {
        Q_ASSERT(_toolBarManager);
        return _toolBarManager->toolBars();
    }

    const QList<QDockWidget*>& docks() const
    {
        Q_ASSERT(_dockManager);
        return _dockManager->docks();
    }

    const QList<QMenu*>& menus() const
    {
        Q_ASSERT(_menuManager);
        return _menuManager->menus();
    }

    CentralWidget* centralWidget() const
    {
        Q_ASSERT(_mainWindow);
        return static_cast<CentralWidget*>(_mainWindow->centralWidget());
    }

    ActionManager* actionManager() const
    {
        return _actionManager;
    }

    CommandManager* commandManager() const
    {
        return _commandManager;
    }

    DockManager* dockManager() const
    {
        return _dockManager;
    }

    DocumentManager* documentManager() const
    {
        return _documentManager;
    }

    MenuManager* menuManager() const
    {
        return _menuManager;
    }

    ToolBarManager* toolBarManager() const
    {
        return _toolBarManager;
    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

private:
    QMainWindow *_mainWindow;
    ActionManager *_actionManager;
    CommandManager *_commandManager;
    DockManager *_dockManager;
    DocumentManager *_documentManager;
    MenuManager *_menuManager;
    ToolBarManager *_toolBarManager;
};

Workspace::Workspace(QMainWindow *mainWindow, CommandManager *commandManager,
    DocumentManager *documentManager, ActionManager *actionManager,
    ToolBarManager *toolBarManager, DockManager *dockManager,
    MenuManager *menuManager, QObject *parent)
    : QObject(parent)
    , _p(new WorkspacePrivate(mainWindow, commandManager, documentManager,
        actionManager, toolBarManager, dockManager, menuManager))
{
    if (actionManager) {
        actionManager->setParent(this);
        actionManager->setWorkspace(this);
    }
    if (commandManager) {
        commandManager->setParent(this);
        commandManager->setWorkspace(this);
    }
    if (dockManager)
        dockManager->setParent(this);
    if (documentManager)
        documentManager->setParent(this);
    if (menuManager)
        menuManager->setParent(this);
    if (toolBarManager)
        toolBarManager->setParent(this);
}

Workspace::~Workspace()
{

}

void Workspace::addToolBar(Qt::ToolBarArea area, QToolBar *toolBar)
{
    Q_ASSERT(toolBar);
    if (!toolBar)
        return;

    _p->addToolBar(area, toolBar);
}

void Workspace::addDock(Qt::DockWidgetArea area, QDockWidget *dock)
{
    Q_ASSERT(dock);
    if (!dock)
        return;

    _p->addDock(area, dock);
}

void Workspace::removeToolBar(QToolBar *toolBar)
{
    Q_ASSERT(toolBar);
    if (!toolBar)
        return;

    _p->removeToolBar(toolBar);
}

void Workspace::removeDock(QDockWidget *dock)
{
    Q_ASSERT(dock);
    if (!dock)
        return;

    _p->removeDock(dock);
}

void Workspace::setCentralWidget(CentralWidget *centralWidget)
{
    Q_ASSERT(centralWidget);
    if (!centralWidget)
        return;

    _p->setCentralWidget(centralWidget);
}

const QList<QDockWidget*>& Workspace::docks() const
{
    return _p->docks();
}

const QList<QMenu*>& Workspace::menus() const
{
    return _p->menus();
}

const QList<QToolBar*>& Workspace::toolBars() const
{
    return _p->toolBars();
}

CentralWidget* Workspace::centralWidget() const
{
    return _p->centralWidget();
}

ActionManager* Workspace::actionManager() const
{
    return _p->actionManager();
}

CommandManager* Workspace::commandManager() const
{
    return _p->commandManager();
}

DockManager* Workspace::dockManager() const
{
    return _p->dockManager();
}

DocumentManager* Workspace::documentManager() const
{
    return _p->documentManager();
}

MenuManager* Workspace::menuManager() const
{
    return _p->menuManager();
}

ToolBarManager* Workspace::toolBarManager() const
{
    return _p->toolBarManager();
}

QMainWindow* Workspace::mainWindow() const
{
    return _p->mainWindow();
}

} // namespace workspaces
} // namespace _9fold
