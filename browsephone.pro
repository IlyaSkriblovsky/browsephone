TEMPLATE = app

QT = core network

CONFIG += debug mobility
MOBILITY += gallery contacts


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
    ItemThumbnailResource.h \
    ContactsResource.h      \
    JsonUtil.h

SOURCES += main.cpp     \
    DeferredResource.cpp\
    GalleryListResource.cpp \
    ItemContentResource.cpp \
    ItemThumbnailResource.cpp   \
    ContactsResource.cpp        \
    JsonUtil.cpp
