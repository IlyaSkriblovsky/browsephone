#include <QCoreApplication>

#include "http/Server.h"
#include "http/SimpleResource.h"
#include "http/StaticDirResource.h"

#include "TestResource.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    http::Server server;
    server.addResource(new http::SimpleResource("/", "<h1>Hello, World!</h1>"));
    server.addResource(new http::StaticDirResource("/f/", "."));
    server.addResource(new TestResource("/test"));

    return app.exec();
}
