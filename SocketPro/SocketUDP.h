#ifndef CSOCKETUDP_H
#define CSOCKETUDP_H
#include <QObject>
#include <QUdpSocket>
//#include "Doc.h"
class CSocketUDP : public QObject
{
    Q_OBJECT
public:
    explicit CSocketUDP(QObject *parent = nullptr);

public:
    QUdpSocket *mSocket;  //申明socket通道


public:
    bool init(QString ip_Adress, int port);
    void packHeadAndEnd(QByteArray &msg, int len);

signals:
    void signal_ReceiveUDP(QByteArray data, QString client_add, unsigned short port);

public slots:
    void slot_ReceiveUDP_data();//通信接收数据槽函数
    void slot_send(QByteArray msg, QString OppAdress, int OppProt, int len);
    void send(QByteArray msg, QString OppAdress, int OppProt, int len);
};

#endif // CSOCKETUDP_H
