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

#include <QtDebug>

namespace _9fold
{
namespace documents
{

class _9FoldDocumentManager::_9FoldDocumentManagerPrivate
{
public:
    _9FoldDocumentManagerPrivate()
    {

    }

private:
};

_9FoldDocumentManager::_9FoldDocumentManager(DocumentViewManager *viewManager,
    QObject *parent)
    : DocumentManager(viewManager, parent), _p(new _9FoldDocumentManagerPrivate())
{

}

_9FoldDocumentManager::~_9FoldDocumentManager()
{

}

Document* _9FoldDocumentManager::createDocument()
{
    return 0;
}

DocumentView* _9FoldDocumentManager::createDocumentView(Document *document)
{
    Q_ASSERT(document);
    if (!document)
        return 0;

    return 0;
}

} // namespace documents
} // namespace _9fold
