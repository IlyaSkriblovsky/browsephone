#pragma once

#include "http/Resource.h"


class TestResource: public http::Resource
{
    public:
        TestResource(const QString& url);

        http::Response* handle(const http::Request* request);


    private:
        QString _url;
};
