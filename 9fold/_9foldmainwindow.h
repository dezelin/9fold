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

#ifndef _9FOLDMAINWINDOW_H
#define _9FOLDMAINWINDOW_H

#include "mainwindow.h"

#include <QMainWindow>
#include <QObject>
#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace ui
{

class _9FoldMainWindow : public MainWindow
{
    Q_OBJECT
public:
    _9FoldMainWindow(QWidget *parent = 0);
    virtual ~_9FoldMainWindow();

private:
    class _9FoldMainWindowPrivate;
    QScopedPointer<_9FoldMainWindowPrivate> _p;
};

} // namespace ui
} // namespace _9fold

#endif // _9FOLDMAINWINDOW_H
