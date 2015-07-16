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

#ifndef SCRIPTINGCONSOLEEDITOR_H
#define SCRIPTINGCONSOLEEDITOR_H

#include "texteditor.h"

#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace widgets
{
namespace editors
{

class ScriptingConsoleEditor : public TextEditor
{
    Q_OBJECT
public:
    ScriptingConsoleEditor(QWidget *parent = 0);
    virtual ~ScriptingConsoleEditor();

    void addText(const QString& text);

signals:
    void keyPressBackspace();
    void keyPressEnter();
    void keyPressLeft();
    void keyPressUp();
    void keyPressDown();

    void execute(const QString& text);

protected slots:
    void onKeyPressEnter();
    void onKeyPressUp();
    void onKeyPressDown();
    void onCursorPositionChanged(int line, int index);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    class ScriptingConsoleEditorPrivate;
    QScopedPointer<ScriptingConsoleEditorPrivate> _p;
};

} // namespace editors;
} // namespace widgets
} // namespace _9fold

#endif // SCRIPTINGCONSOLEEDITOR_H
