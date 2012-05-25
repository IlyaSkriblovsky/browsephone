#pragma once

#include "Resource.h"


class SimpleResource: public Resource
{
    public:
        SimpleResource(const QString& url, const QByteArray& content, const QString& mime = "text/html; charset=utf-8");


        HttpResponse* handle(const HttpRequest* request);


    private:
        QString _url;
        QString _mime;
        QByteArray _content;
};
