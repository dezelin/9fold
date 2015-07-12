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

#include "toolbarmanager.h"

#include <QList>

namespace _9fold
{
namespace toolbars
{

class ToolBarManager::ToolBarManagerPrivate
{
public:
    ToolBarManagerPrivate(QMainWindow *mainWindow)
        : _mainWindow(mainWindow)
    {

    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolBar)
    {
        Q_ASSERT(_mainWindow);
        if (_toolBars.contains(toolBar))
            return;

        _toolBars.append(toolBar);
        _mainWindow->addToolBar(area, toolBar);
    }

    void removeToolBar(QToolBar *toolBar)
    {
        Q_ASSERT(_mainWindow);
        if (!_toolBars.contains(toolBar))
            return;

        _toolBars.removeAll(toolBar);
        _mainWindow->removeToolBar(toolBar);
    }

    const QList<QToolBar*>& toolBars() const
    {
        return _toolBars;
    }

private:
    QMainWindow *_mainWindow;
    QList<QToolBar*> _toolBars;
};

ToolBarManager::ToolBarManager(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), _p(new ToolBarManagerPrivate(mainWindow))
{

}

ToolBarManager::~ToolBarManager()
{

}

QMainWindow* ToolBarManager::mainWindow() const
{
    return _p->mainWindow();
}

void ToolBarManager::addToolBar(Qt::ToolBarArea area, QToolBar *toolBar)
{
    Q_ASSERT(toolBar);
    if (!toolBar)
        return;

    _p->addToolBar(area, toolBar);
}

void ToolBarManager::removeToolBar(QToolBar *toolBar)
{
    Q_ASSERT(toolBar);
    if (!toolBar)
        return;

    _p->removeToolBar(toolBar);
}

const QList<QToolBar*>& ToolBarManager::toolBars() const
{
    return _p->toolBars();
}

} // namespace toolbars
} // namespace _9fold
