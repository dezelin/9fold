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

#include "scriptingconsoleeditor.h"

#include <QVector>

#include <Qsci/qscistyle.h>

namespace _9fold
{
namespace widgets
{
namespace editors
{

class ScriptingConsoleEditor::ScriptingConsoleEditorPrivate
{
public:
    ScriptingConsoleEditorPrivate()
        : _line(0)
        , _index(0)
        , _historyIndex(0)
    {

    }

    void addHistory(const QString& text)
    {
        _history.append(text);
        QVector<QString>::const_iterator it = std::unique(_history.begin(),
            _history.end());
        _history.resize(it - _history.begin());
        resetHistoryIndex();
    }

    int line() const
    {
        return _line;
    }

    int index() const
    {
        return _index;
    }

    void setLine(int line)
    {
        _line = line;
    }

    void setIndex(int index)
    {
        _index = index;
    }

    bool emptyHistory()
    {
        return _history.length() == 0;
    }

    QString previousHistoryIndex()
    {
        if (emptyHistory())
            return QString();

        return _history[_historyIndex > 0 ? --_historyIndex : 0];
    }

    QString nextHistoryIndex()
    {
        if (emptyHistory())
            return QString();

        if (_historyIndex == _history.length())
            return QString();

        return _history[_historyIndex < _history.length() - 1 ?
            ++_historyIndex : _history.length() - 1];
    }

    void resetHistoryIndex()
    {
        _historyIndex = _history.length();
    }

private:
    int _line;
    int _index;
    int _historyIndex;
    QVector<QString> _history;
};

ScriptingConsoleEditor::ScriptingConsoleEditor(QWidget *parent)
    : TextEditor(parent), _p(new ScriptingConsoleEditorPrivate())
{
    setMarginType(1, QsciScintilla::TextMarginRightJustified);
    setMarginsBackgroundColor(paper());
    setMarginText(0, QString(">"), QsciStyle());

    connect(this, SIGNAL(keyPressEnter()), this, SLOT(onKeyPressEnter()));
    connect(this, SIGNAL(keyPressUp()), this, SLOT(onKeyPressUp()));
    connect(this, SIGNAL(keyPressDown()), this, SLOT(onKeyPressDown()));
    connect(this, SIGNAL(cursorPositionChanged(int,int)), this,
        SLOT(onCursorPositionChanged(int,int)));
}

ScriptingConsoleEditor::~ScriptingConsoleEditor()
{

}

void ScriptingConsoleEditor::addText(const QString& text)
{
    insert(text + '\n');
    setMarginText(lines() - 1, QString(">"), QsciStyle());
    setCursorPosition(lines(), 0);
}

void ScriptingConsoleEditor::keyPressEvent(QKeyEvent *event)
{
    int line, index;
    getCursorPosition(&line, &index);

    switch (event->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter: {
        TextEditor::keyPressEvent(event);
        emit keyPressEnter();
        return;
    }
    case Qt::Key_Up: {
        emit keyPressUp();
        return;
    }
    case Qt::Key_Down: {
        emit keyPressDown();
        return;
    }
    case Qt::Key_Left: {
        emit keyPressLeft();
        if (index == 0)
            return;

        break;
    }
    case Qt::Key_Backspace: {
        emit keyPressBackspace();
        if (index == 0)
            return;

        break;
    }
    default: {
        break;
    }
    }

    TextEditor::keyPressEvent(event);
}

void ScriptingConsoleEditor::mousePressEvent(QMouseEvent *event)
{
    TextEditor::mousePressEvent(event);
}

void ScriptingConsoleEditor::mouseReleaseEvent(QMouseEvent *event)
{
    TextEditor::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton && selectedText().length() == 0)
        setCursorPosition(_p->line() + 1, _p->index());
}

void ScriptingConsoleEditor::onKeyPressEnter()
{
    // Skip return if cursor is not in the prompt line
    int line, index;
    getCursorPosition(&line, &index);
    if (line != lines() - 1)
        return;

    // FIXME: A bug in QScintilla?
    QString enteredText = text(lines() - 2).trimmed();
    if (enteredText.length() == 0)
        return;

    _p->addHistory(enteredText);
    emit execute(enteredText);
}

void ScriptingConsoleEditor::onKeyPressUp()
{
    int line, index;
    getCursorPosition(&line, &index);

    QString history = _p->previousHistoryIndex();
    if (history.isEmpty())
        return;

    setSelection(line, 0, line, lineLength(line));
    replaceSelectedText(history);
}

void ScriptingConsoleEditor::onKeyPressDown()
{
    int line, index;
    getCursorPosition(&line, &index);

    QString history = _p->nextHistoryIndex();
    if (history.isEmpty())
        return;

    setSelection(line, 0, line, lineLength(line));
    replaceSelectedText(history);
}

void ScriptingConsoleEditor::onCursorPositionChanged(int line, int index)
{
    _p->setLine(line);
    _p->setIndex(index);

    // Set read-only flag when cursor is not in the prompt line
    setReadOnly(line != lines() - 1);
    setCaretWidth(line != lines() - 1 ? 0 : 1);
}

} // namespace editors;
} // namespace widgets
} // namespace _9fold
