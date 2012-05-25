#include "HttpResponse.h"


class PlainResponse: public HttpResponse
{
    public:
        PlainResponse();


        void setContent(const QByteArray& content) { _content = content; }


    protected:
        void sendContent(QTcpSocket* socket);


    private:
        QByteArray _content;
};
