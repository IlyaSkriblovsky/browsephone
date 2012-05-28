#include "DeferredResource.h"

#include <QTimer>
#include <QVariant>

#include "http/Request.h"
#include "http/PlainResponse.h"


DeferredResource::DeferredResource(const QString& url)
    : _url(url)
{
}


http::Response* DeferredResource::handle(const http::Request* request)
{
    if (request->url() == _url)
    {
        http::PlainResponse* r = new http::PlainResponse;

        QTimer* t = new QTimer(this);
        t->setProperty("response", QVariant::fromValue((QObject*)r));
        t->setSingleShot(true);
        connect(t, SIGNAL(timeout()), this, SLOT(onTimer()));
        t->start(5000);

        return r;
    }

    return 0;
}

void DeferredResource::onTimer()
{
    QTimer* t = static_cast<QTimer*>(sender());
    http::PlainResponse* r = static_cast<http::PlainResponse*>(
        t->property("response").value<QObject*>()
    );
    r->setContent("5sec");
    delete t;
}
