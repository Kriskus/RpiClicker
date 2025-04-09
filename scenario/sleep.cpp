#include "sleep.h"

Sleep::Sleep()
{}

bool Sleep::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[sleep]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[pauza]"), Qt::CaseInsensitive)) {
        prepareOperation(line);
        return true;
    } else {
        return false;
    }
}

void Sleep::prepareOperation(const QString &line)
{
    bool ok;
    m_time = line.split(']').last().toInt(&ok, 10);
    if (!ok || m_time < 0) {
        m_time = 0; // set default
    }
}

void Sleep::execute()
{
    QThread::msleep(m_time);
}
