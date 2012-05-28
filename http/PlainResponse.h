#pragma once

#include "Response.h"


namespace http
{


class PlainResponse: public Response
{
    public:
        PlainResponse();


        void setContent(const QByteArray& content);


    protected:
        void sendContent(QTcpSocket* socket);


    private:
        bool _contentSet;
        QByteArray _content;

        bool _deferred;
        QTcpSocket* _deferredSocket;
};


}
