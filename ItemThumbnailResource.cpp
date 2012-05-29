#include "ItemThumbnailResource.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>

#include <QGalleryItemRequest>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/PlainResponse.h"
#include "http/IODeviceResponse.h"


ItemThumbnailResource::ItemThumbnailResource(const QString& url)
    : _url(url)
{
    _gallery = new QDocumentGallery(this);

    _properties << QDocumentGallery::url;
}


http::ResponsePromise* ItemThumbnailResource::handle(const http::Request* request)
{
    if (! request->url().startsWith(_url))
        return 0;


    QString itemId = request->url().mid(_url.length());

    QGalleryItemRequest* galleryRequest = new QGalleryItemRequest(_gallery);
    galleryRequest->setItemId(itemId);

    galleryRequest->setPropertyNames(_properties);


    http::ResponsePromise* promise = new http::ResponsePromise;

    galleryRequest->setProperty("promise", QVariant::fromValue(static_cast<QObject*>(promise)));

    connect(galleryRequest, SIGNAL(finished()), this, SLOT(onGalleryRequestFinished()));
    connect(galleryRequest, SIGNAL(error(int, QString)), this, SLOT(onGalleryRequestError(int, QString)));

    galleryRequest->execute();

    qDebug() << galleryRequest->error();

    return promise;
}


void ItemThumbnailResource::onGalleryRequestFinished()
{
    QGalleryItemRequest* request = static_cast<QGalleryItemRequest*>(sender());
    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        request->property("promise").value<QObject*>()
    );
    request->deleteLater();

    QByteArray url = request->metaData(QDocumentGallery::url).toByteArray();
    QString md5 = QCryptographicHash::hash(url, QCryptographicHash::Md5).toHex();

    QString thumbnailFile = QString("/home/user/.thumbnails/grid/%1.jpeg").arg(md5);

    QFile* f = new QFile(thumbnailFile);

    if (! f->open(QIODevice::ReadOnly))
    {
        delete f;

        http::PlainResponse* response = new http::PlainResponse;
        response->headers().insert("Content-Type", "text/html");
        response->setStatus(404, "No thumbnail");

        response->setContent(QString("Can't open %1").arg(thumbnailFile).toUtf8());

        promise->fullfill(response);
    }
    else
    {
        http::IODeviceResponse* response = new http::IODeviceResponse(f);
        response->headers().insert("Content-Type", "image/jpeg");
        promise->fullfill(response);
    }
}


void ItemThumbnailResource::onGalleryRequestError(int error, const QString& errorString)
{
    QGalleryItemRequest* request = static_cast<QGalleryItemRequest*>(sender());
    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        request->property("promise").value<QObject*>()
    );
    request->deleteLater();


    http::PlainResponse* response = new http::PlainResponse;
    response->headers().insert("Content-Type", "text/html");
    response->setStatus(404, "Gallery query error");
    response->setContent(QString("Gallery query error %1: %2").arg(error).arg(errorString).toUtf8());
    promise->fullfill(response);
}
