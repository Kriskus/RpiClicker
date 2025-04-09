#include "buttonoperation.h"

#include <QDebug>

ButtonOperation::ButtonOperation()
{}

bool ButtonOperation::checkOperation(const QString &line)
{
    return !line.isEmpty();
}

void ButtonOperation::prepareOperation(const QString &line)
{
    m_buttons.clear();
    for(int el = 0; el < line.size(); el++) {
        if(line[el] == '\n') {
            break;
        }
        if(line[el] == '[') {
            QString temp{};
            while(line[el] != ']') {
                temp.push_back(line[el++]);
            }
            temp.replace('[', "").replace(']', "");
            m_buttons.push_back(temp);
        }
        m_buttons.push_back(line[el]);
    }
}

void ButtonOperation::execute()
{
    for(const auto& button: m_buttons) {
//        qDebug() << button;
        emit sendButtons(button);
        if(m_break) {
            break;
        }
    }
}

void ButtonOperation::breakButtons()
{
    m_break = true;
}
