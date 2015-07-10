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

#ifndef _9FOLDTESTRUNNER_H
#define _9FOLDTESTRUNNER_H

#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QtTest>

class TestRunner : public QObject
{
    Q_OBJECT

public:
    TestRunner()
        : m_overallResult(0)
    {

    }

    void addTest(QObject *test)
    {
        test->setParent(this);
        m_tests.append(test);
    }

    bool runTests()
    {
        int argc = 0;
        char *argv[] = {0};
        QCoreApplication app(argc, argv);
        QTimer::singleShot(0, this, SLOT(run()));
        app.exec();

        return m_overallResult == 0;
    }

private slots:
    void run()
    {
        doRunTests();
        QCoreApplication::instance()->quit();
    }

private:
    void doRunTests()
    {
        foreach (QObject *test, m_tests) {
            m_overallResult |= QTest::qExec(test);
        }
    }

    int m_overallResult;
    QList<QObject *> m_tests;
};

#endif  // _9FOLDTESTRUNNER_H
