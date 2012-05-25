#pragma once

#include <QTcpServer>


class Resource;


class HttpServer: public QTcpServer
{
    Q_OBJECT

    public:
        HttpServer(QObject* parent = 0);
        ~HttpServer();

        void addResource(Resource* resource) { _resources.append(resource); }


    protected:
        void incomingConnection(int socket);


    private slots:
        void onRequestReady();
        void onDisconnected();


    private:
        QList<Resource*> _resources;
};
