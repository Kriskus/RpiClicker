#include "directorycontent.h"

DirectoryContent::DirectoryContent(QObject *parent)
    : QObject{parent}
{

}

QString DirectoryContent::getAppDirectory()
{
    return {QDir::currentPath() + "/data/"};
}

QStringList DirectoryContent::getContent()
{
    QDir directory(getAppDirectory());
    QStringList files = directory.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);

    return files;
}
