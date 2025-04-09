#include "commentoperation.h"

CommentOperation::CommentOperation()
{}

bool CommentOperation::checkOperation(const QString &line)
{
    if(line.contains(QStringLiteral("[Komentarz]"), Qt::CaseInsensitive)
            || line.contains(QStringLiteral("[Comment]"), Qt::CaseInsensitive)) {
        prepareOperation(line);
        return true;
    } else {
        return false;
    }
}

void CommentOperation::prepareOperation(const QString &line)
{
    m_comment = line.split(']').last().trimmed();
}

void CommentOperation::execute()
{
    emit sendComment(m_comment);
}
