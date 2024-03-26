#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#pragma execution_character_set("utf-8")

#include "mysql.h"
#include<QString>
#define SHOWPATH "baseconfig.ini"
//#define BatchOper
//#include "socketconn/UDPSend.h"
class globaldata
{
public:
    globaldata();
public:
    ~globaldata();
    static globaldata& GetInstance()
    {
        static globaldata s_instance;
        return s_instance;
    }
    globaldata(globaldata const&)=delete ;
    globaldata& operator= (globaldata const&)=delete ;



public:
    bool isSqlConFlag;
    void  sqlIniteData();
    QString SelectStationName = "";
    mysql  *currentSql;
    QString localIP;
    QString oppIP;
    unsigned int  localport;
    unsigned int  oppport;

    void SenddataFun();


};
#define global globaldata::GetInstance()
#endif // GLOBALDATA_H
