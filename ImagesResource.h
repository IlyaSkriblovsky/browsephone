#pragma once

#include "http/Resource.h"


#include <QDocumentGallery>

QTM_USE_NAMESPACE



class ImagesResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        ImagesResource();

        http::Response* handle(const http::Request* request);


    private slots:
        void onListRequestFinished();

    private:
        QDocumentGallery* _gallery;
};
