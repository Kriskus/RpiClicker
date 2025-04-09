#ifndef THREADS_H
#define THREADS_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QMessageBox>

#include "file/filereader.h"
#include "file/configurationparser.h"
#include "scenario/scenario.h"

#include "gpio/configgpio.h"
#include "gpio/gpiomenager.h"

class Threads : public QObject
{
    Q_OBJECT
public:
    explicit Threads(QObject *parent = nullptr);

public slots:
    void startScenario(const QVector<QString>& scenarioData, int repeat, const QTime& endTime);
    void startLoopRepeat(const QVector<QString>& loopData, int repeat, int buttonPressedTime, int actionDelayTime);
    void startLoopTime(const QVector<QString>& loopData, const QTime& endTime, int buttonPressedTime, int actionDelayTime);

    void readFileScenario(const QString& fileName);
    void readFileConfigGPIO(const QString& fileName);

    void getConfigurationGPIO(const ConfigGPIO& buttons);
    void getDecision(const QMessageBox::StandardButton& reply);
    void resumeThread();
    void endThread();

private:
    bool m_loopStarted{false};
    QScopedPointer<ConfigGPIO> m_buttons{};
    QScopedPointer<QTimer> m_progressBarTimer{};

private slots:
    void test();
    void progressBarStart();

signals:
    void sendStatusInformation(const QString&);
    void sendQuestion(const QString&);
    void sendComment(const QString&);
    void sendScenarioData(const QVector<QString>&);
    void sendConfigGPIO(const ConfigGPIO&);
    void sendButtons(const QString&);

    void updateProgressBar();

    void resumeScenario();
    void resumeLoop();
    void endScenario();
    void finished();
};

#endif // THREADS_H
