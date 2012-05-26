#pragma once

#include "HttpResponse.h"


class RedirectResponse: public HttpResponse
{
    public:
        RedirectResponse(const QString& url, bool permanent);


    protected:
        void sendContent(QTcpSocket* socket);


    private:
        const QString& _url;
        bool _permanent;
};
