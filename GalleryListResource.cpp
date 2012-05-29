#include "GalleryListResource.h"

#include <QDebug>

#include <QDocumentGallery>
#include <QGalleryQueryRequest>
#include <QGalleryResultSet>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/PlainResponse.h"

QTM_USE_NAMESPACE


GalleryListResource::GalleryListResource(
    const QString& url,
    const QGalleryType& type,
    const QStringList& properties
)
    : _url(url), _type(type), _properties(properties)
{
    _gallery = new QDocumentGallery(this);
}



http::ResponsePromise* GalleryListResource::handle(const http::Request* request)
{
    if (request->url() == _url)
    {
        QGalleryQueryRequest* galleryRequest = new QGalleryQueryRequest(_gallery);
        galleryRequest->setRootType(_type);

        galleryRequest->setPropertyNames(_properties);

        http::ResponsePromise* promise = new http::ResponsePromise;
        galleryRequest->setProperty("promise", QVariant::fromValue(static_cast<QObject*>(promise)));

        connect(galleryRequest, SIGNAL(finished()), this, SLOT(onListRequestFinished()));

        galleryRequest->execute();

        return promise;
    }

    return 0;
}

void GalleryListResource::onListRequestFinished()
{
    QGalleryQueryRequest* request = static_cast<QGalleryQueryRequest*>(sender());
    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        request->property("promise").value<QObject*>()
    );
    request->deleteLater();

    QGalleryResultSet* resultSet = request->resultSet();


    int keys[_properties.size()];
    QVariant::Type types[_properties.size()];
    for (int i = 0; i < _properties.size(); i++)
    {
        keys[i] = resultSet->propertyKey(_properties.at(i));
        types[i] = resultSet->propertyType(keys[i]);
    }



    QString text = "[\n";

    if (resultSet->fetchFirst())
    {
        bool hasNext = false;

        do {
            text += "  {\n";
            text += "    \"id\": \"" + resultSet->itemId().toString() + "\",\n";
            for (int i = 0; i < _properties.size(); i++)
            {
                text += "    \"" + _properties.at(i) + "\": ";

                if (resultSet->metaData(keys[i]).isValid())
                {
                    if (resultSet->metaData(keys[i]).isNull())
                        text += "null";
                    else
                    {
                        switch (types[i])
                        {
                            case QVariant::String: text += "\"" + resultSet->metaData(keys[i]).toString() + "\""; break;
                            case QVariant::Int: text += resultSet->metaData(keys[i]).toString(); break;
                            case QVariant::StringList: {
                                QStringList sl = resultSet->metaData(keys[i]).toStringList();
                                text += "[\n";
                                for (int j = 0; j < sl.size(); j++)
                                {
                                    text += "      \"" + sl.at(j) + "\"";
                                    if (j != sl.size()-1)
                                        text += ",";

                                    text += "\n";
                                }
                                text += "    ]";
                                break;
                            }
                            default: {
                                const char *typeName = QVariant::typeToName(types[i]);
                                if (typeName != 0)
                                    text += QString("'{unknown type: ") + typeName + "}'";
                                else
                                    text += "'{unknown type: nil}'";
                            }
                        }
                    }
                }
                else
                    text += "\"undefined\"";


                if (i != _properties.size()-1)
                    text += ",";

                text += "\n";
            }
            text += "  }";

            hasNext = resultSet->fetchNext();
            if (hasNext)
                text += ",";

            text += "\n";

        } while (hasNext);
    }

    text += "]\n";


    http::PlainResponse* r = new http::PlainResponse;
    r->headers().insert("Content-Type", "application/json; charset=utf-8");
    r->setContent(text.toUtf8());
    promise->fullfill(r);
}
