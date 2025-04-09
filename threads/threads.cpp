#include "threads.h"

Threads::Threads(QObject *parent)
    : QObject{parent}
    , m_progressBarTimer(new QTimer)
{
    connect(m_progressBarTimer.data(), &QTimer::timeout, this, &Threads::updateProgressBar);
}

void Threads::startScenario(const QVector<QString> &scenarioData, int repeat, const QTime &endTime)
{
    progressBarStart();
    QThread* scenarioThread = new QThread(this);
    Scenario* scenario = new Scenario(scenarioData, repeat, endTime);
    GPIOMenager* gpio = new GPIOMenager();

    scenario->moveToThread(scenarioThread);
    gpio->moveToThread(scenarioThread);
    gpio->getButtonsConfig(*m_buttons);
    gpio->configurePins({2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27});

    connect(scenarioThread, &QThread::started, scenario, static_cast<void (Scenario::*)()>(&Scenario::startScenario));
    connect(scenario, &Scenario::sendCommentText, this, &Threads::sendComment);
    connect(scenario, &Scenario::sendReactionText, this, &Threads::sendQuestion);
    connect(scenario, &Scenario::sendButtons, gpio, &GPIOMenager::buttonOperation);
    connect(scenario, &Scenario::sendTimeButton, gpio, &GPIOMenager::getButtonTime);
    connect(scenario, &Scenario::sendTimeDelay, gpio, &GPIOMenager::getDelayTime);

    connect(gpio, &GPIOMenager::sendError, this, &Threads::sendStatusInformation);

    connect(scenario, &Scenario::makeLoopThreadRepeat, this, [this](const QVector<QString>& loopData, int repeat, int buttonPressedTime, int actionDelayTime) {
        QMetaObject::invokeMethod(this, [this, loopData, repeat, buttonPressedTime, actionDelayTime]() {
            startLoopRepeat(loopData, repeat, buttonPressedTime, actionDelayTime);
        }, Qt::QueuedConnection);
    });

    connect(scenario, &Scenario::makeLoopThreadTime, this, [this](const QVector<QString>& loopData, const QTime& endTime, int buttonPressedTime, int actionDelayTime) {
        QMetaObject::invokeMethod(this, [this, loopData, endTime, buttonPressedTime, actionDelayTime]() {
            startLoopTime(loopData, endTime, buttonPressedTime, actionDelayTime);
        }, Qt::QueuedConnection);
    });

    connect(this, &Threads::resumeScenario, scenario, &Scenario::resumeScenario, Qt::DirectConnection);
    connect(this, &Threads::endScenario, scenario, &Scenario::endScenario, Qt::DirectConnection);
    connect(scenario, &Scenario::finished, scenarioThread, &QThread::quit);
    connect(scenario, &Scenario::finished, this, &Threads::finished);
    connect(scenario, &Scenario::finished, m_progressBarTimer.data(), &QTimer::stop);
    connect(scenarioThread, &QThread::finished, scenarioThread, &QThread::deleteLater);
    connect(scenario, &Scenario::finished, scenario, &Scenario::deleteLater);

    scenarioThread->start();
}

void Threads::startLoopRepeat(const QVector<QString> &loopData, int repeat, int buttonPressedTime, int actionDelayTime)
{
    QMetaObject::invokeMethod(this, [this, loopData, repeat, buttonPressedTime, actionDelayTime]() {
        m_loopStarted = true;
        QThread* loopThread = new QThread(this);
        Scenario* scenario = new Scenario(loopData, repeat);
        GPIOMenager* gpio = new GPIOMenager();

        scenario->moveToThread(loopThread);
        gpio->moveToThread(loopThread);
        gpio->getButtonsConfig(*m_buttons);
        gpio->getButtonTime(buttonPressedTime);
        gpio->getDelayTime(actionDelayTime);

        connect(gpio, &GPIOMenager::sendError, this, &Threads::sendStatusInformation);

        connect(loopThread, &QThread::started, scenario, static_cast<void (Scenario::*)()>(&Scenario::startLoopRepeat));
        connect(scenario, &Scenario::sendCommentText, this, &Threads::sendComment);
        connect(scenario, &Scenario::sendReactionText, this, &Threads::sendQuestion);
        connect(scenario, &Scenario::sendButtons, gpio, &GPIOMenager::buttonOperation);
        connect(scenario, &Scenario::sendTimeButton, gpio, &GPIOMenager::getButtonTime);
        connect(scenario, &Scenario::sendTimeDelay, gpio, &GPIOMenager::getDelayTime);

        connect(this, &Threads::resumeLoop, scenario, &Scenario::resumeScenario, Qt::DirectConnection);
        connect(this, &Threads::endScenario, scenario, &Scenario::endScenario, Qt::DirectConnection);

        connect(scenario, &Scenario::loopFinished, loopThread, &QThread::quit);
        connect(loopThread, &QThread::finished, loopThread, &QThread::deleteLater);
        connect(scenario, &Scenario::loopFinished, scenario, &Scenario::deleteLater);
        connect(scenario, &Scenario::loopFinished, this, &Threads::resumeScenario);
        connect(scenario, &Scenario::loopFinished, [this]() {
            m_loopStarted = false;
            emit sendStatusInformation("Zakończenie pętli");
        });

        loopThread->start();
    });
}

