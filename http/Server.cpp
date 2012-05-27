#include "Server.h"

#include <QDebug>
#include <QTcpSocket>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QDateTime>

#include "Client.h"
#include "Request.h"
#include "Response.h"
#include "PlainResponse.h"
#include "Resource.h"


using namespace http;


Server::Server(QObject* parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, 8080);
}


Server::~Server()
{
    foreach (Resource* r, _resources)
        delete r;
}


void Server::incomingConnection(int socket)
{
    Client* client = new Client(socket, this);

    connect(client, SIGNAL(requestReady()), this, SLOT(onRequestReady()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}


void Server::onRequestReady()
{
    Client* client = dynamic_cast<Client*>(sender());

    const Request* request = client->request();
    bool handled = false;

    for (int i = 0; i < _resources.size(); i++)
    {
        Response* response = _resources.at(i)->handle(request);
        if (response)
        {
            client->response(response);

            handled = true;
            break;
        }
    }

    if (! handled)
    {
        PlainResponse* r404 = new PlainResponse;
        r404->setStatus(404, "Not found");
        r404->headers().insert("Content-Type", "text/plain");
        r404->setContent(QString("<h1>Not found</h1>").toUtf8());
        client->response(r404);
    }
}


void Server::onDisconnected()
{
    Client* client = dynamic_cast<Client*>(sender());
    client->deleteLater();
}
