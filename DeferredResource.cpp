#include "DeferredResource.h"

#include <QDebug>
#include <QTimer>
#include <QVariant>

#include "http/Request.h"
#include "http/ResponsePromise.h"
#include "http/PlainResponse.h"


DeferredResource::DeferredResource(const QString& url)
    : _url(url)
{
}


http::ResponsePromise* DeferredResource::handle(const http::Request* request)
{
    if (request->url() == _url)
    {
        http::ResponsePromise* promise = new http::ResponsePromise;

        QTimer* t = new QTimer(this);
        t->setProperty("promise", QVariant::fromValue((QObject*)promise));
        t->setSingleShot(true);
        connect(t, SIGNAL(timeout()), this, SLOT(onTimer()));
        t->start(5000);

        return promise;
    }

    return 0;
}

void DeferredResource::onTimer()
{
    QTimer* t = static_cast<QTimer*>(sender());
    http::ResponsePromise* promise = static_cast<http::ResponsePromise*>(
        t->property("promise").value<QObject*>()
    );

    delete t;


    http::PlainResponse* r = new http::PlainResponse;
    r->setContent("5sec");

    promise->fullfill(r);
}
