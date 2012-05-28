#include <QCoreApplication>

#include "http/Server.h"
#include "http/SimpleResource.h"
#include "http/StaticDirResource.h"

#include "TestResource.h"
#include "DeferredResource.h"
#include "ImagesResource.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    http::Server server;
    server.addResource(new http::SimpleResource("/", "<h1>Hello, World!</h1>"));
    server.addResource(new http::StaticDirResource("/f/", "."));
    server.addResource(new TestResource("/test"));
    server.addResource(new DeferredResource("/5"));

    server.addResource(new ImagesResource);

    return app.exec();
}
