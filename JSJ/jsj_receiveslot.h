#ifndef JSJ_RECEIVESLOT_H
#define JSJ_RECEIVESLOT_H

#include <QObject>
#include<QUdpSocket>
class jsj_receiveSlot : public QUdpSocket
{
    Q_OBJECT
public:
    explicit jsj_receiveSlot();
    QUdpSocket *udpSocket = nullptr;//
    ~jsj_receiveSlot();
public:
    void Init_Interface(int port);
     static jsj_receiveSlot &getDBService();
private slots:
    void onSocketReadyRead();
private:
    void processTheDatagram(QByteArray array,QHostAddress Address,int Port);
};
#define TXSERVICES (jsj_receiveSlot::getDBService())
#endif // JSJ_RECEIVESLOT_H