void Threads::startLoopTime(const QVector<QString> &loopData, const QTime &endTime, int buttonPressedTime, int actionDelayTime)
{
    QMetaObject::invokeMethod(this, [this, loopData, endTime, buttonPressedTime, actionDelayTime]() {
        QThread* loopThread = new QThread(this);
        Scenario* scenario = new Scenario(loopData, 0, endTime);
        GPIOMenager* gpio = new GPIOMenager();

        scenario->moveToThread(loopThread);
        gpio->moveToThread(loopThread);
        gpio->getButtonsConfig(*m_buttons);
        gpio->getButtonTime(buttonPressedTime);
        gpio->getDelayTime(actionDelayTime);

        connect(loopThread, &QThread::started, scenario, static_cast<void (Scenario::*)()>(&Scenario::startLoopTime));
        connect(scenario, &Scenario::sendCommentText, this, &Threads::sendComment);
        connect(scenario, &Scenario::sendReactionText, this, &Threads::sendQuestion);
        connect(scenario, &Scenario::sendButtons, gpio, &GPIOMenager::buttonOperation);
        connect(scenario, &Scenario::sendTimeButton, gpio, &GPIOMenager::getButtonTime);
        connect(scenario, &Scenario::sendTimeDelay, gpio, &GPIOMenager::getDelayTime);

        connect(gpio, &GPIOMenager::sendError, this, &Threads::sendStatusInformation);

        connect(this, &Threads::resumeLoop, scenario, &Scenario::resumeScenario, Qt::DirectConnection);
        connect(this, &Threads::endScenario, scenario, &Scenario::endScenario, Qt::DirectConnection);

        connect(scenario, &Scenario::loopFinished, loopThread, &QThread::quit);
        connect(loopThread, &QThread::finished, loopThread, &QThread::deleteLater);
        connect(scenario, &Scenario::loopFinished, scenario, &Scenario::deleteLater);
        connect(scenario, &Scenario::loopFinished, this, &Threads::resumeScenario);
        connect(scenario, &Scenario::loopFinished, [this]() {
            m_loopStarted = false;
            emit sendStatusInformation("Zakończenie pętli...");
        });

        loopThread->start();
    });
}

void Threads::readFileScenario(const QString &fileName)
{
    QThread* readThread = new QThread();
    FileReader* fileRead = new FileReader(fileName);

    fileRead->moveToThread(readThread);

    connect(readThread, &QThread::started, fileRead, &FileReader::readByLine);
    connect(fileRead, &FileReader::sendFileError, this, &Threads::sendStatusInformation);
    connect(fileRead, &FileReader::sendFileDataLines, this, &Threads::sendScenarioData);

    connect(fileRead, &FileReader::finished, readThread, &QThread::quit);
    connect(readThread, &QThread::finished, readThread, &QThread::deleteLater);
    connect(fileRead, &FileReader::finished, fileRead, &FileReader::deleteLater);
    connect(fileRead, &FileReader::finished, [this]() {
        emit sendStatusInformation("Zakończono odczyt pliku...");
    });

    readThread->start();
}

void Threads::readFileConfigGPIO(const QString &fileName)
{
    if(fileName.contains("Wybierz typ kasy")) {
        return;
    }
    QThread* readThread = new QThread();
    FileReader* fileRead = new FileReader(fileName);
    ConfigurationParser* config = new ConfigurationParser();

    fileRead->moveToThread(readThread);
    config->moveToThread(readThread);

    connect(readThread, &QThread::started, fileRead, &FileReader::readAllData);
    connect(fileRead, &FileReader::sendFileError, this, &Threads::sendStatusInformation);
    connect(config, &ConfigurationParser::sendFileError, this, &Threads::sendStatusInformation);
    connect(fileRead, &FileReader::sendFileDataAll, config, &ConfigurationParser::prepareData);
    connect(config, &ConfigurationParser::sendConfiguration, this, &Threads::getConfigurationGPIO);

    connect(fileRead, &FileReader::finished, readThread, &QThread::quit);
    connect(readThread, &QThread::finished, readThread, &QThread::deleteLater);
    connect(fileRead, &FileReader::finished, fileRead, &FileReader::deleteLater);
    connect(fileRead, &FileReader::finished, config, &ConfigurationParser::deleteLater);
    connect(fileRead, &FileReader::finished, [this]() {
        emit sendStatusInformation("Zakończono odczyt pliku...");
    });

    readThread->start();
}

void Threads::getConfigurationGPIO(const ConfigGPIO &buttons)
{
    m_buttons.reset(new ConfigGPIO(buttons));
}

void Threads::getDecision(const QMessageBox::StandardButton &reply)
{
    if(reply == QMessageBox::Yes) {
        resumeThread();
    } else {
        endThread();
    }
}

void Threads::resumeThread()
{
    if(!m_loopStarted) {
        emit resumeScenario();
    } else {
        emit resumeLoop();
    }
}

void Threads::endThread()
{
    emit endScenario();
}

void Threads::test()
{
    emit finished();
}

void Threads::progressBarStart()
{
    m_progressBarTimer->start(1);
}
