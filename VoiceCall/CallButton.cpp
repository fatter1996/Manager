#include "CallButton.h"
#include <QDebug>

CallButton::CallButton()
{
    setFixedSize(24, 24);
    //setText(name);
    setObjectName(QString("%1:%2").arg(ip).arg(port));
}

CallButton::~CallButton()
{

}

void CallButton::init(QString _ip, uint _port, QString _name)
{
    ip = _ip;
    port = _port;
    name = _name;
}

void CallButton::mousePressEvent(QMouseEvent *e)
{
    emit Call(ip, port);
}
