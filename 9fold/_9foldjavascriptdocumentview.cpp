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

#include <QVBoxLayout>

namespace _9fold
{
namespace views
{

using namespace _9fold::widgets::editors;

class _9FoldJavaScriptDocumentView::_9FoldJavaScriptDocumentViewPrivate
{
public:
    _9FoldJavaScriptDocumentViewPrivate(JavaScriptTextEditor *editor = 0)
        : _editor(editor)
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

private:
    JavaScriptTextEditor *_editor;
};

_9FoldJavaScriptDocumentView::_9FoldJavaScriptDocumentView(QWidget *parent)
    : _9FoldDocumentView(parent), _p(new _9FoldJavaScriptDocumentViewPrivate())
{
    QScopedPointer<JavaScriptTextEditor> editor(new JavaScriptTextEditor(
        new V8ScriptingEngine(this), this));
    _p->setEditor(editor.data());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(editor.take());
    layout->setContentsMargins(0, 0, 0, 0);

}

_9FoldJavaScriptDocumentView::~_9FoldJavaScriptDocumentView()
{

}

JavaScriptTextEditor *_9FoldJavaScriptDocumentView::editor() const
{
    return _p->editor();
}

int _9FoldJavaScriptDocumentView::debug()
{
    return editor()->debug();
}

int _9FoldJavaScriptDocumentView::run()
{
    return editor()->run();
}

} // namespace views
} // namespace _9fold
