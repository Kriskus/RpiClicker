#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QObject>
#include <QScopedPointer>

#include "./gpio/configgpio.h"

class ConfigurationParser : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationParser();

public slots:
    void prepareData(const QString& data);

private:
    QScopedPointer<ConfigGPIO> buttons_;

    void processError(const QString& error);
    void createButtonElement(const QString& buttonName, const QJsonObject& gpioObject);

    bool checkDocument(const QJsonDocument& document);
    bool checkButtonsExist(const QJsonObject& rootObject);
    bool checkButton(const QJsonObject& buttonObject);

signals:
    void sendConfiguration(ConfigGPIO);
    void sendFileError(const QString& errorString);
    void finished();
};

#endif // CONFIGURATIONPARSER_H
