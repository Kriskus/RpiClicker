#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>

class Operation : public QObject
{
    Q_OBJECT
public:
    virtual ~Operation() {}

    virtual bool checkOperation(const QString& line) = 0;
    virtual void prepareOperation(const QString& line) = 0;
    virtual void execute() = 0;
};

#endif // OPERATION_H
