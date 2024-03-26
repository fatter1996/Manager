#include "globaldata.h"

#include <QMessageBox>
#include <QSettings>
globaldata::globaldata()
{
    isSqlConFlag =false;
    sqlIniteData();



//    QString File = QString("%1/%2").arg(QDir::currentPath()).arg(SHOWPATH);
//    QSettings *configIniRead = new QSettings(File, QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型


//    localIP = configIniRead->value("/IP/localIP").toString();
//    oppIP = configIniRead->value("/IP/oppIP").toString();
//    localport = configIniRead->value("/IP/localport").toInt();
//    oppport = configIniRead->value("/IP/oppport").toInt();


//    send = new UDPSend();
//    if(!send->initSender(oppIP,oppport,localport))
//    {
//      qDebug()<<"Udp is False!";
//    }

}
globaldata::~globaldata()
{

}

void globaldata::sqlIniteData()
{
#ifndef BatchOper
//    isSqlConFlag= currentSql->MysqlConnect(DBName);
#endif
}

void globaldata::SenddataFun()
{
    QByteArray sendArray;
    for(unsigned int  i =0;i<4;i++)
        sendArray.push_back(0xEF);
  unsigned int lenth = 20;

  sendArray.push_back((lenth&0x00ff)); //低八位
  sendArray.push_back((lenth>>8));//高八位

  int buffnum = 0;
  sendArray.push_back(buffnum);
  sendArray.push_back(buffnum);
  sendArray.push_back(buffnum);

  sendArray.push_back(0xa1);


  for(unsigned int  i =0;i<6;i++)
     sendArray.push_back(buffnum);

  for(unsigned int  i =0;i<4;i++)
     sendArray.push_back(0xFE);
//  send->send(sendArray);
}
