#pragma once

#include "Response.h"


namespace http
{


class RedirectResponse: public Response
{
    public:
        RedirectResponse(const QString& url, bool permanent);


    protected:
        void sendContent(QTcpSocket* socket);


    private:
        const QString& _url;
        bool _permanent;
};


}
