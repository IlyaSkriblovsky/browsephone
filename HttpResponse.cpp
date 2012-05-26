#include "HttpResponse.h"

#include <QDebug>
#include <QTcpSocket>


HttpResponse::HttpResponse()
    : _code(200), _phrase("OK")
{
    _headers.insert("Server", "Ilya/0.1");
    _headers.insert("Date", httpDate(QDateTime::currentDateTimeUtc()));
}

HttpResponse::~HttpResponse()
{
}


void HttpResponse::sendHeaders(QTcpSocket* socket)
{
    socket->write(
        QString("HTTP/1.1 %1 %2\r\n")
            .arg(_code)
            .arg(_phrase)
            .toUtf8()
    );

    foreach (QString key, _headers.keys())
    {
        socket->write(key.toUtf8());
        socket->write(": ");
        socket->write(_headers.value(key).toUtf8());
        socket->write("\r\n");
    }

    socket->write("\r\n");
}

void HttpResponse::send(QTcpSocket* socket)
{
    sendHeaders(socket);
    sendContent(socket);
}


void HttpResponse::abort()
{
}


QString HttpResponse::httpDate(QDateTime dt)
{
    static const QString weekDay[] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };

    static const QString month[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    return QString("%1, %2 %3 %4 %5:%6:%7 GMT")
        .arg(weekDay[dt.date().dayOfWeek()-1])
        .arg(dt.date().day(), 2, 10, QChar('0'))
        .arg(month[dt.date().month()-1])
        .arg(dt.date().year())
        .arg(dt.time().hour(), 2, 10, QChar('0'))
        .arg(dt.time().minute(), 2, 10, QChar('0'))
        .arg(dt.time().second(), 2, 10, QChar('0'))
    ;
}
