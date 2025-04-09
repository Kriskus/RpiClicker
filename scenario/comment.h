#ifndef COMMENT_H
#define COMMENT_H

#include "operation.h"
#include <QObject>

class CommentOperation : public Operation {
    Q_OBJECT
public:
    CommentOperation();
    bool checkOperation(const QString& line) override;
    void prepareOperation(const QString& line) override;
    void execute() override;

private:
    QString m_comment{};

signals:
    void sendComment(QString);
};

#endif // COMMENT_H
