#include "ServerSide.h"
#include <QDebug>
#include "VoiceCall/Universal.h"
#pragma execution_character_set("utf-8")

ServerSide::ServerSide(QObject* parent)
{
    Q_UNUSED (parent)

    m_pServer = new QTcpServer;
    connect(m_pServer, &QTcpServer::newConnection, this, &ServerSide::onConnection);
    //connect(m_pServer, &QTcpServer::disconnect, this, &ServerSide::onConnection);
    m_pServer->listen(QHostAddress(GlobalData::m_pConfigInfo->serverIp), GlobalData::m_pConfigInfo->serverPORT); //绑定本地IP和端口

}

ServerSide::~ServerSide()
{
    if(m_pServer)
    {
        m_pServer->close();
        m_pServer = nullptr;
    }
}

void ServerSide::onConnection()
{
    //取出连接好的套接字
    QTcpSocket* m_pSocket = m_pServer->nextPendingConnection();
    QString ip;
    quint16 port = 0;
    //获得通信套接字的控制信息
    if(m_pSocket)
    {
        ip = m_pSocket->peerAddress().toString();//获取连接的 ip地址
        port = m_pSocket->peerPort();//获取连接的 端口号
        if(ip != "" && port != 0)
            m_pSocketList.append(m_pSocket);

        connect(m_pSocket, &QTcpSocket::readyRead, this, [=](){

            QByteArray receiveStr = m_pSocket->readAll();
            qDebug() << "----TCP Receive----";
            qDebug() << receiveStr.toHex();
            if(receiveStr.length() <= 10)
                return;
            //长度校验
            if(receiveStr.length() != GlobalData::getBigEndian_2Bit(receiveStr.mid(4,2)))
                return;

            if((unsigned char)receiveStr.at(9) == 0x00 && receiveStr.length() == 14)
            {
                m_pSocket->write(HeartBeat());
            }
            else if((unsigned char)receiveStr.at(9) == 0x96)
            {
                int nameLen = GlobalData::getBigEndian_2Bit(receiveStr.mid(10,2));
                QString name = QString::fromLocal8Bit(receiveStr.mid(12,nameLen));
                emit NewConnection(ip, port, name);
            }
            else emit Signalling(receiveStr);
        });

        connect(m_pSocket, &QTcpSocket::disconnected, this, [=](){
            emit DisConnection(ip, port);
            qDebug() << QString("客户端断开连接: %1:%2").arg(ip).arg(port);
        });

        qDebug() << QString("客户端连接成功: %1:%2").arg(ip).arg(port);
    }
}

void ServerSide::sendToClient(QByteArray data, QString ip, uint port)
{
    for(uint i = 0; i < m_pSocketList.size(); i++)
    {
        qDebug() << m_pSocketList.at(i)->peerAddress().toString() << m_pSocketList.at(i)->peerPort();
        if(ip == m_pSocketList.at(i)->peerAddress().toString() &&
                 port == m_pSocketList.at(i)->peerPort())
        {
            m_pSocketList.at(i)->write(data);
            qDebug() << "----TCP Send----";
            qDebug() << data.toHex();
            return;
        }
    }
}


void ServerSide::timerEvent(QTimerEvent * ev)
{

}

QByteArray ServerSide::HeartBeat()
{
    QByteArray heart;
    heart.append(QByteArray::fromHex("efefefef"));
    heart.append(QByteArray::fromHex("0e00"));
    heart.append(QByteArray::fromHex("0000"));
    heart.append(QByteArray::fromHex("ee"));
    heart.append(QByteArray::fromHex("00"));
    heart.append(QByteArray::fromHex("fefefefe"));
    return heart;
}






