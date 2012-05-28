#include "ResponsePromise.h"

#include "Response.h"

using namespace http;


ResponsePromise::ResponsePromise(Response* response)
    : _response(response)
{
    if (_response)
        _response->setParent(this);
}


void ResponsePromise::fullfill(Response* response)
{
    _response = response;
    _response->setParent(this);
    ready();
}
