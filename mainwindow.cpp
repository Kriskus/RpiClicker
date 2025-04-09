#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_threadsManager(new Threads)
    , m_configComboBox(new ConfigListComboBox)
{
    ui->setupUi(this);

    qRegisterMetaType<QVector<QString>>();
    qRegisterMetaType<ConfigGPIO>();
    ui->pushButtonStartScenario->show();
    ui->pushButtonEndScenario->hide();

    connect(m_threadsManager.data(), &Threads::sendScenarioData, this, &MainWindow::getScenarioData);
    connect(m_threadsManager.data(), &Threads::sendStatusInformation, this, &MainWindow::showStatus);
    connect(m_threadsManager.data(), &Threads::sendQuestion, this, &MainWindow::showMessageBox);
    connect(m_threadsManager.data(), &Threads::sendComment, this, &MainWindow::showComment);
    connect(m_threadsManager.data(), &Threads::updateProgressBar, this, &MainWindow::showProgressBar);

    connect(this, &MainWindow::sendDecision, m_threadsManager.data(), &Threads::getDecision);

    connect(ui->pushButtonReadScenario, &QPushButton::clicked, [this]() {
        m_threadsManager->readFileScenario(getFileName());
    });

    connect(ui->pushButtonEndScenario, &QPushButton::clicked, m_threadsManager.data(), &Threads::endThread, Qt::DirectConnection);
    connect(ui->pushButtonStartScenario, &QPushButton::clicked, [this]() {
        if(m_configComboBox->currentText().contains("Wybierz")) {
            return;
        }
        if(m_scenarioData.size() < 1) {
            return;
        }
        ui->progressBar->setValue(0);
        ui->textBrowser->clear();
        m_threadsManager->startScenario(m_scenarioData, ui->spinBoxRepeat->value(), {});
        ui->pushButtonStartScenario->hide();
        ui->pushButtonEndScenario->show();
    });

    connect(m_threadsManager.data(), &Threads::finished, [this]() {
        ui->pushButtonStartScenario->show();
        ui->pushButtonEndScenario->hide();
        ui->progressBar->setValue(0);
    });

    connect(m_configComboBox.data(), &ConfigListComboBox::sendCurrentConfig, m_threadsManager.data(), &Threads::readFileConfigGPIO);
    ui->horizontalLayoutMenu->insertWidget(0, &*m_configComboBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatus(const QString &status)
{
    ui->statusbar->showMessage(status, 5000);
}

QString MainWindow::getFileName()
{
    return QFileDialog::getOpenFileName(this, "Wybierz plik");
}

void MainWindow::showComment(const QString &information)
{
    ui->textBrowser->append(information);
}

void MainWindow::showMessageBox(const QString &information)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, "Info", information, QMessageBox::Yes | QMessageBox::No);
    emit sendDecision(reply);
}

void MainWindow::getScenarioData(const QVector<QString>& scenarioData)
{
    m_scenarioData = scenarioData;
}

void MainWindow::showProgressBar()
{
    if(ui->progressBar->invertedAppearance()) {
        ui->progressBar->setValue(ui->progressBar->value() - 1);
        if(ui->progressBar->value() == 0) {
            ui->progressBar->setInvertedAppearance(0);
        }
    } else {
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        if(ui->progressBar->value() == 1000) {
            ui->progressBar->setInvertedAppearance(1);
        }
    }
}
