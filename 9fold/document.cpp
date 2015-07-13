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
#include "documentpresenter.h"

#include <QList>

namespace _9fold
{
namespace documents
{

class Document::DocumentPrivate
{
public:
    DocumentPrivate(const QString& name)
        : _name(name)
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

    DocumentPresenter* firstPresenter() const
    {
        if (_presenters.size() < 1)
            return 0;

        return _presenters.at(0);
    }

    const QList<DocumentPresenter*>& presenters() const
    {
        return _presenters;
    }

    const QString& name() const
    {
        return _name;
    }

private:
    QString _name;
    QList<DocumentPresenter*> _presenters;
};

Document::Document(const QString &name, QObject *parent)
    : QObject(parent), _p(new DocumentPrivate(name))
{

}

Document::~Document()
{

}

void Document::attach(DocumentPresenter *presenter)
{
    Q_ASSERT(presenter);
    if (!presenter)
        return;

    _p->attach(presenter);
}

void Document::detach(DocumentPresenter *presenter)
{
    Q_ASSERT(presenter);
    if (!presenter)
        return;

    _p->detach(presenter);
}

DocumentPresenter* Document::firstPresenter() const
{
    return _p->firstPresenter();
}

const QList<DocumentPresenter*>& Document::presenters() const
{
    return _p->presenters();
}

const QString& Document::name() const
{
    return _p->name();
}

} // namespace documents
} // namespace _9fold

