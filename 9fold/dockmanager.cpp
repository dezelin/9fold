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

#include "dockmanager.h"

namespace _9fold
{
namespace docks
{

class DockManager::DockManagerPrivate
{
public:
    DockManagerPrivate(QMainWindow *mainWindow)
        : _mainWindow(mainWindow)
    {

    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

    void addDock(Qt::DockWidgetArea area, QDockWidget *dock)
    {
        Q_ASSERT(_mainWindow);
        if (_docks.contains(dock))
            return;

        _docks.append(dock);
        _mainWindow->addDockWidget(area, dock);
    }

    void removeDock(QDockWidget *dock)
    {
        Q_ASSERT(_mainWindow);
        if (!_docks.contains(dock))
            return;

        _docks.removeAll(dock);
        _mainWindow->removeDockWidget(dock);
    }

    const QList<QDockWidget*>& docks() const
    {
        return _docks;
    }

private:
    QMainWindow *_mainWindow;
    QList<QDockWidget*> _docks;
};

DockManager::DockManager(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), _p(new DockManagerPrivate(mainWindow))
{

}

DockManager::~DockManager()
{

}

void DockManager::addDock(Qt::DockWidgetArea area, QDockWidget *dock)
{
    Q_ASSERT(dock);
    if (!dock)
        return;

    _p->addDock(area, dock);
}

void DockManager::removeDock(QDockWidget *dock)
{
    Q_ASSERT(dock);
    if (!dock)
        return;

    _p->removeDock(dock);
}

const QList<QDockWidget*>& DockManager::docks() const
{
    return _p->docks();
}

QMainWindow* DockManager::mainWindow() const
{
    return _p->mainWindow();
}

} // namespace docks
} // namespace _9fold
