#ifndef COMMENTOPERATION_H
#define COMMENTOPERATION_H

#include "operation.h"
#include <QObject>
#include <QStringList>

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
    void sendComment(const QString&);
};

#endif // COMMENTOPERATION_H
