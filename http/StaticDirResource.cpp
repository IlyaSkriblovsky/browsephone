#include "StaticDirResource.h"

#include <QDebug>
#include <QFile>

#include "Request.h"
#include "IODeviceResponse.h"
#include "PlainResponse.h"
#include "RedirectResponse.h"


using namespace http;


StaticDirResource::StaticDirResource(const QString& urlPrefix, const QString& dir)
    : _urlPrefix(urlPrefix), _dir(dir)
{
    _dir.makeAbsolute();
}


Response* StaticDirResource::handle(const Request* request)
{
    if (request->url().startsWith(_urlPrefix))
    {
        QString tail = request->url().mid(_urlPrefix.length());

        if (tail.contains("../"))
            return 0;

        QString filePath;

        if (tail.length() == 0)
            filePath = _dir.absolutePath();
        else
        {
            if (! _dir.exists(tail))
                return 0;

            filePath = _dir.absoluteFilePath(tail);
        }

        QFileInfo info(filePath);

        if (info.isDir())
        {
            if (tail.length() == 0 || tail.endsWith("/"))
                return dirListing(filePath);
            else
                return new RedirectResponse(_urlPrefix + tail + "/", true);
        }
        else
        {
            QFile* f = new QFile(filePath);
            if (! f->open(QIODevice::ReadOnly))
            {
                delete f;
                qDebug() << "Can't open" << tail;
                return 0;
            }

            return new IODeviceResponse(f);
        }
    }

    return 0;
}


Response* StaticDirResource::dirListing(const QString& path)
{
    QString begin =
        "<!DOCTYPE html>\n"
        "   <html>\n"
        "       <head>\n"
        "           <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
        "       </head>\n"
        "       <body>\n"
        "           <h1>" + path + "</h1>\n"
        "           <ul>\n"
    ;

    QString end =
        "           </ul>\n"
        "       </body>\n"
        "   </html>\n"
    ;

    QString mid;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList();

    foreach (QFileInfo entry, entries)
    {
        QString slashed = entry.fileName() + (entry.isDir() ? "/" : "");
        mid += "<li><a href=\"" + slashed + "\">" + slashed + "</a></li>";
    }

    PlainResponse* r = new PlainResponse;
    r->headers().insert("Content-Type", "text/html");
    r->setContent((begin + mid + end).toUtf8());
    return r;
}
