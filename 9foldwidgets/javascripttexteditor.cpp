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

class JavaScriptTextEditorPrivate
{
public:
    JavaScriptTextEditorPrivate(V8ScriptingEngine *engine, JavaScriptTextEditor *parent)
        : q_ptr(parent), _debug(false), _engine(engine), _breakpointsSet(false)
    {

    }

    V8ScriptingEngine* engine() const
    {
        return _engine;
    }

    bool breakpointsSet() const
    {
        return _breakpointsSet;
    }

    void setBreakpointsSet(bool breakpointsSet)
    {
        _breakpointsSet = breakpointsSet;
    }

    void clearDebug()
    {
        _debug = false;
    }

    void setDebug()
    {
        _debug = true;
    }

    bool isDebug() const
    {
        return _debug;
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
    bool _debug;
    QVector<int> _breakpoints;
    V8ScriptingEngine *_engine;
    bool _breakpointsSet;
};

JavaScriptTextEditor::JavaScriptTextEditor(V8ScriptingEngine *engine, QWidget *parent)
    : TextEditor(parent), d_ptr(new JavaScriptTextEditorPrivate(engine, this))
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
    connect(engine, SIGNAL(breakOccurred()), this, SLOT(onBreakOccurred()));
    connect(engine, SIGNAL(exceptionOccurred()), this, SLOT(onExceptionOccurred()));
    connect(engine, SIGNAL(newFunctionOccurred()), this, SLOT(onNewFunctionOccurred()));
    connect(engine, SIGNAL(beforeCompileOccurred()), this, SLOT(onBeforeCompileOccurred()));
    connect(engine, SIGNAL(afterCompileOccurred()), this, SLOT(onAfterCompileOccurred()));
    connect(engine, SIGNAL(compileErrorOccurred()), this, SLOT(onCompileErrorOccurred()));
    connect(engine, SIGNAL(promiseEventOccurred()), this, SLOT(onPromiseEventOccurred()));
    connect(engine, SIGNAL(asyncTaskEventOccurred()), this, SLOT(onAsyncTaskEventOccurred()));

    connect(engine, SIGNAL(continueResponse(CommandResponse)),
        this, SLOT(onContinueResponse(CommandResponse)));
    connect(engine, SIGNAL(evaluateResponse(CommandResponse)),
        this, SLOT(onEvaluateResponse(CommandResponse)));
    connect(engine, SIGNAL(lookupResponse(CommandResponse)),
        this, SLOT(onLookupResponse(CommandResponse)));
    connect(engine, SIGNAL(backTraceResponse(CommandResponse)),
        this, SLOT(onBackTraceResponse(CommandResponse)));
    connect(engine, SIGNAL(frameResponse(CommandResponse)),
        this, SLOT(onFrameResponse(CommandResponse)));
    connect(engine, SIGNAL(scopeResponse(CommandResponse)),
        this, SLOT(onScopeResponse(CommandResponse)));
    connect(engine, SIGNAL(scopesResponse(CommandResponse)),
        this, SLOT(onScopesResponse(CommandResponse)));
    connect(engine, SIGNAL(scriptsResponse(CommandResponse)),
        this, SLOT(onScriptsResponse(CommandResponse)));
    connect(engine, SIGNAL(sourceResponse(CommandResponse)),
        this, SLOT(onSourceResponse(CommandResponse)));
    connect(engine, SIGNAL(setBreakpointResponse(CommandResponse)),
        this, SLOT(onSetBreakpointResponse(CommandResponse)));
    connect(engine, SIGNAL(changeBreakpointResponse(CommandResponse)),
        this, SLOT(onChangeBreakpointResponse(CommandResponse)));
    connect(engine, SIGNAL(clearBreakpointResponse(CommandResponse)),
        this, SLOT(onClearBreakpointResponse(CommandResponse)));
    connect(engine, SIGNAL(setExceptionBreakResponse(CommandResponse)),
        this, SLOT(onSetExceptionBreakResponse(CommandResponse)));
    connect(engine, SIGNAL(v8flagsResponse(CommandResponse)),
        this, SLOT(onV8flagsResponse(CommandResponse)));
    connect(engine, SIGNAL(versionResponse(CommandResponse)),
        this, SLOT(onVersionResponse(CommandResponse)));
    connect(engine, SIGNAL(gcResponse(CommandResponse)),
        this, SLOT(onGcResponse(CommandResponse)));
    connect(engine, SIGNAL(listBreakpointsResponse(CommandResponse)),
        this, SLOT(onListBreakpointsResponse(CommandResponse)));
    connect(engine, SIGNAL(setVariableValueResponse(CommandResponse)),
        this, SLOT(onSetVariableValueResponse(CommandResponse)));

