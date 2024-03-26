#ifndef SQLDATASTRUCTURE_H
#define SQLDATASTRUCTURE_H

#pragma execution_character_set("utf-8")

#include <QString>

#define sqlcongfigpath "SystemConfig_t3.ini"
/*******************数据库参数结构图************************/
typedef  struct
{
    QString HostName;
    QString UserName;
    QString PassWord;
    QString DataBase;
    int Dort;
    void InitInfo(){
      Dort = 0xff;
      HostName = UserName = PassWord = DataBase ="";
    }
}sqlbasedata;

#endif // SQLDATASTRUCTURE_H
