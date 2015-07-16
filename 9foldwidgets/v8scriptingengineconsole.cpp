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

#include "javascriptconsoleeditor.h"
#include "v8scriptingengineconsole.h"

#include <v8scriptingengine.h>

namespace _9fold
{
namespace widgets
{
namespace scripting
{

using namespace _9fold::scripting::engine;

using namespace _9fold::widgets::editors;

V8ScriptingEngineConsole::V8ScriptingEngineConsole(QWidget *parent)
    : ScriptingConsole(new JavaScriptConsoleEditor(), new V8ScriptingEngine(), parent)
{
    QString version = "Google V8 engine version " + engine()->version();
    editor()->addText(version);
}

V8ScriptingEngineConsole::~V8ScriptingEngineConsole()
{

}

} // namespace scripting
} // namespace widgets
} // namespace _9fold
