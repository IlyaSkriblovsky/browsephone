#include "TestResource.h"

#include "HttpRequest.h"
#include "TestResponse.h"


TestResource::TestResource(const QString& url)
    : _url(url)
{
}

HttpResponse* TestResource::handle(const HttpRequest* request)
{
    if (request->url() == _url)
        return new TestResponse();

    return 0;
}
