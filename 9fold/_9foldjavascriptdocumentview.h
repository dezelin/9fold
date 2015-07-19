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

#ifndef _9FOLDJAVASCRIPTDOCUMENTVIEW_H
#define _9FOLDJAVASCRIPTDOCUMENTVIEW_H

#include "_9folddocumentview.h"

#include <javascripttexteditor.h>

#include <QObject>
#include <QScopedPointer>
#include <QWidget>

namespace _9fold
{
namespace views
{

using namespace _9fold::widgets::editors;

class _9FoldJavaScriptDocumentView : public _9FoldDocumentView
{
    Q_OBJECT
public:
    explicit _9FoldJavaScriptDocumentView(QWidget *parent = 0);
    virtual ~_9FoldJavaScriptDocumentView();

    JavaScriptTextEditor* editor() const;

    int debug();
    int run();

signals:

public slots:

private:
    class _9FoldJavaScriptDocumentViewPrivate;
    QScopedPointer<_9FoldJavaScriptDocumentViewPrivate> _p;
};

} // namespace views
} // namespace _9fold


#endif // _9FOLDJAVASCRIPTDOCUMENTVIEW_H
