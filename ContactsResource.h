#pragma once

#include "http/Resource.h"

#include <QElapsedTimer>

#include <QContactManager>

QTM_USE_NAMESPACE


class ContactsResource: public QObject, public http::Resource
{
    Q_OBJECT

    public:
        ContactsResource(const QString& url);

        http::ResponsePromise* handle(const http::Request* request);



    private slots:
        void onResultsAvailable();


    private:
        QContactManager* _manager;

        QString _url;

        QElapsedTimer _timer;
};
