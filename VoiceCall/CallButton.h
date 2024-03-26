#ifndef CALLBUTTON_H
#define CALLBUTTON_H

#include <QPushButton>
class CallButton : public QPushButton
{
    Q_OBJECT
public:
    CallButton();
    ~CallButton();

    void init(QString _ip, uint _port, QString _name);
    void mousePressEvent(QMouseEvent *e);

signals:
    void Call(QString, uint);

private:
    QString ip;
    uint port = 0;
    QString name;
};

#endif // CALLBUTTON_H
