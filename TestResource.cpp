#include "TestResource.h"

#include <QDebug>

#include <QDocumentGallery>
#include <QGalleryQueryRequest>
#include <QGalleryResultSet>

#include "http/Request.h"
#include "http/PlainResponse.h"


QTM_USE_NAMESPACE


TestResource::TestResource(const QString& url)
    : _url(url)
{
}

http::Response* TestResource::handle(const http::Request* request)
{
    if (request->url() == _url)
    {
        QDocumentGallery gallery;
        QGalleryQueryRequest request(&gallery);
        request.setRootType(QDocumentGallery::Image);

        QStringList propertyNames;
        propertyNames
            << QDocumentGallery::title
            << QDocumentGallery::width
            << QDocumentGallery::height
            << QDocumentGallery::keywords
            << QDocumentGallery::filePath
        ;
        request.setPropertyNames(propertyNames);

        request.execute();
        request.waitForFinished(2000);
        qDebug() << request.itemCount();

        QGalleryResultSet* resultSet = request.resultSet();
        qDebug() << resultSet->itemCount();

        QString content = "";

        qDebug() << request.propertyNames();

        int titleKey = resultSet->propertyKey(QDocumentGallery::title);
        int widthKey = resultSet->propertyKey(QDocumentGallery::width);
        int heightKey = resultSet->propertyKey(QDocumentGallery::height);
        int keywordsKey = resultSet->propertyKey(QDocumentGallery::keywords);
        int filePathKey = resultSet->propertyKey(QDocumentGallery::filePath);

        if (resultSet->fetchFirst())
        {
            do {
                content += resultSet->itemId().toString() + "\n";
                content += resultSet->metaData(filePathKey).toString() + "\n";
                content += resultSet->metaData(titleKey).toString() + "\n";
                content += resultSet->metaData(widthKey).toString() + "\n";
                content += resultSet->metaData(heightKey).toString() + "\n";
                content += resultSet->metaData(keywordsKey).toString() + "\n";
                content += "\n";
            } while (resultSet->fetchNext());
        }

        http::PlainResponse* r = new http::PlainResponse();
        r->headers().insert("Content-Type", "text/plain");
        r->setContent(content.toUtf8());
        return r;
    }

    return 0;
}
