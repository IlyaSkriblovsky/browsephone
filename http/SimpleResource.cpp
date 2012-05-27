#include "SimpleResource.h"

#include "Request.h"
#include "PlainResponse.h"


using namespace http;


SimpleResource::SimpleResource(const QString& url, const QByteArray& content, const QString& mime)
    : _url(url), _mime(mime), _content(content)
{
}


Response* SimpleResource::handle(const Request* request)
{
    if (request->url() == _url)
    {
        PlainResponse* response = new PlainResponse;
        response->setStatus(200, "OK");
        response->headers().insert("Content-Type", _mime);
        response->setContent(_content);
        return response;
    }

    return 0;
}
