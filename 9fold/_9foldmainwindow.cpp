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

#include "_9foldactionmanager.h"
#include "_9folddockmanager.h"
#include "_9folddocumentmanager.h"
#include "_9folddocumentviewmanager.h"
#include "_9foldmainwindow.h"
#include "_9foldmenumanager.h"
#include "_9foldtoolbarmanager.h"
#include "_9foldworkspace.h"

#include <QMainWindow>
#include <QScopedPointer>

namespace _9fold
{
namespace ui
{

using namespace workspaces;

class _9FoldMainWindow::_9FoldMainWindowPrivate
{
public:
    _9FoldMainWindowPrivate(QMainWindow *mainWindow, QWidget *parent)
    {
        QWidget *p = parent ? parent : mainWindow;

        QScopedPointer<_9FoldDocumentManager> documentManager(
            new _9FoldDocumentManager(new _9FoldDocumentViewManager(p), p));

        QScopedPointer<_9FoldActionManager> actionManager(
            new _9FoldActionManager(mainWindow, p));

        QScopedPointer<_9FoldToolBarManager> toolBarManager(
            new _9FoldToolBarManager(mainWindow, p));

        QScopedPointer<_9FoldDockManager> dockManager(
            new _9FoldDockManager(mainWindow, p));

        QScopedPointer<_9FoldMenuManager> menuManager(
            new _9FoldMenuManager(mainWindow, p));

        _workspace.reset(new _9FoldWorkspace(mainWindow, documentManager.take(),
            actionManager.take(), toolBarManager.take(), dockManager.take(),
            menuManager.take(), p));
    }

private:
    QScopedPointer<_9FoldWorkspace> _workspace;
};

_9FoldMainWindow::_9FoldMainWindow(QWidget *parent)
    : MainWindow(parent), _p(new _9FoldMainWindowPrivate(this, parent))
{

}

_9FoldMainWindow::~_9FoldMainWindow()
{

}

} // namespace ui
} // namespace _9fold
