#ifndef LOOPOPERATION_H
#define LOOPOPERATION_H

#include "operation.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QTime>
#include <QDebug>

class LoopOperation : public Operation
{
    Q_OBJECT
public:
    LoopOperation();
    bool checkOperation(const QString &line) override;
    void prepareOperation(const QString &line) override;
    int prepareOperationData(const QVector<QString> &scenario, int currentLine, int buttonPressedTime, int nextActionTime);
    void execute() override;

private:
    QVector<QString> m_loopData{};

    bool m_loopIntType{false};
    int m_repeatNumbers{0};
    QTime m_toTimeLoop{};

    int m_buttonPressedTime{0};
    int m_nextActionTime{0};

    void setLoopType(const QString &line);

signals:
    void pauseCurrentThread();
    void sendLoopDataRepeat(const QVector<QString>&, int, int, int);
    void sendLoopDataTime(const QVector<QString>&, const QTime&, int, int);
};

#endif // LOOPOPERATION_H
