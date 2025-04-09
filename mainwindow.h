#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QScopedPointer>

#include "threads/threads.h"

#include "gpio/configgpio.h"

#include "file/directorycontent.h"

#include "guielements/configlistcombobox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

Q_DECLARE_METATYPE(ConfigGPIO)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showComment(const QString &information);
    void showMessageBox(const QString& information);
    void showStatus(const QString& status);

    QString getFileName();
    void getScenarioData(const QVector<QString> &scenarioData);

    void showProgressBar();

private:
    Ui::MainWindow *ui;
    QScopedPointer<Threads> m_threadsManager{};
    QScopedPointer<DirectoryContent> m_directory{};
    QScopedPointer<ConfigListComboBox> m_configComboBox{};

    QScopedPointer<ConfigGPIO> m_buttons{};
    QVector<QString> m_scenarioData{};

signals:
    void sendDecision(QMessageBox::StandardButton);
};
#endif // MAINWINDOW_H
