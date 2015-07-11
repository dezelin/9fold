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

#include "_9folddocument.h"

#include <QList>
#include <QtGlobal>

namespace _9fold
{
namespace documents
{

class _9FoldDocument::_9FoldDocumentPrivate
{
public:
    _9FoldDocumentPrivate()
    {

    }

    void attach(DocumentPresenter *presenter)
    {
        if (!_presenters.contains(presenter))
            _presenters.append(presenter);
    }

    void detach(DocumentPresenter *presenter)
    {
        _presenters.removeAll(presenter);
    }

private:
    QList<DocumentPresenter*> _presenters;
};


_9FoldDocument::_9FoldDocument(QObject *parent)
    : Document(parent), _p(new _9FoldDocumentPrivate())
{

}

_9FoldDocument::~_9FoldDocument()
{

}

void _9FoldDocument::attach(DocumentPresenter *presenter)
{
    Q_ASSERT(presenter);
    if (!presenter)
        return;

    _p->attach(presenter);
}

void _9FoldDocument::detach(DocumentPresenter *presenter)
{
    Q_ASSERT(presenter);
    if (!presenter)
        return;

    _p->detach(presenter);
}

} // namespace documents
} // namespace _9fold


