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

#ifndef SCRIPTINGCONSOLE_H
#define SCRIPTINGCONSOLE_H

#include "texteditor.h"

#include <scriptingengine.h>

#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace widgets
{
namespace scripting
{

using namespace _9fold::widgets::editors;
using namespace _9fold::scripting::engine;

class ScriptingConsole : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptingConsole(TextEditor *display, TextEditor *input,
        ScriptingEngine *engine, QWidget *parent = 0);
    virtual ~ScriptingConsole();

signals:

public slots:

private:
    class ScriptingConsolePrivate;
    QScopedPointer<ScriptingConsolePrivate> _p;
};

} // namespace scripting
} // namespace widgets
} // namespace _9fold

#endif // SCRIPTINGCONSOLE_H
