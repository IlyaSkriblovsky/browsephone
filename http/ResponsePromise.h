#pragma once

#include <QObject>

namespace http
{

class Response;

class ResponsePromise: public QObject
{
    Q_OBJECT

    public:
        ResponsePromise(Response* response = 0);

        bool isReady() { return _response != 0; }

        void fullfill(Response* response);

        Response* response() { return _response; }

    signals:
        void ready();


    private:
        Response* _response;
};

}
