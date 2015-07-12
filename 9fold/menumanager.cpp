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

#include "menumanager.h"

namespace _9fold
{
namespace menus
{

class MenuManager::MenuManagerPrivate
{
public:
    MenuManagerPrivate(QMainWindow *mainWindow)
        : _mainWindow(mainWindow)
    {

    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

    const QList<QMenu*>& menus() const
    {
        return _menus;
    }

private:
    QMainWindow *_mainWindow;
    QList<QMenu*> _menus;
};

MenuManager::MenuManager(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), _p(new MenuManagerPrivate(mainWindow))
{

}

MenuManager::~MenuManager()
{

}

QMainWindow* MenuManager::mainWindow() const
{
    return _p->mainWindow();
}

const QList<QMenu*>& MenuManager::menus() const
{
    return _p->menus();
}

} // namespace menus
} // namespace _9fold
