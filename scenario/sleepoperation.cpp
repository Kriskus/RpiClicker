#include "sleepoperation.h"

SleepOperation::SleepOperation()
{}

bool SleepOperation::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[sleep]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[pauza]"), Qt::CaseInsensitive)) {
        prepareOperation(line);
        return true;
    } else {
        return false;
    }
}

void SleepOperation::prepareOperation(const QString &line)
{
    bool ok;
    m_time = line.split(']').last().toInt(&ok, 10);
    if (!ok || m_time < 0) {
        m_time = 0; // set default
    }
}

void SleepOperation::execute()
{
    QThread::msleep(m_time);
}
