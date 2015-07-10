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

#include "observertest.h"

#include <observer.h>

#include <QTest>

#include <algorithm>
#include <memory>
#include <vector>

class B : public _9fold::patterns::Observer::Listener
{
public:
    B() : _i(-1) {}

    void inbox(int i)
    {
        _i = i;
    }

    int i() const
    {
        return _i;
    }

private:
    int _i;
};

class A : public _9fold::patterns::Observer
{
public:
    A() {}

    void notify(int i)
    {
        for(Listener *listener : listeners()) {
            B *b = static_cast<B*>(listener);
            b->inbox(i);
        }
    }
};


ObserverTest::ObserverTest(QObject *parent) : QObject(parent)
{

}

void ObserverTest::initTestCase()
{
}

void ObserverTest::cleanupTestCase()
{
}

void ObserverTest::testCaseObserverAttach()
{
    A a;
    std::vector<std::shared_ptr<B>> listeners;
    for(int i = 0; i < 1000; ++i) {
        std::shared_ptr<B> listener(new B());
        listeners.push_back(listener);
        a.attach(listener.get());
    }

    QVERIFY(a.listeners().size() == 1000);

    for(std::shared_ptr<B> b : listeners) {
        auto it = std::find(a.listeners().begin(), a.listeners().end(), b.get());
        QVERIFY(it != a.listeners().end());
    }
}

void ObserverTest::testCaseObserverDettach()
{
    A a;
    std::vector<std::shared_ptr<B>> listeners;
    for(int i = 0; i < 1000; ++i) {
        std::shared_ptr<B> listener(new B());
        listeners.push_back(listener);
        a.attach(listener.get());
    }

    QVERIFY(a.listeners().size() == 1000);

    for(std::shared_ptr<B> b : listeners) {
        a.detach(b.get());
    }

    QVERIFY(a.listeners().size() == 0);
}

void ObserverTest::testCaseObserverNotify()
{
    A a;
    std::vector<std::shared_ptr<B>> listeners;
    for(int i = 0; i < 1000; ++i) {
        std::shared_ptr<B> listener(new B());
        listeners.push_back(listener);
        a.attach(listener.get());
    }

    // Test notify() hundred times
    for(int i = 0; i < 100; ++i) {
        a.notify(i);
        std::for_each(listeners.begin(), listeners.end(), [&](std::shared_ptr<B> l) {
            QVERIFY(l->i() == i);
        });
    }
}

