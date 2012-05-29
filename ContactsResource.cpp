#include "ContactsResource.h"

#include <QDebug>

#include <QContactFetchRequest>
#include <QContactName>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/Response.h"


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


    QContactFetchRequest* r = new QContactFetchRequest(this);
    r->setManager(_manager);

    connect(r, SIGNAL(resultsAvailable()), this, SLOT(onResultsAvailable()));

    _timer.restart();
    r->start();

    return 0;
}

void ContactsResource::onResultsAvailable()
{
    QContactFetchRequest* r = static_cast<QContactFetchRequest*>(sender());
    r->deleteLater();


    QList<QContact> contacts = r->contacts();

    qDebug() << contacts.size();
    qDebug() << _timer.elapsed();

    foreach (const QContact& contact, contacts)
    {
        qDebug() << contact.detail(QContactDisplayLabel::DefinitionName).value(QContactDisplayLabel::FieldLabel);

        QList<QContactDetail> details = contact.details();
        foreach (const QContactDetail& detail, details)
            qDebug() << "    " << detail;
    }
}
