#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QScopedPointer>

class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(const QString& fileName = QString());

public slots:
    void readAllData();
    void readByLine();

private:
    QScopedPointer<QFile> file_{};
    QString fileDataAll_{};
    QVector<QString> fileDataLines_{};

signals:
    void sendFileError(QString);
    void sendFileDataLines(QVector<QString>);
    void sendFileDataAll(QString);
    void finished();
};

#endif // FILEREADER_H
