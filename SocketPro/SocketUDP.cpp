#include "SocketUDP.h"

CSocketUDP::CSocketUDP(QObject *parent) : QObject(parent)
{

}


bool CSocketUDP::init(QString ip_Adress, int port)
{
    mSocket = new QUdpSocket();
    if (true == mSocket->bind(QHostAddress(ip_Adress), port)) //绑定本地接收UDP socket通道
    {
        connect(mSocket, SIGNAL(readyRead()), this, SLOT(slot_ReceiveUDP_data()));
        return true;
    }
    else
    {
        return false;
    }
}
void CSocketUDP::slot_send(QByteArray msg, QString OppAdress, int OppProt, int len)
{
    packHeadAndEnd(msg, len);
    mSocket->writeDatagram(msg, QHostAddress(OppAdress), OppProt);
}

void CSocketUDP::send(QByteArray msg, QString OppAdress, int OppProt, int len)
{
    mSocket->writeDatagram(msg, QHostAddress(OppAdress), OppProt);
}
void CSocketUDP::packHeadAndEnd(QByteArray &msg, int len)
{
    for (int i = 0; i < 4; i++)
    {
        msg[i] = 0xEF;
    }
    for (int i = 1; i < 5; i++)
    {
        msg[len - i] = 0xFE;
    }
    msg[4] = (len);
    msg[5] = (len >> 8);
    //qDebug()<<len<<msg.size()<<msg.toHex();
}
void CSocketUDP::slot_ReceiveUDP_data()  //接收数据槽函数
{
    QByteArray rec_Data;
    QHostAddress client_address; //client ip addr
    unsigned short port;
    rec_Data.clear();
    while (mSocket->hasPendingDatagrams())
    {
        rec_Data.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(rec_Data.data(), rec_Data.size(), &client_address, &port);

        QString strclient_address = client_address.toString();
        emit signal_ReceiveUDP(rec_Data, strclient_address, port);
    }
}

