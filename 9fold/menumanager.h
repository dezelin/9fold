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

#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "actionmanager.h"

#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace menus
{

using namespace _9fold::actions;

class MenuManager : public QObject
{
    Q_OBJECT
public:
    explicit MenuManager(QMainWindow *mainWindow, ActionManager *actionManager,
        QObject *parent = 0);
    virtual ~MenuManager();

    QMainWindow* mainWindow() const;

    ActionManager* actionManager() const;

    const QList<QMenu*>& menus() const;

    void addMenu(QMenu *menu);

signals:

public slots:

private:
    class MenuManagerPrivate;
    QScopedPointer<MenuManagerPrivate> _p;
};

} // namespace menus
} // namespace _9fold

#endif // MENUMANAGER_H
