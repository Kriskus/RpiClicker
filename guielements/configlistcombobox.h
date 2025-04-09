#ifndef CONFIGLISTCOMBOBOX_H
#define CONFIGLISTCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QScopedPointer>

#include "./file/directorycontent.h"

class ConfigListComboBox : public QComboBox
{
    Q_OBJECT
public:
    ConfigListComboBox();

public slots:
    void setConfigDirectory(const QString& directory);
    void getFilePath(const QString &currentText);

    void loadConfiguration(const QStringList& configs);

private:
    QScopedPointer<DirectoryContent> m_directory{};

    QFont settings();

    QString m_directoryPath{};

signals:
    void sendCurrentConfig(QString);
};

#endif // CONFIGLISTCOMBOBOX_H
