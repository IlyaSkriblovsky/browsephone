#include "IODeviceResponse.h"

#include <QDebug>
#include <QIODevice>
#include <QTcpSocket>


using namespace http;


#define BLOCK_SIZE      (100 * 1024)


IODeviceResponse::IODeviceResponse(QIODevice* io)
    : _io(io)
{
    _io->setParent(this);

    if (_io->size() > 0)
        headers().insert("Content-Length", QString::number(_io->size()));
}


IODeviceResponse::~IODeviceResponse()
{
    close();
}


void IODeviceResponse::close()
{
    if (_io)
    {
        _io->close();
        delete _io;
        _io = 0;
    }
}


void IODeviceResponse::sendContent(QTcpSocket* socket)
{
    _socket = socket;

    connect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    sendChunk(BLOCK_SIZE);
}


void IODeviceResponse::onBytesWritten(qint64 count)
{
    Q_UNUSED(count)

    if (_io && _socket->bytesToWrite() == 0)
        sendChunk(BLOCK_SIZE);
}


void IODeviceResponse::abort()
{
    close();
}


void IODeviceResponse::sendChunk(int size)
{
    QByteArray bytes = _io->read(size);

    if (bytes.size() > 0)
        _socket->write(bytes);

    if (_io->atEnd())
    {
        close();

        finished();
    }
}
