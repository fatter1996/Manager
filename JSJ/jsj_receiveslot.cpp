#include "jsj_receiveslot.h"
#include<QMessageBox>
jsj_receiveSlot::jsj_receiveSlot()
{

}

jsj_receiveSlot::~jsj_receiveSlot()
{
    if(udpSocket!=nullptr)
    {
        udpSocket->abort();
        delete udpSocket;
    }

}
#include"jsj_services.h"
void jsj_receiveSlot::Init_Interface(int port)
{
    udpSocket=new QUdpSocket(this);//用于与连接的客户端通讯的QTcpSocket
    if(udpSocket->bind(DBSERVICES.LoginSQLData.LocalPort port))
        connect(udpSocket,SIGNAL(readyRead()),
                this,SLOT(onSocketReadyRead()));
    else
        QMessageBox::information(nullptr,"提示","端口绑定失败");


}

void jsj_receiveSlot::processTheDatagram(QByteArray array, QHostAddress Address, int Port)
{
    QByteArray array_Rec;
    array_Rec.resize(1000);
    array_Rec = array;
    const int length = array_Rec.size();
    if ((unsigned char)array_Rec[0] == 0xef && ((unsigned char)array_Rec[1] == 0xef) &&
            ((unsigned char)array_Rec[2] == 0xef) && ((unsigned char)array_Rec[3] == 0xef))

    {
        if ((unsigned char)array_Rec[9] == 0x32) //阶段计划接收信息
        {
            QString PlanNumber = array_Rec.mid(9,4);
            int length = array[14];
            QString CheciNumber = array_Rec.mid(14,length);
            //emit Set_jdjh_Slot(PlanNumber,CheciNumber);
            qDebug()<<"接收到阶段计划信息:"<<PlanNumber<<CheciNumber;
        }
        else if ((unsigned char) array_Rec[9] == 0x04)
        {
            QString Number = array_Rec.mid(9,10);
            Number.remove(QRegExp("\\s"));
            //emit SetDDML_Slot(Number);
            qDebug()<<"接收到调度命令信息:"<<Number;
        }else
        {
            int IDNUM = array_Rec[6];//检测发过来的心跳是否有与之匹配项
            int index = -1;
            qDebug()<<"StationId:"<<IDNUM;
            qDebug()<<"StationIP:"<<Address;
            for (index = 0; index <  DBSERVICES.CurBackInfoList.size();index++)
            {
                if(IDNUM == DBSERVICES.CurBackInfoList[index].StationId)
                {
                    DBSERVICES.CurBackInfoList[index].IPStr = Address.toString();
                    DBSERVICES.CurBackInfoList[index].Port = Port;
                    break;
                }

            }
        }
    }
}
void jsj_receiveSlot::onSocketReadyRead()
{
    while (udpSocket->hasPendingDatagrams())
    {
        // 数据缓冲区，类似 char [ ]
        QByteArray datagram;
        // 获取当前数据包的大小，从到来的套接字数据包
        datagram.resize(udpSocket->pendingDatagramSize());   //这里为什么会阻塞
        QHostAddress sender;  // 保存客户端的IP地址
        quint16 senderPort;  // 保存客户端的端口号
        // 读取数据，.data ( )完成的是数据类型转换为 char *
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        // 解析和处理...
        processTheDatagram(datagram,sender,senderPort);
    }

}
jsj_receiveSlot &jsj_receiveSlot::getDBService()
{
    static jsj_receiveSlot s_DBService;
    return  s_DBService;
}
