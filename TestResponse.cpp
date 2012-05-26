#include "TestResponse.h"

#include <QTcpSocket>


TestResponse::~TestResponse()
{
    qDebug() << "~TestResponse";
}


void TestResponse::sendContent(QTcpSocket* socket)
{
    _counter = 10;
    _socket = socket;

    connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer.start(500);
}


void TestResponse::onTimer()
{
    _socket->write("hello\n");
    _socket->flush();

    if (--_counter == 0)
    {
        _timer.stop();
        finished();
    }
}

void TestResponse::abort()
{
    _timer.stop();
}
