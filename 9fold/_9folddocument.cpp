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

namespace _9fold
{
namespace documents
{

class _9FoldDocument::_9FoldDocumentPrivate
{
public:
    _9FoldDocumentPrivate(_9FoldDocument::Type type)
        : _type(type)
    {

    }

    _9FoldDocument::Type type() const
    {
        return _type;
    }

private:
    _9FoldDocument::Type _type;
};


_9FoldDocument::_9FoldDocument(const QString &name, Type type, QObject *parent)
    : Document(name, parent), _p(new _9FoldDocumentPrivate(type))
{

}

_9FoldDocument::~_9FoldDocument()
{

}

_9FoldDocument::Type _9FoldDocument::type() const
{
    return _p->type();
}

} // namespace documents
} // namespace _9fold


