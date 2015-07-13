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

#ifndef _9FOLDACTION_H
#define _9FOLDACTION_H

#include "_9foldcommandmanager.h"

#include <QAction>
#include <QObject>
#include <QScopedPointer>

namespace _9fold
{
namespace actions
{

using namespace _9fold::commands;

class _9FoldAction : public QAction
{
    Q_OBJECT
public:
    // Owns only command instance
    explicit _9FoldAction(_9FoldCommandManager *commandManager,
        _9FoldCommandManager::GuiCommand *command, QObject *parent = 0);
    _9FoldAction(_9FoldCommandManager *commandManager,
        _9FoldCommandManager::GuiCommand *command, const QString &text,
        QObject* parent);
    _9FoldAction(_9FoldCommandManager *commandManager,
        _9FoldCommandManager::GuiCommand *command, const QIcon &icon,
        const QString &text, QObject* parent);

    virtual ~_9FoldAction();

    _9FoldCommandManager* commandManager() const;
    _9FoldCommandManager::GuiCommand* command() const;

    void setCommandManager(_9FoldCommandManager *commandManager);
    void setCommand(CommandManager::GuiCommand *command);

signals:

public slots:

private slots:
    void trigger();

private:
    class _9FoldActionPrivate;
    QScopedPointer<_9FoldActionPrivate> _p;
};

} // namespace actions
} // namespace _9fold

#endif // _9FOLDACTION_H
