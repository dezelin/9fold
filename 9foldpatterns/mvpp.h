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

#ifndef MVPP_H
#define MVPP_H

#include "observer.h"

#include <list>
#include <memory>

namespace _9fold
{
namespace patterns
{
namespace mvpp
{

//
// Model - Passive view - Presenter design pattern
//

class Model;
class View;

class Presenter : Observer::Listener
{
public:
    Presenter(Model *model = nullptr, View *view = nullptr);
    virtual ~Presenter();

    Presenter(const Presenter& presenter);
    Presenter(Presenter&& presenter);
    Presenter& operator=(Presenter presenter);

    void swap(Presenter& presenter);

    Model* model() const;
    View* view() const;

private:
    class PresenterPrivate;
    std::unique_ptr<PresenterPrivate> _p;
};

class Model : Observer
{
public:
    Model();
    virtual ~Model();
};

class View
{
public:
    View(Presenter *presenter = nullptr);
    virtual ~View();

    View(const View& view);
    View(View&& view);
    View& operator=(View view);

    void swap(View& view);

    Presenter* presenter() const;

private:
    class ViewPrivate;
    std::unique_ptr<ViewPrivate> _p;
};

} // namespace mvpp
} // namspace patterns
} // namespace _9fold

#endif // MVPP_H
