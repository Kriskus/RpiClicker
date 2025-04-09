#include "filereader.h"

FileReader::FileReader(const QString &fileName)
    : file_(new QFile())
{
    file_.data()->setFileName(fileName);
}

void FileReader::readAllData()
{
    if(file_.data()->open(QIODevice::ReadOnly)) {
        QString fileData{};
        fileData.push_back(file_.data()->readAll());
        file_.data()->close();
        emit sendFileDataAll(fileData);
    } else {
        emit sendFileError(file_.data()->errorString());
    }
    emit finished();
}

void FileReader::readByLine()
{
    if(file_.data()->open(QIODevice::ReadOnly)) {
        QVector<QString> fileData{};
        while(!file_.data()->atEnd()) {
            fileData.push_back(file_.data()->readLine().replace("\r\n",""));
        }
        file_.data()->close();
        emit sendFileDataLines(fileData);
    } else {
        emit sendFileError(file_.data()->errorString());
    }
    emit finished();
}
