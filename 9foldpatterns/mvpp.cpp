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

#include "mvpp.h"

#include <algorithm>
#include <assert.h>

namespace _9fold
{
namespace patterns
{
namespace mvpp
{

//
// Class Presenter::PresenterPrivate
//

class Presenter::PresenterPrivate
{
public:
    PresenterPrivate(Model *model, View *view);
    PresenterPrivate(const PresenterPrivate& pp);

    Model* model() const;
    View* view() const;

private:
    Model *_model;
    View *_view;
};

Presenter::PresenterPrivate::PresenterPrivate(Model* model, View *view)
    : _model(model), _view(view)
{

}

Presenter::PresenterPrivate::PresenterPrivate(const PresenterPrivate& pp)
{
    _model = pp.model();
    _view = pp.view();
}

Model* Presenter::PresenterPrivate::model() const
{
    return _model;
}

View* Presenter::PresenterPrivate::view() const
{
    return _view;
}

//
// Class Presenter
//

Presenter::Presenter(Model *model, View *view)
    : _p(new PresenterPrivate(model, view))
{
}

Presenter::~Presenter()
{

}

Presenter::Presenter(const Presenter& presenter)
{
    assert(presenter._p);
    _p.reset(new PresenterPrivate(*presenter._p));
}

Presenter::Presenter(Presenter&& presenter)
    : Presenter()
{
    std::swap(*this, presenter);
}

Presenter& Presenter::operator=(Presenter presenter)
{
    std::swap(*this, presenter);
    return *this;
}

void Presenter::swap(Presenter& presenter)
{
    std::swap(_p, presenter._p);
}

Model* Presenter::model() const
{
    assert(_p);
    return _p->model();
}

View* Presenter::view() const
{
    assert(_p);
    return _p->view();
}

//
// Class Model
//

Model::Model()
{

}

Model::~Model()
{

}

//
// Class View::ViewPrivate
//

class View::ViewPrivate
{
public:
    ViewPrivate(Presenter *presenter);
    ViewPrivate(const ViewPrivate& vp);

    Presenter* presenter() const;

private:
    Presenter *_presenter;
};

View::ViewPrivate::ViewPrivate(Presenter *presenter)
    : _presenter(presenter)
{

}

View::ViewPrivate::ViewPrivate(const ViewPrivate& vp)
{
    _presenter = vp.presenter();
}

Presenter* View::ViewPrivate::presenter() const
{
    return _presenter;
}

//
// Class View
//

View::View(Presenter *presenter)
    : _p(new ViewPrivate(presenter))
{

}

View::~View()
{

}

View::View(const View& view)
{
    assert(view._p);
    _p.reset(new ViewPrivate(*view._p));
}

View::View(View&& view)
    : View()
{
    std::swap(*this, view);
}

View& View::operator=(View view)
{
    std::swap(*this, view);
    return *this;
}

void View::swap(View& view)
{
    std::swap(_p, view._p);
}

Presenter* View::presenter() const
{
    assert(_p);
    return _p->presenter();
}

} // namespace mvpp
} // namspace patterns
} // namespace _9fold
