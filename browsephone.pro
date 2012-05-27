TEMPLATE = app

QT = core network

CONFIG += debug


include(http/http.pri)


HEADERS += \
    TestResponse.h  \
    TestResource.h  \

SOURCES += main.cpp     \
    TestResponse.cpp    \
    TestResource.cpp    \
