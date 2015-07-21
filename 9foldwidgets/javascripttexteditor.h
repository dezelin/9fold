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
    typedef V8ScriptingEngine::CommandRequest CommandRequest;
    typedef V8ScriptingEngine::CommandResponse CommandResponse;

    JavaScriptTextEditor(V8ScriptingEngine *engine, QWidget *parent = 0);
    virtual ~JavaScriptTextEditor();

    V8ScriptingEngine* engine() const;

    int debug();
    int run();

    int addBreakpoint(int line);
    int clearBreakpoint(int line);

signals:

private slots:

    void onBreakOccurred();
    void onExceptionOccurred();
    void onNewFunctionOccurred();
    void onBeforeCompileOccurred();
    void onAfterCompileOccurred();
    void onCompileErrorOccurred();
    void onPromiseEventOccurred();
    void onAsyncTaskEventOccurred();

    void onContinueResponse(const CommandResponse& response);
    void onEvaluateResponse(const CommandResponse& response);
    void onLookupResponse(const CommandResponse& response);
    void onBackTraceResponse(const CommandResponse& response);
    void onFrameResponse(const CommandResponse& response);
    void onScopeResponse(const CommandResponse& response);
    void onScopesResponse(const CommandResponse& response);
    void onScriptsResponse(const CommandResponse& response);
    void onSourceResponse(const CommandResponse& response);
    void onSetBreakpointResponse(const CommandResponse& response);
    void onChangeBreakpointResponse(const CommandResponse& response);
    void onClearBreakpointResponse(const CommandResponse& response);
    void onSetExceptionBreakResponse(const CommandResponse& response);
    void onV8flagsResponse(const CommandResponse& response);
    void onVersionResponse(const CommandResponse& response);
    void onGcResponse(const CommandResponse& response);
    void onListBreakpointsResponse(const CommandResponse& response);
    void onSetVariableValueResponse(const CommandResponse& response);

    void onError(const V8ScriptingEngine::V8Error& error);
    void onFinished(const QString& result);
    void onLinesChanged();
    void onMarginClicked(int margin, int line, Qt::KeyboardModifiers state);

private:
    void debugInsertBreakpoints();

private:
    JavaScriptTextEditorPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(JavaScriptTextEditor)
};

} // namespace editors
} // namespace widgets
} // namespace _9fold

#endif // JAVASCRIPTTEXTEDITOR_H
