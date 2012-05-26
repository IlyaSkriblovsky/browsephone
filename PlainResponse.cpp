#include "PlainResponse.h"

#include <QTcpSocket>

PlainResponse::PlainResponse()
{
}


void PlainResponse::sendContent(QTcpSocket* socket)
{
    socket->write(_content);
    finished();
}
