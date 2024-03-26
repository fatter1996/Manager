#ifndef T_globaldata_H
#define T_globaldata_H

#pragma execution_character_set("utf-8")

#include "mysql.h"
#include<QString>
#define SHOWPATH "baseconfig.ini"
//#define BatchOper
//#include "socketconn/UDPSend.h"
class T_globaldata
{
public:
    T_globaldata();
public:
    ~T_globaldata();
    static T_globaldata& GetInstance()
    {
        static T_globaldata s_instance;
        return s_instance;
    }
    T_globaldata(T_globaldata const&)=delete ;
    T_globaldata& operator= (T_globaldata const&)=delete ;



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
#define globals T_globaldata::GetInstance()
#endif // T_globaldata_H
