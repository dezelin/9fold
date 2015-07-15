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

#include "_9foldworkspace.h"

#include "_9foldactionmanager.h"
#include "_9foldcommandmanager.h"
#include "_9folddockmanager.h"
#include "_9folddocumentmanager.h"
#include "_9foldmenumanager.h"
#include "_9foldtoolbarmanager.h"

#include <_9foldcentralwidget.h>
#include <v8scriptingengineconsole.h>


namespace _9fold
{
namespace workspaces
{

using namespace _9fold::widgets::scripting;

_9FoldWorkspace::_9FoldWorkspace(QMainWindow *mainWindow,
    _9FoldCommandManager *commandManager, _9FoldDocumentManager *documentManager,
    _9FoldActionManager *actionManager, _9FoldToolBarManager *toolBarManager,
    _9FoldDockManager *dockManager, _9FoldMenuManager *menuManager, QObject *parent)
    : Workspace(mainWindow, commandManager, documentManager, actionManager,
        toolBarManager, dockManager, menuManager, parent)
{
    _menuManager()->addDefaultMenus();

    setCentralWidget(new _9FoldCentralWidget(mainWindow));
}

_9FoldWorkspace::~_9FoldWorkspace()
{

}

_9FoldDocumentManager* _9FoldWorkspace::_documentManager() const
{
    return static_cast<_9FoldDocumentManager*>(documentManager());
}

_9FoldMenuManager* _9FoldWorkspace::_menuManager() const
{
    return static_cast<_9FoldMenuManager*>(menuManager());
}

void _9FoldWorkspace::addNewJavaScriptDocument()
{
    Q_ASSERT(_documentManager());
    QScopedPointer<_9FoldDocument> document(_documentManager()->createNewJavaScriptDocument());

    DocumentPresenter *presenter = document->firstPresenter();
    Q_ASSERT(presenter);

    DocumentView *view = presenter->view();
    Q_ASSERT(view);

    centralWidget()->addTab(view, document->name() + "*");

    _documentManager()->addDocument(document.take());
}

void _9FoldWorkspace::viewJavaScriptConsole()
{
    QScopedPointer<QDockWidget> dock(new QDockWidget(tr("JavaScript console"),
        mainWindow()));
    dock->setWidget(new V8ScriptingEngineConsole());
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    addDock(Qt::BottomDockWidgetArea, dock.take());
}

} // namespace workspaces
} // namespace _9fold
