#ifndef SLEEP_H
#define SLEEP_H

#include "operation.h"
#include <QObject>
#include <QThread>

class Sleep : public Operation
{
    Q_OBJECT
public:
    Sleep();
    bool checkOperation(const QString &line) override;
    void prepareOperation(const QString &line) override;
    void execute() override;

private:
    int m_time{0};
};

#endif // SLEEP_H
