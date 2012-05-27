TEMPLATE = app

QT = core network

CONFIG += debug


target.path = /opt/browsephone/
INSTALLS += target


include(http/http.pri)


HEADERS += \
    TestResponse.h  \
    TestResource.h  \

SOURCES += main.cpp     \
    TestResponse.cpp    \
    TestResource.cpp    \
