#pragma once

#include <QString>
#include <QByteArray>
#include <QMap>


class HttpRequest
{
    public:
        HttpRequest();
        ~HttpRequest();

        void feed(const QByteArray& chunk);

        bool ready() const { return _ready; }

        QString method() const { return _method; }
        QString url() const { return _url; }
        const QMap<QString, QString> headers() const { return _headers; }


    private:
        QByteArray _data;
        bool _ready;

        QString _method;
        QString _url;
        QMap<QString, QString> _headers;
};
