#ifndef USERREACTIONOPERATION_H
#define USERREACTIONOPERATION_H

#include "operation.h"
#include <QObject>

class UserReactionOperation : public Operation
{
    Q_OBJECT
public:
    UserReactionOperation();
    bool checkOperation(const QString& line) override;
    void prepareOperation(const QString& line) override;
    void execute() override;

private:
    QString m_information{};

signals:
    void sendReaction(const QString&);
};

#endif // USERREACTIONOPERATION_H
