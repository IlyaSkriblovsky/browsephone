#include "SimpleResource.h"

#include "HttpRequest.h"
#include "PlainResponse.h"


SimpleResource::SimpleResource(const QString& url, const QByteArray& content, const QString& mime)
    : _url(url), _mime(mime), _content(content)
{
}


HttpResponse* SimpleResource::handle(const HttpRequest* request)
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
