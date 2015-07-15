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

#ifndef _9FOLDWORKSPACE_H
#define _9FOLDWORKSPACE_H

#include "workspace.h"

#include <QMainWindow>
#include <QObject>
#include <QWidget>

namespace _9fold
{

namespace actions
{
    class _9FoldActionManager;
}

namespace commands
{
    class _9FoldCommandManager;
}

namespace docks
{
    class _9FoldDockManager;
}

namespace documents
{
    class _9FoldDocumentManager;
}

namespace menus
{
    class _9FoldMenuManager;
}

namespace toolbars
{
    class _9FoldToolBarManager;
};


namespace workspaces
{

using namespace _9fold::actions;
using namespace _9fold::docks;
using namespace _9fold::documents;
using namespace _9fold::menus;
using namespace _9fold::toolbars;

class _9FoldWorkspace : public Workspace
{
    Q_OBJECT
public:
    _9FoldWorkspace(QMainWindow *mainWindow, _9FoldCommandManager *commandManager,
        _9FoldDocumentManager *documentManager, _9FoldActionManager *actionManager,
        _9FoldToolBarManager *toolBarManager, _9FoldDockManager *dockManager,
        _9FoldMenuManager *menuManager, QObject *parent = 0);

    virtual ~_9FoldWorkspace();

    void addNewJavaScriptDocument();
    void viewJavaScriptConsole();

protected:
    _9FoldDocumentManager* _documentManager() const;
    _9FoldMenuManager* _menuManager() const;
};

} // namespace workspaces
} // namespace _9fold

#endif // _9FOLDWORKSPACE_H
