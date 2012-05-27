#include "TestResource.h"

#include "http/Request.h"

#include "TestResponse.h"



TestResource::TestResource(const QString& url)
    : _url(url)
{
}

http::Response* TestResource::handle(const http::Request* request)
{
    if (request->url() == _url)
        return new TestResponse();

    return 0;
}
