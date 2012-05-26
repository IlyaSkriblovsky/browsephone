TEMPLATE = app

QT = core network

CONFIG += debug


HEADERS = \
    HttpRequest.h   \
    HttpResponse.h  \
    Client.h        \
    HttpServer.h    \
    PlainResponse.h \
    Resource.h      \
    SimpleResource.h\
    TestResponse.h  \
    TestResource.h  \
    IODeviceResponse.h  \
    StaticDirResource.h \
    RedirectResponse.h

SOURCES = main.cpp      \
    HttpRequest.cpp     \
    HttpResponse.cpp    \
    Client.cpp          \
    HttpServer.cpp      \
    PlainResponse.cpp   \
    Resource.cpp        \
    SimpleResource.cpp  \
    TestResponse.cpp    \
    TestResource.cpp    \
    IODeviceResponse.cpp\
    StaticDirResource.cpp   \
    RedirectResponse.cpp
