#pragma once

#include <QString>


namespace http
{


class Request;
class Response;


class Resource
{
    public:
        Resource();
        ~Resource();


        virtual Response* handle(const Request* request) = 0;
};


}
