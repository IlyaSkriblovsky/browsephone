#include "ImagesResource.h"

#include <QDebug>

#include <QDocumentGallery>
#include <QGalleryQueryRequest>
#include <QGalleryResultSet>

#include "http/Request.h"
#include "http/PlainResponse.h"

QTM_USE_NAMESPACE


ImagesResource::ImagesResource()
{
    _gallery = new QDocumentGallery(this);
}



http::Response* ImagesResource::handle(const http::Request* request)
{
    if (request->url() == "/images")
    {
        QGalleryQueryRequest* galleryRequest = new QGalleryQueryRequest(_gallery);
        galleryRequest->setRootType(QDocumentGallery::Image);

        QStringList propertyNames;
        propertyNames
            << QDocumentGallery::title
            << QDocumentGallery::width
            << QDocumentGallery::height
            << QDocumentGallery::keywords
        ;
        galleryRequest->setPropertyNames(propertyNames);

        http::PlainResponse* response = new http::PlainResponse;
        galleryRequest->setProperty("response", QVariant::fromValue(static_cast<QObject*>(response)));

        connect(galleryRequest, SIGNAL(finished()), this, SLOT(onListRequestFinished()));

        galleryRequest->execute();

        return response;
    }

    return 0;
}

void ImagesResource::onListRequestFinished()
{
    qDebug() << "onListRequestFinished";

    QGalleryQueryRequest* request = static_cast<QGalleryQueryRequest*>(sender());
    http::PlainResponse* response = static_cast<http::PlainResponse*>(
        request->property("response").value<QObject*>()
    );
    request->deleteLater();

    QGalleryResultSet* resultSet = request->resultSet();

    QStringList properties;
    properties
        << QDocumentGallery::title
        << QDocumentGallery::width
        << QDocumentGallery::height
        << QDocumentGallery::keywords
    ;

    int keys[properties.size()];
    QVariant::Type types[properties.size()];
    for (int i = 0; i < properties.size(); i++)
    {
        keys[i] = resultSet->propertyKey(properties.at(i));
        types[i] = resultSet->propertyType(keys[i]);
    }



    QString text = "[\n";

    if (resultSet->fetchFirst())
    {
        do {
            text += "  {\n";
            text += "    id: \"" + resultSet->itemId().toString() + "\",\n";
            for (int i = 0; i < properties.size(); i++)
            {
                text += "    " + properties.at(i) + ": ";
                switch (types[i])
                {
                    case QVariant::String: text += "\"" + resultSet->metaData(keys[i]).toString() + "\","; break;
                    case QVariant::Int: text += resultSet->metaData(keys[i]).toString() + ","; break;
                    default: text += "'<unknown type>',\n";
                }
            }
            text += "  },\n";
        } while (resultSet->fetchNext());
    }

    text += "]\n";

    response->setContent(text.toUtf8());
}
