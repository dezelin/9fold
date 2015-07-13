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

namespace _9fold
{
namespace views
{

class _9FoldDocumentView::_9FoldDocumentViewPrivate
{
public:
    _9FoldDocumentViewPrivate()
    {

    }

private:
};

_9FoldDocumentView::_9FoldDocumentView(QWidget *parent)
    : DocumentView(parent), _p(new _9FoldDocumentViewPrivate())
{

}

_9FoldDocumentView::~_9FoldDocumentView()
{

}

} // namespace views
} // namespace _9fold
