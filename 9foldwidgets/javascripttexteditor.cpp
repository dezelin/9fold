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

#include "javascripttexteditor.h"

#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscistyle.h>

#include <QVector>

namespace _9fold
{
namespace widgets
{
namespace editors
{

const int kMarginLineNumbers = 0;
const int kMarginSymbols = 1;

const int kBreakpointMarker = 8;
const int kErrorMarker = 9;

const int kAdditionalMarginWidth = 6;

class JavaScriptTextEditorPrivate
{
public:
    JavaScriptTextEditorPrivate(JavaScriptTextEditor *parent)
        : q_ptr(parent)
    {

    }

    void addBreakpoint(int line)
    {
        if (_breakpoints.contains(line))
            return;

        _breakpoints.push_back(line);
    }

    void clearBreakpoint(int line)
    {
        _breakpoints.removeAll(line);
    }

    const QVector<int>& breakpoints() const
    {
        return _breakpoints;
    }

private:
    JavaScriptTextEditor* const q_ptr;
    Q_DECLARE_PUBLIC(JavaScriptTextEditor)

private:
    QVector<int> _breakpoints;
};

JavaScriptTextEditor::JavaScriptTextEditor(QWidget *parent)
    : TextEditor(parent), d_ptr(new JavaScriptTextEditorPrivate(this))
{
    setLexer(new QsciLexerJavaScript(this));
    setWrapMode(QsciScintilla::WrapCharacter);

    QFontMetrics fontmetrics = QFontMetrics(font());
    setMarginsFont(font());
    setMarginsBackgroundColor(paper());

    setMarginWidth(kMarginLineNumbers, fontmetrics.width(QString::number(lines()) + kAdditionalMarginWidth));
    setMarginLineNumbers(kMarginLineNumbers, true);

    setMarginWidth(kMarginSymbols, fontmetrics.width(QString::number(0) + kAdditionalMarginWidth));
    setMarginSensitivity(kMarginSymbols, true);

    markerDefine(QsciScintilla::Circle, kBreakpointMarker);
    markerDefine(QsciScintilla::ThreeRightArrows, kErrorMarker);

    setAnnotationDisplay(QsciScintilla::AnnotationBoxed);

    //
    // Qscintilla signals/slots
    //
    connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)),
        this, SLOT(onMarginClicked(int,int,Qt::KeyboardModifiers)));
    connect(this, SIGNAL(linesChanged()), this, SLOT(onLinesChanged()));
}

JavaScriptTextEditor::~JavaScriptTextEditor()
{
    Q_D(JavaScriptTextEditor);
    delete d;
}

int JavaScriptTextEditor::addBreakpoint(int line)
{
    Q_D(JavaScriptTextEditor);
    d->addBreakpoint(line);
    return 0;
}

int JavaScriptTextEditor::clearBreakpoint(int line)
{
    Q_D(JavaScriptTextEditor);
    d->clearBreakpoint(line);
    return 0;
}

void JavaScriptTextEditor::displayError(const V8ScriptingEngine::V8Error &error)
{
    markerDeleteAll(kErrorMarker);
    markerAdd(error.line() - 1, kErrorMarker);

    clearAnnotations();
    annotate(error.line() - 1, error.message(), 1);
}

void JavaScriptTextEditor::onLinesChanged()
{
    QFontMetrics fontmetrics = QFontMetrics(font());
    setMarginWidth(kMarginLineNumbers, fontmetrics.width(QString::number(lines()) + 6));
}

void JavaScriptTextEditor::onMarginClicked(int /*margin*/, int line, Qt::KeyboardModifiers /*state*/)
{
    int mask = markersAtLine(line);
    if (mask & (1 << kErrorMarker)) {
        markerDelete(line, kErrorMarker);
        clearAnnotations(line);
        return;
    }

    if (mask & (1 << kBreakpointMarker)) {
        markerDelete(line, kBreakpointMarker);
        clearBreakpoint(line);
    }
    else {
        markerAdd(line, kBreakpointMarker);
        addBreakpoint(line);
    }
}

} // namespace editors
} // namespace widgets
} // namespace _9fold
