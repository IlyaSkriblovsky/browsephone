#pragma once

#include "Resource.h"


class TestResource: public Resource
{
    public:
        TestResource(const QString& url);

        HttpResponse* handle(const HttpRequest* request);


    private:
        QString _url;
};
