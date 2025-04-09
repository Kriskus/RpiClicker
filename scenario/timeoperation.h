#ifndef TIMEOPERATION_H
#define TIMEOPERATION_H

#include "operation.h"
#include <QObject>
#include <QString>
#include <QStringList>

class TimeOperation : public Operation
{
    Q_OBJECT
public:
    TimeOperation();
    bool checkOperation(const QString &line) override;
    void prepareOperation(const QString &line) override;
    void execute();

private:
    int m_time{0};
    bool m_buttonTime{false};

signals:
    void sendButtonTime(int);
    void sendActionTime(int);
    void sendErrorQuestion(const QString&);
};

#endif // TIMEOPERATION_H
