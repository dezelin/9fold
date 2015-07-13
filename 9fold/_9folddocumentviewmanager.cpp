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

#include "_9folddocumentview.h"
#include "_9folddocumentviewmanager.h"
#include "_9folddocumentpresenter.h"

#include "_9foldjavascriptdocument.h"
#include "_9foldjavascriptdocumentpresenter.h"
#include "_9foldjavascriptdocumentview.h"


namespace _9fold
{
namespace views
{

_9FoldDocumentViewManager::_9FoldDocumentViewManager(QMainWindow *mainWindow, QObject *parent)
    : DocumentViewManager(mainWindow, parent)
{

}

_9FoldDocumentViewManager::~_9FoldDocumentViewManager()
{

}

DocumentView* _9FoldDocumentViewManager::createView(Document *document)
{
    Q_ASSERT(document);
    if (!document)
        return 0;

    QScopedPointer<_9FoldDocumentView> _view;
    QScopedPointer<_9FoldDocumentPresenter> _presenter;
    _9FoldDocument *_9foldDoc = static_cast<_9FoldDocument*>(document);

    switch (_9foldDoc->type())
    {
        case _9FoldDocument::Type::JAVASCRIPT:
        {
            _view.reset(new _9FoldJavaScriptDocumentView(mainWindow()));
            _presenter.reset(new _9FoldJavaScriptDocumentPresenter(
                static_cast<_9FoldJavaScriptDocument*>(_9foldDoc),
                    static_cast<_9FoldJavaScriptDocumentView*>(_view.data()),
                        _9foldDoc));
            break;
        }
        default:
        {
            Q_ASSERT(!"Unknown document type.");
            break;
        }
    }

    _view->attach(_presenter.data());
    document->attach(_presenter.take());

    return _view.take();
}

} // namespace views
} // namespace _9fold


