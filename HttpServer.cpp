#include "HttpServer.h"

#include <QDebug>
#include <QTcpSocket>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QDateTime>

#include "Client.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "PlainResponse.h"
#include "Resource.h"


HttpServer::HttpServer(QObject* parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, 8080);
}


HttpServer::~HttpServer()
{
    foreach (Resource* r, _resources)
        delete r;
}


void HttpServer::incomingConnection(int socket)
{
    Client* client = new Client(socket, this);

    qDebug() << "new client";

    connect(client, SIGNAL(requestReady()), this, SLOT(onRequestReady()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}


void HttpServer::onRequestReady()
{
    Client* client = dynamic_cast<Client*>(sender());

    const HttpRequest* request = client->request();
    bool handled = false;

    for (int i = 0; i < _resources.size(); i++)
    {
        HttpResponse* response = _resources.at(i)->handle(request);
        if (response)
        {
            client->send(response);
            delete response;

            handled = true;
            break;
        }
    }

    if (! handled)
    {
        PlainResponse r404;
        r404.setStatus(404, "Not found");
        r404.headers().insert("Content-Type", "text/plain");
        r404.setContent(QString("<h1>Not found</h1>").toUtf8());
        client->send(&r404);
    }
}


void HttpServer::onDisconnected()
{
    Client* client = dynamic_cast<Client*>(sender());
    client->deleteLater();
    qDebug() << "client disconnected";
}
