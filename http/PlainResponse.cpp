#include "PlainResponse.h"

#include <QTcpSocket>


using namespace http;


PlainResponse::PlainResponse()
    : _contentSet(false), _deferred(false), _deferredSocket(0)
{
}


void PlainResponse::setContent(const QByteArray& content)
{
    _contentSet = true;
    _content = content;

    if (_deferred)
        sendContent(_deferredSocket);
}


void PlainResponse::sendContent(QTcpSocket* socket)
{
    if (_contentSet)
    {
        socket->write(_content);
        finished();
    }
    else
    {
        _deferred = true;
        _deferredSocket = socket;
    }
}
