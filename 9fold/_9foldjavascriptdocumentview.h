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

#ifndef _9FOLDJAVASCRIPTDOCUMENTVIEW_H
#define _9FOLDJAVASCRIPTDOCUMENTVIEW_H

#include "_9folddocumentview.h"

#include <javascripttexteditor.h>
#include <v8scriptingengine.h>

#include <QObject>
#include <QWidget>

namespace _9fold
{
namespace views
{

using namespace _9fold::widgets::editors;

class _9FoldJavaScriptDocumentViewPrivate;
class _9FoldJavaScriptDocumentView : public _9FoldDocumentView
{
    Q_OBJECT
public:
    explicit _9FoldJavaScriptDocumentView(QWidget *parent = 0);
    virtual ~_9FoldJavaScriptDocumentView();

    JavaScriptTextEditor* editor() const;
    V8ScriptingEngine* engine() const;

    int debug();
    int run();

signals:

public slots:

    // ScriptingEngine slots

    void onBreakOccurred();
    void onExceptionOccurred();
    void onNewFunctionOccurred();
    void onBeforeCompileOccurred();
    void onAfterCompileOccurred();
    void onCompileErrorOccurred();
    void onPromiseEventOccurred();
    void onAsyncTaskEventOccurred();

    void onContinueResponse(const V8ScriptingEngine::CommandResponse& response);
    void onEvaluateResponse(const V8ScriptingEngine::CommandResponse& response);
    void onLookupResponse(const V8ScriptingEngine::CommandResponse& response);
    void onBackTraceResponse(const V8ScriptingEngine::CommandResponse& response);
    void onFrameResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScopeResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScopesResponse(const V8ScriptingEngine::CommandResponse& response);
    void onScriptsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSourceResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onChangeBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onClearBreakpointResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetExceptionBreakResponse(const V8ScriptingEngine::CommandResponse& response);
    void onV8flagsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onVersionResponse(const V8ScriptingEngine::CommandResponse& response);
    void onGcResponse(const V8ScriptingEngine::CommandResponse& response);
    void onListBreakpointsResponse(const V8ScriptingEngine::CommandResponse& response);
    void onSetVariableValueResponse(const V8ScriptingEngine::CommandResponse& response);

    void onError(const V8ScriptingEngine::V8Error &error);
    void onFinished(const QString& result);

    // TextEditor slots

    void onAddBreakpoint(int line);
    void onClearBreakpoint(int line);

private:
    _9FoldJavaScriptDocumentViewPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(_9FoldJavaScriptDocumentView)
};

} // namespace views
} // namespace _9fold


#endif // _9FOLDJAVASCRIPTDOCUMENTVIEW_H
