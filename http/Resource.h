#pragma once

#include <QString>


namespace http
{


class Request;
class ResponsePromise;


class Resource
{
    public:
        Resource();
        ~Resource();


        virtual ResponsePromise* handle(const Request* request) = 0;
};


}
