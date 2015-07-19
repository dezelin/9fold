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

class JavaScriptTextEditor::JavaScriptTextEditorPrivate
{
public:
    JavaScriptTextEditorPrivate(V8ScriptingEngine *engine)
        : _engine(engine)
    {

    }

    V8ScriptingEngine* engine() const
    {
        return _engine;
    }

private:
    V8ScriptingEngine *_engine;
};

JavaScriptTextEditor::JavaScriptTextEditor(V8ScriptingEngine *engine, QWidget *parent)
    : TextEditor(parent), _p(new JavaScriptTextEditorPrivate(engine))
{
    setLexer(new QsciLexerJavaScript(this));
    setWrapMode(QsciScintilla::WrapCharacter);

    QFontMetrics fontmetrics = QFontMetrics(font());
    setMarginsFont(font());
    setMarginsBackgroundColor(paper());

    setMarginWidth(kMarginLineNumbers, fontmetrics.width(QString::number(lines()) + 6));
    setMarginLineNumbers(kMarginLineNumbers, true);

    setMarginWidth(kMarginSymbols, fontmetrics.width(QString::number(0)));
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

    //
    // V8ScriptingEngine signals/slots
    //
    connect(engine, SIGNAL(finished(QString)), this, SLOT(onFinished(QString)));
    connect(engine, SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), this,
        SLOT(onError(V8ScriptingEngine::V8Error)));
}

JavaScriptTextEditor::~JavaScriptTextEditor()
{

}

V8ScriptingEngine *JavaScriptTextEditor::engine() const
{
    return _p->engine();
}

int JavaScriptTextEditor::debug()
{
    engine()->initializeDebugging();
    return run();
}

int JavaScriptTextEditor::run()
{
    _p->engine()->runAsync(text());
    return 0;
}

int JavaScriptTextEditor::addBreakpoint(int line)
{
    V8ScriptingEngine::V8Breakpoint breakpoint(line);
    V8ScriptingEngine::CommandRequest request;
    request["arguments"] = breakpoint.toArguments();
    return _p->engine()->setBreakpoint(request);
}

int JavaScriptTextEditor::clearBreakpoint(int /*line*/)
{
    return 0;
}

void JavaScriptTextEditor::onError(const V8ScriptingEngine::V8Error &error)
{
    markerDeleteAll(kErrorMarker);
    markerAdd(error.line() - 1, kErrorMarker);

    clearAnnotations();
    annotate(error.line() - 1, error.message(), 1);
}

void JavaScriptTextEditor::onFinished(const QString &result)
{

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
