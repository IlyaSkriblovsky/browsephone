#pragma once

#include "Resource.h"

#include <QDir>


namespace http
{

class Response;


class StaticDirResource: public Resource
{
    public:
        StaticDirResource(const QString& urlPrefix, const QString& dir);


        virtual ResponsePromise* handle(const Request* request);


    private:
        QString _urlPrefix;
        QDir _dir;


        Response* dirListing(const QString& path);
};


}
