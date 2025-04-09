#include "configurationparser.h"
#include <QDebug>

ConfigurationParser::ConfigurationParser()
    : buttons_(new ConfigGPIO())
{}

void ConfigurationParser::prepareData(const QString& data)
{
    QJsonDocument document = QJsonDocument::fromJson(data.toLatin1());

    if (!checkDocument(document)) {
        return;
    }

    QJsonObject rootObject = document.object();
    if (!checkButtonsExist(rootObject)) {
        return;
    }

    QJsonArray buttonsArray = rootObject["buttons"].toArray();
    for (const QJsonValue& buttonValue : buttonsArray) {
        if (!buttonValue.isObject()) {
            processError("Error: 'buttons' array contains a non-object element.");
            return;
        }

        QJsonObject buttonObject = buttonValue.toObject();
        if (!checkButton(buttonObject)) {
            return;
        }

        QString buttonName = buttonObject["name"].toString();
        QJsonObject gpioObject = buttonObject["gpio"].toObject();
        createButtonElement(buttonName, gpioObject);
    }
    emit sendConfiguration(*buttons_);
    emit finished();
}

void ConfigurationParser::processError(const QString& error)
{
    emit sendFileError(error);
    emit finished();
}

void ConfigurationParser::createButtonElement(const QString& buttonName, const QJsonObject& gpioObject)
{
    int pin1 = gpioObject["PIN1"].toInt();
    int pin2 = gpioObject["PIN2"].toInt();
    buttons_->addButton(buttonName, {pin1, pin2});
}

bool ConfigurationParser::checkDocument(const QJsonDocument& document)
{
    if (document.isNull() || document.isEmpty()) {
        processError("Error parsing JSON document: Document is null or empty.");
        return false;
    }
    return true;
}

bool ConfigurationParser::checkButtonsExist(const QJsonObject& rootObject)
{
    if (!rootObject.contains("buttons") || !rootObject["buttons"].isArray()) {
        processError("Error: 'buttons' array not found or invalid.");
        return false;
    }
    return true;
}

bool ConfigurationParser::checkButton(const QJsonObject& buttonObject)
{
    if (!buttonObject.contains("name") || !buttonObject["name"].isString()) {
        processError("Error: 'nazwa' not found or invalid.");
        return false;
    }

    if (!buttonObject.contains("gpio") || !buttonObject["gpio"].isObject()) {
        processError("Error: 'gpio' object not found or invalid.");
        return false;
    }

    QJsonObject gpioObject = buttonObject["gpio"].toObject();
    if (!gpioObject.contains("PIN1") || !gpioObject.contains("PIN2")) {
        processError("Error: 'gpio' object missing 'PIN1' or 'PIN2'.");
        return false;
    }
    return true;
}
