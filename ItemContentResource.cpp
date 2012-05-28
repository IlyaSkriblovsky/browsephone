#include "ItemContentResource.h"

#include <QDebug>
#include <QFile>

#include <QGalleryItemRequest>
#include <QGalleryResultSet>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/PlainResponse.h"
#include "http/IODeviceResponse.h"


ItemContentResource::ItemContentResource(const QString& url)
    : _url(url)
{
    _gallery = new QDocumentGallery(this);

    _properties << QDocumentGallery::filePath << QDocumentGallery::mimeType;
}


http::ResponsePromise* ItemContentResource::handle(const http::Request* request)
{
    if (request->url().startsWith(_url))
    {
        QString itemId = request->url().mid(_url.length());

        QGalleryItemRequest* galleryRequest = new QGalleryItemRequest(_gallery);
        galleryRequest->setItemId(itemId);

        galleryRequest->setPropertyNames(_properties);


        http::ResponsePromise* promise = new http::ResponsePromise;

        galleryRequest->setProperty("promise", QVariant::fromValue(static_cast<QObject*>(promise)));

        connect(galleryRequest, SIGNAL(finished()), this, SLOT(onGalleryRequestFinished()));

        galleryRequest->execute();

        return promise;
    }

    return 0;
}



void ItemContentResource::onGalleryRequestFinished()
{
    QGalleryItemRequest* request = static_cast<QGalleryItemRequest*>(sender());
    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        request->property("promise").value<QObject*>()
    );
    request->deleteLater();


    QString filePath = QUrl::fromPercentEncoding(request->metaData(QDocumentGallery::filePath).toByteArray());


    QFile* f = new QFile(filePath);
    if (! f->open(QIODevice::ReadOnly))
    {
        delete f;

        http::PlainResponse* response = new http::PlainResponse;
        response->headers().insert("Content-Type", "text/html");
        response->setStatus(500, "Can't open file");

        response->setContent(QString("Can't open %1").arg(filePath).toUtf8());

        promise->fullfill(response);
    }
    else
    {
        http::IODeviceResponse* response = new http::IODeviceResponse(f);
        response->headers().insert("Content-Type", request->metaData(QDocumentGallery::mimeType).toString());
        promise->fullfill(response);
    }
}
