#pragma once

#include <QString>


class HttpRequest;
class HttpResponse;


class Resource
{
    public:
        Resource();
        ~Resource();


        virtual HttpResponse* handle(const HttpRequest* request) = 0;
};
