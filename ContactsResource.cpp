#include "ContactsResource.h"

#include <QDebug>

#include <QContactFetchRequest>
#include <QContactName>
#include <QContactType>
#include <QContactGuid>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactNickname>
#include <QContactAddress>
#include <QContactAvatar>
#include <QContactAnniversary>
#include <QContactBirthday>
#include <QContactSyncTarget>
#include <QContactUrl>
#include <QContactNote>
#include <QContactFavorite>
#include <QContactOnlineAccount>
#include <QContactOrganization>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/Response.h"
#include "http/PlainResponse.h"

#include "JsonUtil.h"


ContactsResource::ContactsResource(const QString& url)
    : _url(url)
{
    _manager = new QContactManager;
    _manager->setParent(this);
}


http::ResponsePromise* ContactsResource::handle(const http::Request* request)
{
    if (request->url() != _url)
        return 0;


    http::ResponsePromise* promise = new http::ResponsePromise;

    QContactFetchRequest* r = new QContactFetchRequest(this);
    r->setManager(_manager);
    r->setProperty("promise", QVariant::fromValue((QObject*)promise));

    connect(r, SIGNAL(resultsAvailable()), this, SLOT(onResultsAvailable()));

    _timer.restart();
    r->start();

    return promise;
}

void ContactsResource::onResultsAvailable()
{
    QContactFetchRequest* r = static_cast<QContactFetchRequest*>(sender());
    r->deleteLater();

    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        r->property("promise").value<QObject*>()
    );


    QString json = "[\n";


    QList<QContact> contacts = r->contacts();

    qDebug() << contacts.size();
    qDebug() << _timer.elapsed();

    for (int ci = 0; ci < contacts.size(); ci++)
    {
        json += QString("  {\n");
        json += QString("    \"id\": %1,\n").arg(contacts[ci].id().localId());
        json += QString("    \"label\": \"%1\",\n").arg(contacts[ci].detail(QContactDisplayLabel::DefinitionName).value(QContactDisplayLabel::FieldLabel));
        json += QString("    \"details\": [\n");

        QList<QContactDetail> details = contacts[ci].details();
        for (int di = 0; di < details.size(); di++)
        {
            QString definitionName = details[di].definitionName();

            json += "      {\n";
            json += QString("        \"def\": %1,\n").arg(JsonUtil::string(details[di].definitionName()));

            QVariantMap fields = details[di].variantValues();
            QStringList strs;
            foreach (const QString& key, fields.keys())
                strs.append(QString("        \"%1\": %2").arg(key).arg(JsonUtil::serialize(fields.value(key))));

            json += strs.join(",\n");

            json += "\n      }";
            if (di < details.size() - 1) json += ",";
            json += "\n";
        }


        json += "    ]\n";
        json += "  }";

        if (ci < contacts.size()-1) json += ",";

        json += "\n";
    }

    json += "]\n";

    http::PlainResponse* response = new http::PlainResponse;
    response->headers().insert("Content-Type", "application/json; charset=utf-8");
    response->setContent(json.toUtf8());
    promise->fullfill(response);
}
