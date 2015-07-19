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

#ifndef _9FOLDCOMMANDMANAGER_H
#define _9FOLDCOMMANDMANAGER_H

#include "commandmanager.h"

#include <QObject>

namespace _9fold
{

namespace workspaces
{
    class _9FoldWorkspace;
}

namespace commands
{

using namespace _9fold::commands;

class _9FoldCommandManager : public CommandManager
{
    Q_OBJECT
public:
    _9FoldCommandManager(QObject *parent = 0);
    virtual ~_9FoldCommandManager();

    GuiCommand* createNewJavaScriptCommand() const;
    GuiCommand* createScriptDebugCommand() const;
    GuiCommand* createScriptRunCommand() const;
    GuiCommand* createViewJavaScriptConsoleCommand() const;

private:
    _9FoldWorkspace* _workspace() const;
};

} // namespace commands
} // namespace _9fold

#endif // _9FOLDCOMMANDMANAGER_H
