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

class JavaScriptTextEditorPrivate;
class JavaScriptTextEditor : public TextEditor
{
    Q_OBJECT
public:
    typedef V8ScriptingEngine::V8Error V8Error;

    JavaScriptTextEditor(QWidget *parent = 0);
    virtual ~JavaScriptTextEditor();

    void displayError(const V8ScriptingEngine::V8Error &error);

signals:
    void addBreakpoint(int line);
    void clearBreakpoint(int line);

private slots:
    void onLinesChanged();
    void onMarginClicked(int margin, int line, Qt::KeyboardModifiers state);

private:
    JavaScriptTextEditorPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(JavaScriptTextEditor)
};

} // namespace editors
} // namespace widgets
} // namespace _9fold

#endif // JAVASCRIPTTEXTEDITOR_H
