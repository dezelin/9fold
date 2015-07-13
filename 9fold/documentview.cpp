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

#include "documentview.h"

namespace _9fold
{
namespace views
{

//
// Private class
//
class DocumentView::DocumentViewPrivate
{
public:
    DocumentViewPrivate()
        : _presenter(0)
    {

    }

    void attach(DocumentPresenter *presenter)
    {
        _presenter = presenter;
    }

    void detach()
    {
        _presenter = 0;
    }

    DocumentPresenter* presenter() const
    {
        return _presenter;
    }

private:
    DocumentPresenter *_presenter;
};

DocumentView::DocumentView(QWidget *parent)
    : QWidget(parent), _p(new DocumentViewPrivate())
{

}

DocumentView::~DocumentView()
{

}

void DocumentView::attach(DocumentPresenter *presenter)
{
    _p->attach(presenter);
}

void DocumentView::detach()
{
    _p->detach();
}

DocumentPresenter* DocumentView::presenter() const
{
    return _p->presenter();
}

} // namespace views
} // namespace _9fold


