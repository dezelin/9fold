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

#ifndef _9FOLDJAVASCRIPTDOCUMENTPRESENTER_H
#define _9FOLDJAVASCRIPTDOCUMENTPRESENTER_H

#include "_9foldjavascriptdocument.h"
#include "_9folddocumentpresenter.h"
#include "_9foldjavascriptdocumentview.h"

#include <QObject>

namespace _9fold
{
namespace documents
{

class _9FoldJavaScriptDocumentPresenter : public _9FoldDocumentPresenter
{
    Q_OBJECT
public:
    explicit _9FoldJavaScriptDocumentPresenter(_9FoldJavaScriptDocument *document,
        _9FoldJavaScriptDocumentView *view, QObject *parent = 0);
    virtual ~_9FoldJavaScriptDocumentPresenter();

signals:

public slots:
};

} // namespace documents
} // namespace _9fold

#endif // _9FOLDJAVASCRIPTDOCUMENTPRESENTER_H
