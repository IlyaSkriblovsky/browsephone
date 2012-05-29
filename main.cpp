#include <QCoreApplication>

#include "http/Server.h"
#include "http/SimpleResource.h"
#include "http/StaticDirResource.h"

#include "GalleryListResource.h"
#include "ItemContentResource.h"
#include "ItemThumbnailResource.h"
#include "ContactsResource.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    http::Server server;
    server.addResource(new http::SimpleResource("/", "<h1>Hello, World!</h1>"));
    server.addResource(new http::StaticDirResource("/static/", "/opt/browsephone/static"));

    server.addResource(new ItemContentResource("/content/"));
    server.addResource(new ItemThumbnailResource("/thumb/"));

    server.addResource(new ContactsResource("/contacts"));

    QStringList imageProperties;
    imageProperties
        << QDocumentGallery::url
        << QDocumentGallery::title
        << QDocumentGallery::width
        << QDocumentGallery::height
        << QDocumentGallery::fileSize
        << QDocumentGallery::fileExtension
        << QDocumentGallery::mimeType
    ;
    server.addResource(new GalleryListResource("/images", QDocumentGallery::Image, imageProperties));


    QStringList audioProperties;
    audioProperties
        << QDocumentGallery::url
        << QDocumentGallery::title
        << QDocumentGallery::artist
        << QDocumentGallery::audioBitRate
        << QDocumentGallery::audioCodec
        << QDocumentGallery::genre
        << QDocumentGallery::duration
        << QDocumentGallery::performer
        << QDocumentGallery::sampleRate
        << QDocumentGallery::fileSize
        << QDocumentGallery::fileExtension
        << QDocumentGallery::mimeType
    ;
    server.addResource(new GalleryListResource("/audios", QDocumentGallery::Audio, audioProperties));


    QStringList videoProperties;
    videoProperties
        << QDocumentGallery::url
        << QDocumentGallery::title
        << QDocumentGallery::width
        << QDocumentGallery::height
        << QDocumentGallery::duration
        << QDocumentGallery::performer
        << QDocumentGallery::artist
        << QDocumentGallery::fileSize
        << QDocumentGallery::fileExtension
        << QDocumentGallery::mimeType
    ;
    server.addResource(new GalleryListResource("/videos", QDocumentGallery::Video, videoProperties));


    QStringList docProperties;
    docProperties
        << QDocumentGallery::url
        << QDocumentGallery::title
        << QDocumentGallery::pageCount
        << QDocumentGallery::wordCount
        << QDocumentGallery::fileSize
        << QDocumentGallery::fileExtension
        << QDocumentGallery::mimeType
    ;
    server.addResource(new GalleryListResource("/docs", QDocumentGallery::Document, docProperties));

    QStringList fileProperties;
    fileProperties
        << QDocumentGallery::url
        << QDocumentGallery::title
        << QDocumentGallery::fileSize
        << QDocumentGallery::fileExtension
        << QDocumentGallery::mimeType
    ;
    server.addResource(new GalleryListResource("/files", QDocumentGallery::File, fileProperties));

    return app.exec();
}
