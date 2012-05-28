#pragma once

#include <QObject>


class QTcpSocket;


namespace http
{

class Request;
class ResponsePromise;


class Client: public QObject
{
    Q_OBJECT

    public:
        Client(int socket, QObject* parent = 0);
        ~Client();

        const Request* request() const { return _request; }

        void promise(ResponsePromise* promise);

    signals:
        void requestReady();
        void disconnected();


    private slots:
        void onReadyRead();
        void onDisconnected();

        void onPromiseReady();
        void onResponseFinished();


    private:
        QTcpSocket* _socket;
        Request* _request;
        ResponsePromise* _promise;
};


}
