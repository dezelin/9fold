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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace _9fold
{
namespace ui
{

class MainWindow::MainWindowPrivate
{
public:
    MainWindowPrivate(QMainWindow* mainWindow, Ui::MainWindow *ui)
        : _mainWindow(mainWindow), _ui(ui)
    {
        Q_ASSERT(_mainWindow);
        Q_ASSERT(_ui);
        _ui->setupUi(_mainWindow);
    }

    QMainWindow* mainWindow() const
    {
        return _mainWindow;
    }

    Ui::MainWindow* ui()
    {
        return _ui.data();
    }

private:
    QMainWindow *_mainWindow;
    QScopedPointer<Ui::MainWindow> _ui;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _p(new MainWindowPrivate(this, new Ui::MainWindow()))
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _p->ui()->retranslateUi(this);
        break;
    default:
        break;
    }
}

} // namespace ui
} // namespace _9fold
