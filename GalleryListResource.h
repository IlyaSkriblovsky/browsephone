#pragma once

#include "http/Resource.h"

#include <QStringList>

#include <QDocumentGallery>

QTM_USE_NAMESPACE



class GalleryListResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        GalleryListResource(const QString& url, const QGalleryType& type, const QStringList& properties);

        http::ResponsePromise* handle(const http::Request* request);


    private slots:
        void onListRequestFinished();

    private:
        QDocumentGallery* _gallery;

        QString _url;
        QGalleryType _type;
        QStringList _properties;
};
