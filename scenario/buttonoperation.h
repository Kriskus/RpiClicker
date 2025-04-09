#ifndef BUTTONOPERATION_H
#define BUTTONOPERATION_H

#include "operation.h"
#include <QObject>
#include <QVector>
#include <QString>

class ButtonOperation : public Operation
{
    Q_OBJECT
public:
    ButtonOperation();
    bool checkOperation(const QString &line) override;
    void prepareOperation(const QString &line) override;
    void execute();

    void breakButtons();

private:
    QVector<QString> m_buttons{};

    bool m_break{false};

signals:
    void sendButtons(const QString&);
    void pause();
};

#endif // BUTTONOPERATION_H
