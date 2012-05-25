#include "HttpResponse.h"

#include <QTcpSocket>


HttpResponse::HttpResponse()
{
}

HttpResponse::~HttpResponse()
{
}


void HttpResponse::send(QTcpSocket* socket)
{
    socket->write(
        QString("HTTP/1.1 %1 %2\r\n")
            .arg(_code)
            .arg(_phrase)
            .toUtf8()
    );

    foreach (QString key, _headers.keys())
    {
        socket->write(key.toUtf8());
        socket->write(": ");
        socket->write(_headers.value(key).toUtf8());
        socket->write("\r\n");
    }

    socket->write("\r\n");

    sendContent(socket);

    socket->disconnectFromHost();
}
