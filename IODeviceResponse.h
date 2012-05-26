#pragma once

#include "HttpResponse.h"

class QIODevice;
class QTcpSocket;


class IODeviceResponse: public HttpResponse
{
    Q_OBJECT

    public:
        IODeviceResponse(QIODevice* io);
        ~IODeviceResponse();

        virtual void abort();


    protected:
        void sendContent(QTcpSocket* socket);


    private slots:
        void onBytesWritten(qint64 count);


    private:
        QTcpSocket* _socket;
        QIODevice* _io;


        void sendChunk(int size);
        void close();
};
