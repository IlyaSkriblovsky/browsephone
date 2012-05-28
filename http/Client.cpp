#include "Client.h"

#include <QDebug>
#include <QTcpSocket>

#include "Request.h"
#include "ResponsePromise.h"
#include "Response.h"


using namespace http;


Client::Client(int socket, QObject* parent)
    : QObject(parent), _socket(0), _request(0), _promise(0)
{
    _request = new Request;

    _socket = new QTcpSocket(this);
    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    _socket->setSocketDescriptor(socket);
}

Client::~Client()
{
    delete _promise;
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
    disconnected();
}


void Client::promise(ResponsePromise* promise)
{
    _promise = promise;
    _promise->setParent(this);

    connect(_promise, SIGNAL(ready()), this, SLOT(onPromiseReady()));
    if (_promise->isReady())
        onPromiseReady();
    //response->send(_socket);
}


void Client::onPromiseReady()
{
    Response* response = _promise->response();

    connect(response, SIGNAL(finished()), this, SLOT(onResponseFinished()));
    response->send(_socket);
}


void Client::onResponseFinished()
{
    _socket->disconnectFromHost();
}
