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

#include "_9folddocumentmanager.h"
#include "_9foldjavascriptdocument.h"
#include "_9foldjavascriptdocumentview.h"

#include <QtDebug>
#include <QtGlobal>

namespace _9fold
{
namespace documents
{

const QString kTemplateJavaScriptDocumentName = QT_TR_NOOP("Script%1.js");

class _9FoldDocumentManager::_9FoldDocumentManagerPrivate
{
public:
    _9FoldDocumentManagerPrivate()
        : _counter(0)
    {

    }

    int counter() const
    {
        return _counter;
    }

    int incrementCounter()
    {
        return _counter++;
    }

    void decrementCounter()
    {
        _counter--;
    }

private:
    int _counter;
};

_9FoldDocumentManager::_9FoldDocumentManager(_9FoldDocumentViewManager *viewManager,
    QObject *parent)
    : DocumentManager(viewManager, parent), _p(new _9FoldDocumentManagerPrivate())
{

}

_9FoldDocumentManager::~_9FoldDocumentManager()
{

}

Document* _9FoldDocumentManager::createDocument(int documentType)
{
    QScopedPointer<_9FoldDocument> _doc;
    _9FoldDocument::Type type = static_cast<_9FoldDocument::Type>(documentType);
    switch (type)
    {
        case _9FoldDocument::Type::JAVASCRIPT:
        {
            QString name = kTemplateJavaScriptDocumentName.arg(_p->incrementCounter());
            _doc.reset(new _9FoldJavaScriptDocument(name, this));
            break;
        }
        default:
        {
            Q_ASSERT(!"Unknown document type.");
            break;
        }
    }

    return _doc.take();
}

_9FoldDocument* _9FoldDocumentManager::createNewJavaScriptDocument()
{
    QScopedPointer<_9FoldJavaScriptDocument> document(
        static_cast<_9FoldJavaScriptDocument*>(createDocument(static_cast<int>(
            _9FoldDocument::Type::JAVASCRIPT))));

    documentViewManager()->createView(document.data());
    return document.take();
}

} // namespace documents
} // namespace _9fold
