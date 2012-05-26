#pragma once

#include "Resource.h"

#include <QDir>


class HttpResponse;


class StaticDirResource: public Resource
{
    public:
        StaticDirResource(const QString& urlPrefix, const QString& dir);


        virtual HttpResponse* handle(const HttpRequest* request);


    private:
        QString _urlPrefix;
        QDir _dir;


        HttpResponse* dirListing(const QString& path);
};
