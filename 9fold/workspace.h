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

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "actionmanager.h"
#include "dockmanager.h"
#include "documentmanager.h"
#include "menumanager.h"
#include "toolbarmanager.h"

#include <centralwidget.h>

#include <QDockWidget>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QScopedPointer>
#include <QToolBar>
#include <QWidget>

namespace _9fold
{
namespace workspaces
{

using namespace actions;
using namespace docks;
using namespace documents;
using namespace menus;
using namespace toolbars;
using namespace widgets;

class Workspace : public QObject
{
    Q_OBJECT
public:
    explicit Workspace(QMainWindow *mainWindow, DocumentManager *documentManager,
        ActionManager *actionManager, ToolBarManager *toolBarManager,
        DockManager *dockManager, MenuManager *menuManager, QObject *parent = 0);
    virtual ~Workspace();

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolBar);
    void addDock(Qt::DockWidgetArea area, QDockWidget *dock);

    void removeToolBar(QToolBar *toolBar);
    void removeDock(QDockWidget *dock);

    CentralWidget* centralWidget() const;
    virtual void setCentralWidget(CentralWidget *centralWidget);

    const QList<QDockWidget*>& docks() const;
    const QList<QMenu*>& menus() const;
    const QList<QToolBar*>& toolBars() const;

    ActionManager* actionManager() const;
    DockManager* dockManager() const;
    DocumentManager* documentManager() const;
    ToolBarManager* toolBarManager() const;

    QMainWindow* mainWindow() const;

signals:

public slots:

private:
    class WorkspacePrivate;
    QScopedPointer<WorkspacePrivate> _p;
};

} // namespace workspaces
} // namespace _9fold

#endif // WORKSPACE_H
