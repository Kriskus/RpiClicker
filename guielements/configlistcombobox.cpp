#include "configlistcombobox.h"

#include <QFont>

ConfigListComboBox::ConfigListComboBox()
    : m_directory(new DirectoryContent)
{
    setMinimumWidth(350);
    setFont(settings());
    addItem("Wybierz typ kasy");
    setConfigDirectory(m_directory->getAppDirectory());
    loadConfiguration(m_directory->getContent());

    connect(this, &ConfigListComboBox::currentTextChanged, this, &ConfigListComboBox::getFilePath);
}

void ConfigListComboBox::setConfigDirectory(const QString &directory)
{
    m_directoryPath = directory;
}

void ConfigListComboBox::getFilePath(const QString& currentText)
{
    emit sendCurrentConfig(m_directoryPath + currentText);
}

void ConfigListComboBox::loadConfiguration(const QStringList &configs)
{
    addItems(configs);
}

QFont ConfigListComboBox::settings()
{
    QFont font;
    font.setPointSize(12);
    return font;
}
