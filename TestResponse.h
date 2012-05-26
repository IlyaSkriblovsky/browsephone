#pragma once

#include "HttpResponse.h"

#include <QTimer>

class QTcpSocket;


class TestResponse: public HttpResponse
{
    Q_OBJECT

    public:
        ~TestResponse();

        virtual void abort();

    protected:
        void sendContent(QTcpSocket* socket);


    private slots:
        void onTimer();

    private:
        QTimer _timer;
        int _counter;

        QTcpSocket* _socket;
};
