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

#ifndef DOCKMANAGER_H
#define DOCKMANAGER_H

#include <QDockWidget>
#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace docks
{

class DockManager : public QObject
{
    Q_OBJECT
public:
    explicit DockManager(QMainWindow *mainWindow, QObject *parent = 0);
    virtual ~DockManager();

    void addDock(Qt::DockWidgetArea area, QDockWidget *dock);
    void removeDock(QDockWidget *dock);

    const QList<QDockWidget*>& docks() const;

    QMainWindow* mainWindow() const;

signals:

public slots:

private:
    class DockManagerPrivate;
    QScopedPointer<DockManagerPrivate> _p;
};

} // namespace docks
} // namespace _9fold

#endif // DOCKMANAGER_H
