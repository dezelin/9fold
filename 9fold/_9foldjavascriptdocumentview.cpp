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

#include "_9foldjavascriptdocumentview.h"

#include <javascripttexteditor.h>
#include <v8scriptingengine.h>

#include <QScopedPointer>
#include <QVBoxLayout>

namespace _9fold
{
namespace views
{

using namespace _9fold::widgets::editors;

class _9FoldJavaScriptDocumentViewPrivate
{
public:
    _9FoldJavaScriptDocumentViewPrivate(V8ScriptingEngine *engine,
        JavaScriptTextEditor *editor, _9FoldJavaScriptDocumentView *parent)
        : q_ptr(parent), _editor(editor), _engine(engine)
    {

    }

    JavaScriptTextEditor* editor() const
    {
        return _editor;
    }

    void setEditor(JavaScriptTextEditor* editor)
    {
        _editor = editor;
    }

    V8ScriptingEngine* engine() const
    {
        return _engine.data();
    }

private:
    _9FoldJavaScriptDocumentView* const q_ptr;
    Q_DECLARE_PUBLIC(_9FoldJavaScriptDocumentView)

    JavaScriptTextEditor *_editor;
    QScopedPointer<V8ScriptingEngine> _engine;
};

_9FoldJavaScriptDocumentView::_9FoldJavaScriptDocumentView(QWidget *parent)
    : _9FoldDocumentView(parent)
    , d_ptr(new _9FoldJavaScriptDocumentViewPrivate(new V8ScriptingEngine(this),
        new JavaScriptTextEditor(this), this))
{
    Q_D(_9FoldJavaScriptDocumentView);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(d->editor());
    layout->setContentsMargins(0, 0, 0, 0);

    connect(d->engine(), SIGNAL(breakOccurred()), this, SLOT(onBreakOccurred()));
    connect(d->engine(), SIGNAL(exceptionOccurred()), this, SLOT(onExceptionOccurred()));
    connect(d->engine(), SIGNAL(newFunctionOccurred()), this, SLOT(onNewFunctionOccurred()));
    connect(d->engine(), SIGNAL(beforeCompileOccurred()), this, SLOT(onBeforeCompileOccurred()));
    connect(d->engine(), SIGNAL(afterCompileOccurred()), this, SLOT(onAfterCompileOccurred()));
    connect(d->engine(), SIGNAL(compileErrorOccurred()), this, SLOT(onCompileErrorOccurred()));
    connect(d->engine(), SIGNAL(promiseEventOccurred()), this, SLOT(onPromiseEventOccurred()));
    connect(d->engine(), SIGNAL(asyncTaskEventOccurred()), this, SLOT(onAsyncTaskEventOccurred()));

    connect(d->engine(), SIGNAL(continueResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onContinueResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(evaluateResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onEvaluateResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(lookupResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onLookupResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(backTraceResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onBackTraceResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(frameResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onFrameResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(scopeResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScopeResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(scopesResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScopesResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(scriptsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onScriptsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(sourceResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSourceResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(setBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(changeBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onChangeBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(clearBreakpointResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onClearBreakpointResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(setExceptionBreakResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetExceptionBreakResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(v8flagsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onV8flagsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(versionResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onVersionResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(gcResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onGcResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(listBreakpointsResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onListBreakpointsResponse(V8ScriptingEngine::CommandResponse)));
    connect(d->engine(), SIGNAL(setVariableValueResponse(V8ScriptingEngine::CommandResponse)),
        this, SLOT(onSetVariableValueResponse(V8ScriptingEngine::CommandResponse)));

    connect(d->engine(), SIGNAL(errorOccurred(V8ScriptingEngine::V8Error)),
            this, SLOT(onError(V8ScriptingEngine::V8Error)));
    connect(d->engine(), SIGNAL(finished(QString)), this, SLOT(onFinished(QString)));
}

_9FoldJavaScriptDocumentView::~_9FoldJavaScriptDocumentView()
{
    Q_D(_9FoldJavaScriptDocumentView);
    delete d;
}

JavaScriptTextEditor *_9FoldJavaScriptDocumentView::editor() const
{
    Q_D(const _9FoldJavaScriptDocumentView);
    return d->editor();
}

V8ScriptingEngine *_9FoldJavaScriptDocumentView::engine() const
{
    Q_D(const _9FoldJavaScriptDocumentView);
    return d->engine();
}

int _9FoldJavaScriptDocumentView::debug()
{
    Q_D(_9FoldJavaScriptDocumentView);
    d->engine()->debugAsync(d->editor()->text());
    return 0;
}

int _9FoldJavaScriptDocumentView::run()
{
    Q_D(_9FoldJavaScriptDocumentView);
    d->engine()->runAsync(d->editor()->text());
    return 0;
}

void _9FoldJavaScriptDocumentView::onBreakOccurred()
{

}

void _9FoldJavaScriptDocumentView::onExceptionOccurred()
{

}

void _9FoldJavaScriptDocumentView::onNewFunctionOccurred()
{

}

void _9FoldJavaScriptDocumentView::onBeforeCompileOccurred()
{

}

void _9FoldJavaScriptDocumentView::onAfterCompileOccurred()
{

}

void _9FoldJavaScriptDocumentView::onCompileErrorOccurred()
{

}

void _9FoldJavaScriptDocumentView::onPromiseEventOccurred()
{

}

void _9FoldJavaScriptDocumentView::onAsyncTaskEventOccurred()
{

}

void _9FoldJavaScriptDocumentView::onContinueResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onEvaluateResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onLookupResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onBackTraceResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onFrameResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onScopeResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onScopesResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onScriptsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onSourceResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onSetBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onChangeBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onClearBreakpointResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onSetExceptionBreakResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onV8flagsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onVersionResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onGcResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onListBreakpointsResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onSetVariableValueResponse(const ScriptingEngine::CommandResponse &response)
{

}

void _9FoldJavaScriptDocumentView::onError(const V8ScriptingEngine::V8Error &error)
{
    Q_D(_9FoldJavaScriptDocumentView);
    d->editor()->displayError(error);
}

void _9FoldJavaScriptDocumentView::onFinished(const QString &result)
{

}

} // namespace views
} // namespace _9fold
