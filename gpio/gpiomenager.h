#ifndef GPIOMENAGER_H
#define GPIOMENAGER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QPair>
#include <QThread>
#include <QApplication>

#include "gpio/configgpio.h"

class GPIOMenager : public QObject
{
    Q_OBJECT
public:
    explicit GPIOMenager();

public slots:
    void buttonOperation(const QString &button);
    void getButtonsConfig(const ConfigGPIO& buttons);
    void configurePins(const QVector<int> &pins);

    void getButtonTime(int newTime);
    void getDelayTime(int newTime);

private:
    QScopedPointer<QProcess> m_process;
    QScopedPointer<ConfigGPIO> m_buttonsConfig{};
    QPair<int, int> m_pins;

    int m_pressedTime{0};
    int m_delayTime{0};

    bool test1{false};

private slots:
    void buttonPress(QPair<int, int> pins);
    void buttonRelease(QPair<int, int> pins);

    void pinOn(int pin);
    void pinOff(int pin);
    void executeCommand(const QStringList &arguments);

signals:
    void buttonsPressed();
    void sendError(QString);
};

#endif // GPIOMENAGER_H
