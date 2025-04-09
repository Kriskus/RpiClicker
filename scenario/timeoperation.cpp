#include "timeoperation.h"

TimeOperation::TimeOperation()
{}

bool TimeOperation::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[CWK]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[ButtonTime]"), Qt::CaseInsensitive)) {
        m_buttonTime = true;
        return true;
    } else if(line.contains(QStringLiteral("[ActionDelay]"), Qt::CaseInsensitive)
              || line.contains(QStringLiteral("[CNA]"), Qt::CaseInsensitive)){
        m_buttonTime = false;
        return true;
    }
    return false;
}

void TimeOperation::prepareOperation(const QString &line)
{
    bool ok;
    m_time = line.split(']').last().toInt(&ok, 10);
    if (!ok || m_time < 0) {
        emit sendErrorQuestion("Time value is invalid. Continue scenario?");
    } else {
        execute();
    }
}

void TimeOperation::execute()
{
    if(m_buttonTime) {
        emit sendButtonTime(m_time);
    } else {
        emit sendActionTime(m_time);
    }
}
