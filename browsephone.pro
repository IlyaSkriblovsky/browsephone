TEMPLATE = app

QT = core network

CONFIG += debug mobility
MOBILITY += gallery


target.path = /opt/browsephone/
INSTALLS += target


include(http/http.pri)


HEADERS += \
    TestResource.h  \
    DeferredResource.h  \
    ImagesResource.h

SOURCES += main.cpp     \
    TestResource.cpp    \
    DeferredResource.cpp\
    ImagesResource.cpp
