#include <QCoreApplication>

#include "HttpServer.h"
#include "SimpleResource.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    HttpServer server;
    server.addResource(
        new SimpleResource("/", "<h1>Hello, World!</h1>")
    );

    return app.exec();
}
