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

#include "_9foldcommandmanager.h"
#include "_9foldworkspace.h"

namespace _9fold
{
namespace commands
{

_9FoldCommandManager::_9FoldCommandManager(QObject *parent)
    : CommandManager(parent)
{

}

_9FoldCommandManager::~_9FoldCommandManager()
{

}

_9FoldWorkspace* _9FoldCommandManager::_workspace() const
{
    return static_cast<_9FoldWorkspace*>(workspace());
}

_9FoldCommandManager::GuiCommand* _9FoldCommandManager::createNewJavaScriptCommand() const
{
    struct NewJavaScriptCommand : public GuiCommand
    {
        NewJavaScriptCommand(_9FoldWorkspace *workspace) : GuiCommand(), _workspace(workspace)
        {

        }

        virtual void execute()
        {
            Q_ASSERT(_workspace);
            _workspace->addNewJavaScriptDocument();
        }

    private:
        _9FoldWorkspace *_workspace;
    };

    return new NewJavaScriptCommand(_workspace());
}

} // namespace commands
} // namespace _9fold
