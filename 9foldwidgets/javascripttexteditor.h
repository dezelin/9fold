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

#ifndef JAVASCRIPTTEXTEDITOR_H
#define JAVASCRIPTTEXTEDITOR_H

#include "texteditor.h"

#include <v8scriptingengine.h>

#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace widgets
{
namespace editors
{

using namespace _9fold::scripting::engine;

class JavaScriptTextEditor : public TextEditor
{
    Q_OBJECT
public:
    JavaScriptTextEditor(V8ScriptingEngine *engine, QWidget *parent = 0);
    virtual ~JavaScriptTextEditor();

    V8ScriptingEngine* engine() const;

    int debug();
    int run();

    int addBreakpoint(int line);
    int clearBreakpoint(int line);

signals:

private slots:
    void onError(const V8ScriptingEngine::V8Error& error);
    void onFinished(const QString& result);
    void onLinesChanged();
    void onMarginClicked(int margin, int line, Qt::KeyboardModifiers state);

private:
    class JavaScriptTextEditorPrivate;
    QScopedPointer<JavaScriptTextEditorPrivate> _p;
};

} // namespace editors
} // namespace widgets
} // namespace _9fold

#endif // JAVASCRIPTTEXTEDITOR_H