    connect(engine, SIGNAL(finished(QString)), this, SLOT(onFinished(QString)));
    connect(engine, SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)), this,
        SLOT(onError(V8ScriptingEngine::V8Error)));
}

JavaScriptTextEditor::~JavaScriptTextEditor()
{
    Q_D(JavaScriptTextEditor);
    delete d;
}

V8ScriptingEngine *JavaScriptTextEditor::engine() const
{
    Q_D(const JavaScriptTextEditor);
    return d->engine();
}

int JavaScriptTextEditor::debug()
{
    Q_D(JavaScriptTextEditor);
    d->setDebug();
    engine()->debugAsync(text());
    return 0;
}

int JavaScriptTextEditor::run()
{
    engine()->runAsync(text());
    return 0;
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

void JavaScriptTextEditor::onBreakOccurred()
{
    Q_D(JavaScriptTextEditor);
    // The first break after compile
    if (d->breakpointsSet())
        return;

    d->setBreakpointsSet(true);
    debugInsertBreakpoints();

    ScriptingEngine::CommandRequest req;
    QJsonObject arguments;
    req["arguments"] = arguments;
    engine()->getScripts(req);

}

void JavaScriptTextEditor::onExceptionOccurred()
{

}

void JavaScriptTextEditor::onNewFunctionOccurred()
{

}

void JavaScriptTextEditor::onBeforeCompileOccurred()
{

}

void JavaScriptTextEditor::onAfterCompileOccurred()
{

}

void JavaScriptTextEditor::onCompileErrorOccurred()
{

}

void JavaScriptTextEditor::onPromiseEventOccurred()
{

}

void JavaScriptTextEditor::onAsyncTaskEventOccurred()
{

}

void JavaScriptTextEditor::onContinueResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onEvaluateResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onLookupResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onBackTraceResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onFrameResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onScopeResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onScopesResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onScriptsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onSourceResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onSetBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{
    Q_D(JavaScriptTextEditor);
}

void JavaScriptTextEditor::onChangeBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onClearBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onSetExceptionBreakResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onV8flagsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onVersionResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onGcResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onListBreakpointsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onSetVariableValueResponse(const ScriptingEngine::CommandResponse &response)
{

}

void JavaScriptTextEditor::onError(const V8ScriptingEngine::V8Error &error)
{
    markerDeleteAll(kErrorMarker);
    markerAdd(error.line() - 1, kErrorMarker);

    clearAnnotations();
    annotate(error.line() - 1, error.message(), 1);
}

void JavaScriptTextEditor::onFinished(const QString &/*result*/)
{
    Q_D(JavaScriptTextEditor);
    d->setBreakpointsSet(false);
    d->clearDebug();
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

void JavaScriptTextEditor::debugInsertBreakpoints()
{
    Q_D(JavaScriptTextEditor);
    for(int i = 0; i < d->breakpoints().size(); ++i) {
        V8ScriptingEngine::V8Breakpoint breakpoint(d->breakpoints()[i]);
        V8ScriptingEngine::CommandRequest request;
        request["arguments"] = breakpoint.toArguments();
        engine()->setBreakpoint(request);
    }
}

} // namespace editors
} // namespace widgets
} // namespace _9fold
