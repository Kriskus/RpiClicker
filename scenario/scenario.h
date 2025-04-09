#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QThread>
#include <QStringLiteral>
#include <QMutex>
#include <QWaitCondition>
#include <QApplication>
#include <QDateTime>
#include <QDebug>

#include "buttonoperation.h"
#include "commentoperation.h"
#include "loopoperation.h"
#include "operation.h"
#include "sleepoperation.h"
#include "timeoperation.h"
#include "userreactionoperation.h"

class Scenario : public QObject
{
    Q_OBJECT
public:
    explicit Scenario(const QVector<QString> &scenario = {}, int repeat = 0, const QTime &endTime = {});
    ~Scenario();

public slots:
    void startScenario();
    void startLoopRepeat();
    void startLoopTime();

    void pauseScenario();
    void resumeScenario();
    void endScenario();

private:
    int m_currentLine{0};
    int m_buttonPressingTime{150};
    int m_actionDelayTime{200};

    QVector<QString> m_scenario{};
    int m_repeat{0};
    QTime m_endTime{};
    bool m_threadRunning{true};

    bool m_threadPaused{false};
    QMutex m_sync;
    QWaitCondition m_pauseCond;

    QScopedPointer<ButtonOperation> m_button;
    QScopedPointer<CommentOperation> m_comment;
    QScopedPointer<LoopOperation> m_loop;
    QScopedPointer<SleepOperation> m_sleep;
    QScopedPointer<TimeOperation> m_time;
    QScopedPointer<UserReactionOperation> m_userReaction;

private slots:
    void setButtonPressingTime(int newTime);
    void setDelayActionTime(int newTime);

signals:
    void sendCommentText(QString);
    void sendReactionText(QString);
    void makeLoopThreadRepeat(const QVector<QString>&, int, int, int);
    void makeLoopThreadTime(const QVector<QString>&, const QTime&, int, int);
    void sendButtons(const QString&);
    void sendTimeButton(int);
    void sendTimeDelay(int);
    void pauseThread();
    void loopFinished();
    void stopButtons();
    void finished();
};

