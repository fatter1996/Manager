#include "ClientSide.h"
#include <QSettings>
#include <QDebug>
#include "VoiceCall/Universal.h"

ClientSide::ClientSide(QObject* parent)
{
    m_pSocket = new QTcpSocket;
    connect(m_pSocket, &QTcpSocket::connected, this, &ClientSide::onConnected);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &ClientSide::onReceive);
    timerId = startTimer(1000);
    timerId2 = startTimer(1000);
}

ClientSide::~ClientSide()
{
    killTimer(timerId);
    if(m_pSocket != nullptr)
    {
        m_pSocket->disconnectFromHost();//断开与服务器的连接
        m_pSocket->close();
        m_pSocket = nullptr;
    }
}

void ClientSide::onConnected()
{
    //连接成功后发送学员名称
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(GlobalData::getLittleEndian_2Bit(GlobalData::m_pConfigInfo->name.toLocal8Bit().length() + 16));
    array.append(QByteArray::fromHex("0000"));
    array.append(QByteArray::fromHex("ff"));
    array.append(QByteArray::fromHex("96"));
    array.append(GlobalData::getLittleEndian_2Bit(GlobalData::m_pConfigInfo->name.toLocal8Bit().length()));
    array.append(GlobalData::m_pConfigInfo->name.toLocal8Bit());
    array.append(QByteArray::fromHex("fefefefe"));
    m_pSocket->write(array);
    flag = 0;
}

void ClientSide::onReceive()
{
    QByteArray receiveStr = m_pSocket->readAll();
    if(receiveStr.length() <= 10)
        return;
    //长度校验
    if(receiveStr.length() != GlobalData::getBigEndian_2Bit(receiveStr.mid(4,2)))
        return;

    if((unsigned char)receiveStr.at(9) == 0x00 && receiveStr.length() == 14)
    {
        flag = 0;
    }
    else emit Signalling(receiveStr);
    qDebug() << "----TCP Receive----";
    qDebug() << receiveStr.toHex();
}

void ClientSide::sendToServer(QByteArray data)
{
    m_pSocket->write(data);
    qDebug() << "----TCP Send----";
    qDebug() << data.toHex();
}

void ClientSide::HeartBeat()
{
    QByteArray heart;
    heart.append(QByteArray::fromHex("efefefef"));
    heart.append(QByteArray::fromHex("0e00"));
    heart.append(QByteArray::fromHex("0000"));
    heart.append(QByteArray::fromHex("ff"));
    heart.append(QByteArray::fromHex("00"));
    heart.append(QByteArray::fromHex("fefefefe"));
    m_pSocket->write(heart);
}

void ClientSide::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId() == timerId)
    {
        if(flag >= 3)
        {
            m_pSocket->disconnectFromHost();
            m_pSocket->connectToHost(GlobalData::m_pConfigInfo->serverIp, GlobalData::m_pConfigInfo->serverPORT);
        }
        flag++;
    }
    if(ev->timerId() == timerId2)
    {
        HeartBeat();
    }
}


