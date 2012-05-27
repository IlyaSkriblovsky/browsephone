#pragma once

#include <QTcpServer>


namespace http
{


class Resource;


class Server: public QTcpServer
{
    Q_OBJECT

    public:
        Server(QObject* parent = 0);
        ~Server();

        void addResource(Resource* resource) { _resources.append(resource); }


    protected:
        void incomingConnection(int socket);


    private slots:
        void onRequestReady();
        void onDisconnected();


    private:
        QList<Resource*> _resources;
};


}
