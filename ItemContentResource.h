#pragma once

#include "http/Resource.h"

#include <QStringList>

#include <QDocumentGallery>

QTM_USE_NAMESPACE



class ItemContentResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        ItemContentResource(const QString& url);

        http::ResponsePromise* handle(const http::Request* request);


    private slots:
        void onGalleryRequestFinished();


    private:
        QDocumentGallery* _gallery;

        QString _url;

        QStringList _properties;
};
