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


    QString json = "{\n";


    QList<QContact> contacts = r->contacts();

    qDebug() << contacts.size();
    qDebug() << _timer.elapsed();

    for (int ci = 0; ci < contacts.size(); ci++)
    {
        // qDebug() << contacts[ci].detail(QContactDisplayLabel::DefinitionName).value(QContactDisplayLabel::FieldLabel) << contacts[ci].id().localId();

        json += QString("  %1: {\n").arg(contacts[ci].id().localId());
        json += QString("    label: \"%1\",\n").arg(contacts[ci].detail(QContactDisplayLabel::DefinitionName).value(QContactDisplayLabel::FieldLabel));
        json += QString("    details: [\n");

        QList<QContactDetail> details = contacts[ci].details();
        for (int di = 0; di < details.size(); di++)
        {
            QString definitionName = details[di].definitionName();

            json += "      {\n";
            json += QString("        def: \"%1\",\n").arg(definitionName);

            QStringList contexts = details[di].contexts();
            if (contexts.size() > 0)
                json += QString("        contexts: [\"%1\"]").arg(contexts.join("\", \""));
            else
                json += "        contexts: []";


            if (definitionName == QContactPhoneNumber::DefinitionName)
            {
                json += ",\n";

                json += QString("        number: \"%1\",\n").arg(details[di].value(QContactPhoneNumber::FieldNumber));
                json += QString("        subtypes: [\"%1\"]\n").arg(
                    details[di].value<QStringList>(QContactPhoneNumber::FieldSubTypes).join("\", \"")
                );
            }
            else if (definitionName == QContactEmailAddress::DefinitionName)
            {
                json += ",\n";

                json += QString("        email: \"%1\"\n").arg(details[di].value(QContactEmailAddress::FieldEmailAddress));
            }
            else if (definitionName == QContactType::DefinitionName)
            {
                json += ",\n";

                json += QString("        type: \"%1\"\n").arg(details[di].value(QContactType::FieldType));
            }
            else if (definitionName == QContactGuid::DefinitionName)
            {
                json += ",\n";

                json += QString("        guid: \"%1\"\n").arg(details[di].value(QContactGuid::FieldGuid));
            }
            else if (definitionName == QContactName::DefinitionName)
            {
                json += ",\n";

                json += "        name: {\n";
                json += QString("          custom: \"%1\",\n").arg(details[di].value(QContactName::FieldCustomLabel));
                json += QString("          first: \"%1\",\n").arg(details[di].value(QContactName::FieldFirstName));
                json += QString("          last: \"%1\",\n").arg(details[di].value(QContactName::FieldLastName));
                json += QString("          middle: \"%1\",\n").arg(details[di].value(QContactName::FieldMiddleName));
                json += QString("          prefix: \"%1\",\n").arg(details[di].value(QContactName::FieldPrefix));
                json += QString("          suffix: \"%1\"\n").arg(details[di].value(QContactName::FieldSuffix));
                json += "        }\n";
            }
            else if (definitionName == QContactNickname::DefinitionName)
            {
                json += ",\n";

                json += QString("        nickname: \"%1\"\n").arg(details[di].value(QContactNickname::FieldNickname));
            }
            else if (definitionName == QContactAddress::DefinitionName)
            {
                json += ",\n";

                json += QString("        subtypes: [\"%1\"],\n").arg(details[di].value<QStringList>(QContactAddress::FieldSubTypes).join("\", \""));

                json += "        address: {\n";
                json += QString("          country: \"%1\",\n").arg(details[di].value(QContactAddress::FieldCountry));
                json += QString("          locality: \"%1\",\n").arg(details[di].value(QContactAddress::FieldLocality));
                json += QString("          postOfficeBox: \"%1\",\n").arg(details[di].value(QContactAddress::FieldPostOfficeBox));
                json += QString("          postcode: \"%1\",\n").arg(details[di].value(QContactAddress::FieldPostcode));
                json += QString("          region: \"%1\",\n").arg(details[di].value(QContactAddress::FieldRegion));
                json += QString("          street: \"%1\"\n").arg(details[di].value(QContactAddress::FieldStreet));
                json += "        }\n";
            }
            else if (definitionName == QContactAvatar::DefinitionName)
            {
                json += ",\n";

                json += QString("        image: \"%1\",\n").arg(details[di].value(QContactAvatar::FieldImageUrl));
                json += QString("        video: \"%1\"\n").arg(details[di].value(QContactAvatar::FieldVideoUrl));
            }
            else if (definitionName == QContactBirthday::DefinitionName)
            {
                json += ",\n";

                json += QString("        date: \"%1\"\n").arg(details[di].value<QDate>(QContactBirthday::FieldBirthday).toString("dd.MM.yyyy"));
            }
            else if (definitionName == QContactAnniversary::DefinitionName)
            {
                json += ",\n";

                json += QString("        subtype: \"%1\",\n").arg(details[di].value(QContactAnniversary::FieldSubType));
                json += QString("        event: \"%1\",\n").arg(details[di].value(QContactAnniversary::FieldEvent));
                json += QString("        date: \"%1\"\n").arg(details[di].value<QDate>(QContactAnniversary::FieldOriginalDate).toString("dd.MM.yyyy"));
            }
            else if (definitionName == QContactSyncTarget::DefinitionName)
            {
                json += ",\n";

                json += QString("        syncTarget: \"%1\"\n").arg(details[di].value(QContactSyncTarget::FieldSyncTarget));
            }
            else if (definitionName == QContactUrl::DefinitionName)
            {
                json += ",\n";

                json += QString("        subtype: \"%1\",\n").arg(details[di].value(QContactUrl::FieldSubType));
                json += QString("        url: \"%1\"\n").arg(details[di].value(QContactUrl::FieldUrl));
            }
            else if (definitionName == QContactNote::DefinitionName)
            {
                json += ",\n";

                json += QString("        note: \"%1\"\n").arg(details[di].value(QContactNote::FieldNote));
            }
            else if (definitionName == QContactFavorite::DefinitionName)
            {
                json += ",\n";

                if (details[di].value<bool>(QContactFavorite::FieldFavorite))
                    json += "        favorite: true,\n";
                else
                    json += "        favorite: false,\n";
                json += QString("        index: %1\n").arg(details[di].value<int>(QContactFavorite::FieldIndex));
            }
            else if (definitionName == QContactOnlineAccount::DefinitionName)
            {
                json += ",\n";

                json += QString("        subtypes: [\"%1\"],\n").arg(details[di].value<QStringList>(QContactOnlineAccount::FieldSubTypes).join("\", \""));
                json += QString("        capabilities: [\"%1\"],\n").arg(details[di].value<QStringList>(QContactOnlineAccount::FieldCapabilities).join("\", \""));

                json += QString("        accountUri: \"%1\",\n").arg(details[di].value(QContactOnlineAccount::FieldAccountUri));
                json += QString("        protocol: \"%1\",\n").arg(details[di].value(QContactOnlineAccount::FieldProtocol));
                json += QString("        serviceProvider: \"%1\"\n").arg(details[di].value(QContactOnlineAccount::FieldServiceProvider));
            }
            else if (definitionName == QContactOrganization::DefinitionName)
            {
                json += ",\n";

                json += QString("        department: [\"%1\"],\n").arg(details[di].value<QStringList>(QContactOrganization::FieldDepartment).join("\", \""));

                json += QString("        assistantName: \"%1\",\n").arg(details[di].value(QContactOrganization::FieldAssistantName));
                json += QString("        location: \"%1\",\n").arg(details[di].value(QContactOrganization::FieldLocation));
                json += QString("        name: \"%1\",\n").arg(details[di].value(QContactOrganization::FieldName));
                json += QString("        role: \"%1\",\n").arg(details[di].value(QContactOrganization::FieldRole));
                json += QString("        title: \"%1\"\n").arg(details[di].value(QContactOrganization::FieldTitle));
            }
            else
            {
                qDebug() << "    " << details[di];

                json += "\n";
            }

            json += "      }";
            if (di < details.size() - 1) json += ",";
            json += "\n";
        }


        json += "    ]\n";
        json += "  }";

        if (ci < contacts.size()-1) json += ",";

        json += "\n";
    }

    json += "}\n";

    http::PlainResponse* response = new http::PlainResponse;
    response->headers().insert("Content-Type", "application/json; charset=utf-8");
    response->setContent(json.toUtf8());
    promise->fullfill(response);
}
