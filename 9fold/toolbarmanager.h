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

#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace toolbars
{

class ToolBarManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolBarManager(QMainWindow *mainWindow, QObject *parent = 0);
    virtual ~ToolBarManager();

    QMainWindow* mainWindow() const;

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolBar);
    void removeToolBar(QToolBar *toolBar);

    const QList<QToolBar*>& toolBars() const;

signals:

public slots:

private:
    class ToolBarManagerPrivate;
    QScopedPointer<ToolBarManagerPrivate> _p;
};

} // namespace toolbars
} // namespace _9fold

#endif // TOOLBARMANAGER_H
