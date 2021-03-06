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
        QByteArray _content;
};


}
