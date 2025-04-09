#ifndef CONFIGGPIO_H
#define CONFIGGPIO_H

#include <QMap>
#include <QString>
#include <QPair>

class ConfigGPIO
{
public:
    ConfigGPIO();

    void addButton(const QString& name, QPair<int, int> pins);
    QPair<int, int> checkButton(const QString& buttonName);

    void clearButtons();

private:
    QMap<QString, QPair<int, int>> buttons_{};
};

#endif // CONFIGGPIO_H
