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

#ifndef DOCUMENTPRESENTER_H
#define DOCUMENTPRESENTER_H

#include <QObject>
#include <QScopedPointer>

namespace _9fold
{
namespace documents
{

class Document;
class DocumentView;

class DocumentPresenter : public QObject
{
    Q_OBJECT
public:
    explicit DocumentPresenter(Document *document, DocumentView *view,
        QObject *parent = 0);
    virtual ~DocumentPresenter();

    Document* document() const;
    DocumentView* view() const;

signals:

public slots:

private:
    class DocumentPresenterPrivate;
    QScopedPointer<DocumentPresenterPrivate> _p;
};

} // namespace documents
} // namespace _9fold

#endif // DOCUMENTPRESENTER_H
