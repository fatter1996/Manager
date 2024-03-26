#ifndef MYSQL_H
#define MYSQL_H

#pragma execution_character_set("utf-8")

#include "sqldatastructure.h"
#include "basestructure.h"
#include <QSqlQueryModel>

class mysql
{
public:
    mysql();
public:
    ~mysql();
//    static mysql& GetInstance()
//    {
//        static mysql s_instance;
//        return s_instance;
//    }
//    mysql(mysql const&)=delete ;
//    mysql& operator= (mysql const&)=delete ;

private:
    sqlbasedata sqldata;

public:
    void Readmysqlini();
    QSqlDatabase db;
    bool MysqlConnect(const QString &dbName);
    QString AddStationData(stationlist list);
    QVector<stationlist>  Readstationlist();
    QString AddGDData(QVariantList codes,QVariantList names,QVariantList types,int stationid);
    QString DetleGDData(int stationid,QString FormName);
    QString AddQJData(QVariantList codes,QVariantList names,int stationid);
    QString AddGDDCData(QVariantList codes,QVariantList names,int stationid);
    QString UpdataDevData(QString FromName,int stationid,QString Name,int state);
    QString AddXHData(QVariantList codes,QVariantList names,QVariantList types,int stationid);
    QString AddGDBaseData(QVariantList names,int stationid);
    QVector<zoneData> ReadZonelist(int id);
    QString LoadZoneData(zoneData *zone);
    QString DetleZoneData(int Zoneid);
    QString UpdataDevState(zoneData *zone);


};
//#define sql mysql::GetInstance()
#endif // MYSQL_H
