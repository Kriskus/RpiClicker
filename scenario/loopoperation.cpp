#include "loopoperation.h"

LoopOperation::LoopOperation()
{}

bool LoopOperation::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[loop]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[petla]"), Qt::CaseInsensitive)) {
        return true;
    } else {
        return false;
    }
}

void LoopOperation::prepareOperation(const QString &line)
{
    Q_UNUSED(line);
}

int LoopOperation::prepareOperationData(const QVector<QString> &scenario, int currentLine, int buttonPressedTime, int nextActionTime)
{
    m_buttonPressedTime = buttonPressedTime;
    m_nextActionTime = nextActionTime;
    m_loopData.clear();
    QString temp{};
    for(int cLine = currentLine; cLine < scenario.size(); cLine++) {
        temp = scenario[cLine];
        if(scenario[cLine].contains(QStringLiteral("[loop]"), Qt::CaseInsensitive)) {
            setLoopType(scenario[cLine]);
        } else if(scenario[cLine].contains(QStringLiteral("[endloop]"), Qt::CaseInsensitive)) {
            return cLine;
        } else {
            m_loopData.push_back(scenario[cLine]);
        }
    }
    return 0;
}

void LoopOperation::execute()
{
    if(m_loopIntType) {
        emit sendLoopDataRepeat(m_loopData, m_repeatNumbers, m_buttonPressedTime, m_nextActionTime);
    } else {
        emit sendLoopDataTime(m_loopData, m_toTimeLoop, m_buttonPressedTime, m_nextActionTime);
    }
}

void LoopOperation::setLoopType(const QString &line)
{
    m_repeatNumbers = line.split(']').last().toInt(&m_loopIntType, 10);
    if(!m_loopIntType) {
        QString time = line.split(']').last();
        time.replace('\n', "");
        m_toTimeLoop = QTime::fromString(time, "HH:mm");
    }
}
