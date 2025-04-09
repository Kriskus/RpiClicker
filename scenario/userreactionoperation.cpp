#include "userreactionoperation.h"

UserReactionOperation::UserReactionOperation()
{}

bool UserReactionOperation::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[Reakcja]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[Reaction]"), Qt::CaseInsensitive)) {
        prepareOperation(line);
        return true;
    } else {
        return false;
    }
}

void UserReactionOperation::prepareOperation(const QString &line)
{
    m_information = line.split(']').last().trimmed();
}

void UserReactionOperation::execute()
{
    emit sendReaction(m_information);
}
