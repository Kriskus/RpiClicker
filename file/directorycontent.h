#ifndef DIRECTORYCONTENT_H
#define DIRECTORYCONTENT_H

#include <QObject>
#include <QDir>
#include <QApplication>

class DirectoryContent : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryContent(QObject *parent = nullptr);

public slots:
    QString getAppDirectory();
    QStringList getContent();

signals:
    void finished();
};

#endif // DIRECTORYCONTENT_H
