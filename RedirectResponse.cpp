#include "RedirectResponse.h"

#include <QTcpSocket>


RedirectResponse::RedirectResponse(const QString& url, bool permanent)
    : _url(url), _permanent(permanent)
{
    setStatus(
        permanent ? 301 : 302,
        permanent ?  "Moved permanently" : "Found"
    );

    headers().insert("Location", url);
}


void RedirectResponse::sendContent(QTcpSocket* socket)
{
    finished();
}
