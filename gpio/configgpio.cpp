#include "configgpio.h"

ConfigGPIO::ConfigGPIO()
{

}

void ConfigGPIO::addButton(const QString &name, QPair<int, int> pins)
{
    buttons_.insert(name, pins);
}

QPair<int, int> ConfigGPIO::checkButton(const QString &buttonName)
{
    if (buttons_.contains(buttonName)) {
        return buttons_.value(buttonName);
    }
    return {0, 0};
}


void ConfigGPIO::clearButtons()
{
    buttons_.clear();
}
