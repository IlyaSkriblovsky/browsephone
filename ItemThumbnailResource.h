#pragma once

#include "http/Resource.h"

#include <QDocumentGallery>

QTM_USE_NAMESPACE


class QFile;


class ItemThumbnailResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        ItemThumbnailResource(const QString& url);

        http::ResponsePromise* handle(const http::Request* request);


    private slots:
        void onGalleryRequestFinished();
        void onGalleryRequestError(int error, const QString& errorString);


    private:
        QDocumentGallery* _gallery;

        QString _url;

        QStringList _properties;


        QFile* tryOpen(const QString& filename);
};
