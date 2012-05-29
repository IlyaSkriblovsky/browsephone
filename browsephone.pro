TEMPLATE = app

QT = core network

CONFIG += debug mobility
MOBILITY += gallery


target.path = /opt/browsephone/
INSTALLS += target


static.files = static/*
static.path = /opt/browsephone/static/
INSTALLS += static


include(http/http.pri)


HEADERS += \
    DeferredResource.h  \
    GalleryListResource.h   \
    ItemContentResource.h   \
    ItemThumbnailResource.h

SOURCES += main.cpp     \
    DeferredResource.cpp\
    GalleryListResource.cpp \
    ItemContentResource.cpp \
    ItemThumbnailResource.cpp
