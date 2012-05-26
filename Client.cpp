#include "Client.h"

#include <QDebug>
#include <QTcpSocket>

#include "HttpRequest.h"
#include "HttpResponse.h"


Client::Client(int socket, QObject* parent)
    : QObject(parent), _socket(0), _request(0), _response(0)
{
    _request = new HttpRequest;

    _socket = new QTcpSocket(this);
    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    _socket->setSocketDescriptor(socket);
}

Client::~Client()
{
    delete _request;
}


void Client::onReadyRead()
{
    QByteArray data = _socket->readAll();

    _request->feed(data);
    if (_request->ready())
        requestReady();
}

void Client::onDisconnected()
{
    if (_response)
    {
        _response->abort();
        delete _response;
        _response = 0;
    }

    disconnected();
}


void Client::response(HttpResponse* response)
{
    _response = response;
    _response->setParent(this);

    connect(_response, SIGNAL(finished()), this, SLOT(onResponseFinished()));
    response->send(_socket);
}


void Client::onResponseFinished()
{
    _socket->disconnectFromHost();
}
