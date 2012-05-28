#pragma once

#include <QObject>

#include "http/Resource.h"


class DeferredResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        DeferredResource(const QString& url);

        http::ResponsePromise* handle(const http::Request* request);


    private slots:
        void onTimer();

    private:
        QString _url;
};
