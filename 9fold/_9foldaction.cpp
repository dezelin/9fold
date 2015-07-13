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

namespace _9fold
{
namespace actions
{

class _9FoldAction::_9FoldActionPrivate
{
public:
    _9FoldActionPrivate(_9FoldCommandManager *commandManager,
        _9FoldCommandManager::GuiCommand *command)
        : _commandManager(commandManager), _command(command)
    {
    }

    _9FoldCommandManager* commandManager() const
    {
        return _commandManager;
    }

    _9FoldCommandManager::GuiCommand* command() const
    {
        return _command.data();
    }

    void setCommandManager(_9FoldCommandManager* commandManager)
    {
        _commandManager = commandManager;
    }

    void setCommand(_9FoldCommandManager::GuiCommand *command)
    {
        _command.reset(command);
    }

    void enqueue()
    {
        Q_ASSERT(_commandManager);
        Q_ASSERT(_command);
        _commandManager->enqueue(_command.data());
    }

private:
    _9FoldCommandManager *_commandManager;
    QScopedPointer<_9FoldCommandManager::GuiCommand> _command;
};

_9FoldAction::_9FoldAction(_9FoldCommandManager *commandManager,
    CommandManager::GuiCommand *command, QObject *parent)
    : QAction(parent), _p(new _9FoldActionPrivate(commandManager, command))
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(trigger()));
}

_9FoldAction::_9FoldAction(_9FoldCommandManager *commandManager,
    CommandManager::GuiCommand *command, const QString &text, QObject* parent)
    : QAction(text, parent), _p(new _9FoldActionPrivate(commandManager, command))
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(trigger()));
}

_9FoldAction::_9FoldAction(_9FoldCommandManager *commandManager,
    CommandManager::GuiCommand *command, const QIcon &icon,
    const QString &text, QObject* parent)
    : QAction(icon, text, parent), _p(new _9FoldActionPrivate(commandManager, command))
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(trigger()));
}


_9FoldAction::~_9FoldAction()
{

}

_9FoldCommandManager* _9FoldAction::commandManager() const
{
    return _p->commandManager();
}

CommandManager::GuiCommand *_9FoldAction::command() const
{
    return _p->command();
}

void _9FoldAction::setCommandManager(_9FoldCommandManager *commandManager)
{
    Q_ASSERT(commandManager);
    if (!commandManager)
        return;

    _p->setCommandManager(commandManager);
}

void _9FoldAction::setCommand(_9FoldCommandManager::GuiCommand *command)
{
    Q_ASSERT(command);
    if (!command)
        return;

    _p->setCommand(command);
}


void _9FoldAction::trigger()
{
    _p->enqueue();
}

} // namespace actions
} // namespace _9fold
