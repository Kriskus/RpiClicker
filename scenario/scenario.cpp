#include "scenario.h"

Scenario::Scenario(const QVector<QString>& scenario, int repeat, const QTime& endTime)
    : m_scenario(scenario)
    , m_repeat(repeat)
    , m_endTime(endTime)
    , m_button(new ButtonOperation)
    , m_comment(new CommentOperation)
    , m_loop(new LoopOperation)
    , m_sleep(new SleepOperation)
    , m_time(new TimeOperation)
    , m_userReaction(new UserReactionOperation)
{
    connect(m_button.data(), &ButtonOperation::sendButtons, this, &Scenario::sendButtons);
    connect(this, &Scenario::stopButtons, m_button.data(), &ButtonOperation::breakButtons);
    connect(m_comment.data(), &CommentOperation::sendComment, this, &Scenario::sendCommentText);
    connect(m_loop.data(), &LoopOperation::sendLoopDataRepeat, this, &Scenario::makeLoopThreadRepeat);
    connect(m_loop.data(), &LoopOperation::sendLoopDataTime, this, &Scenario::makeLoopThreadTime);
    connect(m_loop.data(), &LoopOperation::sendLoopDataRepeat, this, &Scenario::pauseScenario);
    connect(m_loop.data(), &LoopOperation::sendLoopDataTime, this, &Scenario::pauseScenario);
    connect(m_time.data(), &TimeOperation::sendButtonTime, this, &Scenario::sendTimeButton);
    connect(m_time.data(), &TimeOperation::sendActionTime, this, &Scenario::sendTimeDelay);
    connect(m_time.data(), &TimeOperation::sendButtonTime, this, &Scenario::setButtonPressingTime);
    connect(m_time.data(), &TimeOperation::sendActionTime, this, &Scenario::setDelayActionTime);
    connect(m_time.data(), &TimeOperation::sendErrorQuestion, this, &Scenario::sendReactionText);
    connect(m_userReaction.data(), &UserReactionOperation::sendReaction, this, &Scenario::sendReactionText);
    connect(m_userReaction.data(), &UserReactionOperation::sendReaction, this, &Scenario::pauseScenario);
}

Scenario::~Scenario()
{
    endScenario();
}

void Scenario::startScenario()
{
    for(int rep = 0; rep < m_repeat; rep++) {
        for(m_currentLine = 0; m_currentLine < m_scenario.size(); m_currentLine++) {
            if(m_comment->checkOperation(m_scenario[m_currentLine])) {
                m_comment->execute();
            } else if(m_time->checkOperation(m_scenario[m_currentLine])) {
                m_time->prepareOperation(m_scenario[m_currentLine]);
            } else if(m_userReaction->checkOperation(m_scenario[m_currentLine])) {
                m_userReaction->execute();
            } else if(m_sleep->checkOperation(m_scenario[m_currentLine])) {
                m_sleep->execute();
            } else if(m_loop->checkOperation(m_scenario[m_currentLine])) {
                m_currentLine = m_loop->prepareOperationData(m_scenario, m_currentLine, m_buttonPressingTime, m_actionDelayTime);
                m_loop->execute();
            } else {
                if(m_button->checkOperation(m_scenario[m_currentLine])) {
                    m_button->prepareOperation(m_scenario[m_currentLine]);
                    m_button->execute();
                }
            }
            m_sync.lock();
            if(m_threadPaused) {
                m_pauseCond.wait(&m_sync);
            }
            m_sync.unlock();
            if(!m_threadRunning) {
                emit finished();
                return;
            }
        }
    }
    emit finished();
}

void Scenario::startLoopRepeat()
{
    for(int rep = 0; rep < m_repeat; rep++) {
        emit sendCommentText("Pętla " + QString::number(rep+1) + " z " + QString::number(m_repeat) );
        for(m_currentLine = 0; m_currentLine < m_scenario.size(); m_currentLine++) {
            if(m_comment->checkOperation(m_scenario[m_currentLine])) {
                m_comment->execute();
            } else if(m_time->checkOperation(m_scenario[m_currentLine])) {
                m_time->prepareOperation(m_scenario[m_currentLine]);
            } else if(m_userReaction->checkOperation(m_scenario[m_currentLine])) {
                m_userReaction->execute();
            } else if(m_sleep->checkOperation(m_scenario[m_currentLine])) {
                m_sleep->execute();
            } else if(m_loop->checkOperation(m_scenario[m_currentLine])) {
                m_currentLine = m_loop->prepareOperationData(m_scenario, m_currentLine, m_buttonPressingTime, m_actionDelayTime);
                m_loop->execute();
            } else {
                if(m_button->checkOperation(m_scenario[m_currentLine])) {
                    m_button->prepareOperation(m_scenario[m_currentLine]);
                    m_button->execute();
                }
            }
            m_sync.lock();
            if(m_threadPaused) {
                m_pauseCond.wait(&m_sync);
            }
            m_sync.unlock();
            if(!m_threadRunning) {
                emit finished();
                return;
            }
        }
    }
    emit loopFinished();
}

void Scenario::startLoopTime()
{
    int repeat = 0;
    while(QTime::currentTime() < m_endTime) {
        emit sendCommentText("Pętla " + QString::number(++repeat));
        for(m_currentLine = 0; m_currentLine < m_scenario.size(); m_currentLine++) {
            if(m_comment->checkOperation(m_scenario[m_currentLine])) {
                m_comment->execute();
            } else if(m_time->checkOperation(m_scenario[m_currentLine])) {
                m_time->prepareOperation(m_scenario[m_currentLine]);
            } else if(m_userReaction->checkOperation(m_scenario[m_currentLine])) {
                m_userReaction->execute();
            } else if(m_sleep->checkOperation(m_scenario[m_currentLine])) {
                m_sleep->execute();
            } else if(m_loop->checkOperation(m_scenario[m_currentLine])) {
                m_currentLine = m_loop->prepareOperationData(m_scenario, m_currentLine, m_buttonPressingTime, m_actionDelayTime);
                m_loop->execute();
            } else {
                if(m_button->checkOperation(m_scenario[m_currentLine])) {
                    m_button->prepareOperation(m_scenario[m_currentLine]);
                    m_button->execute();
                }
            }
            m_sync.lock();
            if(m_threadPaused) {
                m_pauseCond.wait(&m_sync);
            }
            m_sync.unlock();
            if(!m_threadRunning) {
                emit finished();
                return;
            }
        }
    }
    emit loopFinished();
}

void Scenario::pauseScenario()
{
    m_sync.lock();
    m_threadPaused = true;
    m_sync.unlock();
}

void Scenario::resumeScenario()
{
    m_sync.lock();
    m_threadPaused = false;
    m_sync.unlock();
    m_pauseCond.wakeAll();
}

void Scenario::endScenario()
{
    m_sync.lock();
    m_threadPaused = false;
    m_threadRunning = false;
    emit stopButtons();
    m_sync.unlock();
    m_pauseCond.wakeAll();
}

void Scenario::setButtonPressingTime(int newTime)
{
    m_buttonPressingTime = newTime;
}

void Scenario::setDelayActionTime(int newTime)
{
    m_actionDelayTime = newTime;
}
