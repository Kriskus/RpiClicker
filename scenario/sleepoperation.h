#ifndef SLEEPOPERATION_H
#define SLEEPOPERATION_H

#include "operation.h"
#include <QObject>
#include <QThread>

class SleepOperation : public Operation
{
    Q_OBJECT
public:
    SleepOperation();
    bool checkOperation(const QString &line) override;
    void prepareOperation(const QString &line) override;
    void execute() override;

private:
    int m_time{0};
};

#endif // SLEEPOPERATION_H
