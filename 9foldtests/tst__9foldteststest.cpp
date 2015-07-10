#include <QString>
#include <QtTest>

class _9foldtestsTest : public QObject
{
    Q_OBJECT

public:
    _9foldtestsTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

_9foldtestsTest::_9foldtestsTest()
{
}

void _9foldtestsTest::initTestCase()
{
}

void _9foldtestsTest::cleanupTestCase()
{
}

void _9foldtestsTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(_9foldtestsTest)

#include "tst__9foldteststest.moc"
