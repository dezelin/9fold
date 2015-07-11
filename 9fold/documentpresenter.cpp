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

#include "document.h"
#include "documentview.h"
#include "documentpresenter.h"

#include <QtGlobal>


namespace _9fold
{
namespace documents
{

//
// Private class
//
class DocumentPresenter::DocumentPresenterPrivate
{
public:
    DocumentPresenterPrivate(Document *document, DocumentView *view)
        : _document(document), _view(view)
    {

    }

    Document* document() const
    {
        return _document;
    }

    DocumentView* view() const
    {
        return _view;
    }

private:
    Document *_document;
    DocumentView *_view;
};

DocumentPresenter::DocumentPresenter(Document *document, DocumentView *view,
    QObject *parent) : QObject(parent), _p(new DocumentPresenterPrivate(document, view))
{
    Q_ASSERT(document);
    Q_ASSERT(view);

    document->attach(this);
    view->attach(this);
}

DocumentPresenter::~DocumentPresenter()
{

}

Document* DocumentPresenter::document() const
{
    return _p->document();
}

DocumentView* DocumentPresenter::view() const
{
    return _p->view();
}

} // namespace documents
} // namespace _9fold
