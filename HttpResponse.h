#pragma once

#include <QString>
#include <QMap>

class QTcpSocket;


class HttpResponse
{
    public:
        HttpResponse();
        ~HttpResponse();

        QMap<QString, QString>& headers() { return _headers; }

        void setStatus(int code, const QString& phrase) { _code = code; _phrase = phrase; }

        void send(QTcpSocket* socket);


    protected:
        virtual void sendContent(QTcpSocket* socket) = 0;


    private:
        QMap<QString, QString> _headers;

        int _code;
        QString _phrase;
};
