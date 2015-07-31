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

#include <QVBoxLayout>

namespace _9fold
{
namespace widgets
{
namespace scripting
{

class ScriptingConsole::ScriptingConsolePrivate
{
public:
    ScriptingConsolePrivate(ScriptingConsoleEditor *editor, ScriptingEngine *engine)
        : _editor(editor), _engine(engine)
    {

    }

    ScriptingConsoleEditor* editor() const
    {
        return _editor;
    }

    ScriptingEngine* engine() const
    {
        return _engine;
    }

private:
    ScriptingConsoleEditor *_editor;
    ScriptingEngine *_engine;
};

ScriptingConsole::ScriptingConsole(ScriptingConsoleEditor *editor,
    ScriptingEngine *engine, QWidget *parent)
    : QWidget(parent), _p(new ScriptingConsolePrivate(editor, engine))
{
    Q_ASSERT(editor);
    Q_ASSERT(engine);

    editor->setParent(this);
    engine->setParent(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(editor);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(editor, SIGNAL(execute(QString)), this, SLOT(execute(QString)));
}

ScriptingConsole::~ScriptingConsole()
{

}

ScriptingConsoleEditor* ScriptingConsole::editor() const
{
    return _p->editor();
}

ScriptingEngine* ScriptingConsole::engine()
{
    return _p->engine();
}

void ScriptingConsole::execute(const QString& text)
{
    QString result = engine()->run(QString(), text);
    editor()->addText(result);
}

} // namespace scripting
} // namespace widgets
} // namespace _9fold
