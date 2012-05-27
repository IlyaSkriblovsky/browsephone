#include "Request.h"

#include <QDebug>
#include <QStringList>


using namespace http;


Request::Request()
    : _ready(false)
{
}

Request::~Request()
{
}


void Request::feed(const QByteArray& chunk)
{
    _data.append(chunk);
    _ready = _data.contains("\r\n\r");

    if (_ready)
    {
        QStringList lines = QString::fromUtf8(_data).split("\r\n", QString::SkipEmptyParts);

        QString firstLine = lines.at(0);
        QStringList parts = firstLine.split(' ');
        _method = parts.at(0);
        _url = parts.at(1);

        for (int i = 1; i < lines.size(); i++)
        {
            QString line = lines.at(i);
            int colon = line.indexOf(':');
            if (colon != -1)
            {
                if (line.at(colon+1) == ' ')
                    _headers.insert(line.left(colon), line.mid(colon+2));
                else
                    _headers.insert(line.left(colon), line.mid(colon+1));
            }
        }
    }
}
