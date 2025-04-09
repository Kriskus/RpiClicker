#include "gpiomenager.h"

GPIOMenager::GPIOMenager()
    : m_process(new QProcess)
{
    m_process->moveToThread(QApplication::instance()->thread()); // Ensure m_process lives in the main thread
}

void GPIOMenager::buttonOperation(const QString &button)
{
    m_pins = m_buttonsConfig->checkButton(button);
    buttonPress(m_pins);
    QThread::msleep(m_pressedTime);
    buttonRelease(m_pins);
    QThread::msleep(m_delayTime);
}

void GPIOMenager::buttonPress(QPair<int, int> pins)
{
    pinOn(pins.first);
    pinOn(pins.second);
}

void GPIOMenager::buttonRelease(QPair<int, int> pins)
{
    pinOff(pins.first);
    pinOff(pins.second);
}

void GPIOMenager::configurePins(const QVector<int> &pins)
{
    for(auto &pin: pins) {
        QStringList arguments;
        arguments << "set" << QString::number(pin) << "pu";
        executeCommand(arguments);
    }
}

void GPIOMenager::pinOn(int pin)
{
    QStringList arguments;
    arguments << "set" << QString::number(pin) << "op";
    executeCommand(arguments);
}

void GPIOMenager::pinOff(int pin)
{
    QStringList arguments;
    arguments << "set" << QString::number(pin) << "ip";
    executeCommand(arguments);
}

void GPIOMenager::executeCommand(const QStringList &arguments)
{
    QMetaObject::invokeMethod(&*m_process, [this, arguments]() {
        m_process->start("pinctrl", arguments);
        if (!m_process->waitForFinished(3000)) {
            qDebug() << ("Process timeout or error:" + m_process->errorString());
            qDebug() << ("Standard output:" + m_process->readAllStandardOutput());
            qDebug() << ("Standard error:" + m_process->readAllStandardError());
            emit sendError("Process timeout or error:" + m_process->errorString());
            emit sendError("Standard output:" + m_process->readAllStandardOutput());
            emit sendError("Standard error:" + m_process->readAllStandardError());
        }
    }, Qt::QueuedConnection);
}

void GPIOMenager::getButtonsConfig(const ConfigGPIO &buttons)
{
    m_buttonsConfig.reset(new ConfigGPIO(buttons));
}

void GPIOMenager::getButtonTime(int newTime)
{
    m_pressedTime = newTime;
}

void GPIOMenager::getDelayTime(int newTime)
{
    m_delayTime = newTime;
}
