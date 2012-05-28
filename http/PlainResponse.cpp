#include "PlainResponse.h"

#include <QTcpSocket>


using namespace http;


PlainResponse::PlainResponse()
{
}


void PlainResponse::setContent(const QByteArray& content)
{
    _content = content;
}


void PlainResponse::sendContent(QTcpSocket* socket)
{
    socket->write(_content);
    finished();
}
