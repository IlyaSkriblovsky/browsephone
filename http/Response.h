#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <QDateTime>

class QTcpSocket;


namespace http
{


class Response: public QObject
{
    Q_OBJECT

    public:
        Response();
        ~Response();

        QMap<QString, QString>& headers() { return _headers; }

        void setStatus(int code, const QString& phrase) { _code = code; _phrase = phrase; }

        void send(QTcpSocket* socket);
        virtual void abort();


    signals:
        void finished();


    protected:
        virtual void sendContent(QTcpSocket* socket) = 0;


    private:
        QMap<QString, QString> _headers;

        int _code;
        QString _phrase;


        void sendHeaders(QTcpSocket* socket);

        static QString httpDate(QDateTime dt);
};


}
