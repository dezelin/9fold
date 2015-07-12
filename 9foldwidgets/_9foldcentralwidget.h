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

#ifndef _9FOLDCENTRALWIDGET_H
#define _9FOLDCENTRALWIDGET_H

#include "centralwidget.h"

#include <QObject>
#include <QWidget>

namespace _9fold
{
namespace widgets
{

class _9FoldCentralWidget : public CentralWidget
{
    Q_OBJECT
public:
    _9FoldCentralWidget(QWidget *parent);
    virtual ~_9FoldCentralWidget();
};

} // namespace widgets
} // namespace _9fold

#endif // _9FOLDCENTRALWIDGET_H
