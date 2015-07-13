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

#include "scriptingconsole.h"

namespace _9fold
{
namespace widgets
{
namespace scripting
{

class ScriptingConsole::ScriptingConsolePrivate
{
public:
    ScriptingConsolePrivate(TextEditor *display, TextEditor *input, ScriptingEngine *engine)
        : _display(display), _input(input), _engine(engine)
    {

    }

    TextEditor* display() const
    {
        return _display;
    }

    TextEditor* input() const
    {
        return _input;
    }

    ScriptingEngine* engine() const
    {
        return _engine;
    }

private:
    TextEditor *_display;
    TextEditor *_input;
    ScriptingEngine *_engine;
};

ScriptingConsole::ScriptingConsole(TextEditor *display, TextEditor *input,
    ScriptingEngine *engine, QWidget *parent)
    : QWidget(parent), _p(new ScriptingConsolePrivate(display, input, engine))
{

}

ScriptingConsole::~ScriptingConsole()
{

}

} // namespace scripting
} // namespace widgets
} // namespace _9fold
