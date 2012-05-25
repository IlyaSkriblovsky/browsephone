#pragma once

#include <QObject>


class QTcpSocket;

class HttpRequest;
class HttpResponse;


class Client: public QObject
{
    Q_OBJECT

    public:
        Client(int socket, QObject* parent = 0);
        ~Client();

        const HttpRequest* request() const { return _request; }

        void send(HttpResponse* response);

    signals:
        void requestReady();
        void disconnected();


    private slots:
        void onReadyRead();
        void onDisconnected();


    private:
        QTcpSocket* _socket;
        HttpRequest* _request;
};
