TEMPLATE = app

QT = core network


HEADERS = \
    HttpRequest.h   \
    HttpResponse.h  \
    Client.h        \
    HttpServer.h    \
    PlainResponse.h \
    Resource.h      \
    SimpleResource.h

SOURCES = main.cpp      \
    HttpRequest.cpp     \
    HttpResponse.cpp    \
    Client.cpp          \
    HttpServer.cpp      \
    PlainResponse.cpp   \
    Resource.cpp        \
    SimpleResource.cpp
