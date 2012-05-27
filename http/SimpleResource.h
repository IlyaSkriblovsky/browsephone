#pragma once

#include "Resource.h"


namespace http
{


class SimpleResource: public Resource
{
    public:
        SimpleResource(const QString& url, const QByteArray& content, const QString& mime = "text/html; charset=utf-8");


        Response* handle(const Request* request);


    private:
        QString _url;
        QString _mime;
        QByteArray _content;
};


}
