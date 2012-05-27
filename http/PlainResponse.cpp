#include "PlainResponse.h"

#include <QTcpSocket>


using namespace http;


PlainResponse::PlainResponse()
{
}


void PlainResponse::sendContent(QTcpSocket* socket)
{
    socket->write(_content);
    finished();
}
