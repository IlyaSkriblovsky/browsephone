#include <QCoreApplication>

#include "HttpServer.h"
#include "SimpleResource.h"
#include "TestResource.h"
#include "StaticDirResource.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    HttpServer server;
    server.addResource(new SimpleResource("/", "<h1>Hello, World!</h1>"));
    server.addResource(new TestResource("/test"));
    server.addResource(new StaticDirResource("/f/", "."));

    return app.exec();
}
