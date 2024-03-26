#ifndef BASESTRUCTURE_H
#define BASESTRUCTURE_H

#pragma execution_character_set("utf-8")

#include<QObject>
#include <QString>
#include<QWidget>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#define DBName "demo.db"

#include <vector>
using namespace std;


static unsigned int StringToHex(QString  SetString)
{
    return    SetString.toULong(0,16);
}
/*******************车站列表List************************/
typedef  struct
{
    int  StationID;
    QString stationname;
    int StationCode;
    QString stationfilename;

    void InitInfo(){
        StationID = StationCode = 0xff;
        stationname = stationfilename ="";
    }
    bool JudgeData()
    {
        if(stationname == ""||stationfilename == ""
                || StationCode == 0xff)
            return false;
        else
            return true;
    }
}stationlist;
/*******************车站列表List************************/
#include<QStandardItem>
typedef  struct
{
    QString Devname;
    bool IsselectFlag;
    QString OperStr;
    QString StateStr;
    bool isChecked;
    int m_nCode;
    void InitInfo(){
        m_nCode=0xffff;
        IsselectFlag = true;
        isChecked = false;
        Devname = OperStr = StateStr = "";
    }
}zonedev;
typedef  struct
{
    int GDNUM;
    int DCNUM;
    int XHNUM;
    int  id;
    int  stationid;
    bool IsSelect;
    QString zonename;
    QString dcDevInfo;
    QString qdDevInfo;
    QString qjDevInfo;
    QString xhDevInfo;
    bool IsdcSelect = false;
    bool IsqdSelect = false;
    bool IsqjSelect = false;
    bool IsxhSelect;
    vector<zonedev> vect_dcDev;
    vector<zonedev> vect_qdDev;
    vector<zonedev> vect_qjDev;
    vector<zonedev> vect_xhDev;

    void InitInfo(){
        id = stationid = 0xff;
        zonename = dcDevInfo = qdDevInfo= qjDevInfo = xhDevInfo = "";
        SetAllCheck();
    }
    bool  IsInclude(QString Name,int type)
    {
        return false;
    }
    void SetAllCheck()
    {
        IsSelect = IsdcSelect = IsqdSelect = IsqjSelect = IsxhSelect = true;
        for(unsigned int i = 0;i<vect_dcDev.size();i++)
        {
           vect_dcDev[i].IsselectFlag = true;
        }
        for(unsigned int i = 0;i<vect_qdDev.size();i++)
        {
           vect_qdDev[i].IsselectFlag = true;
        }
        for(unsigned int i = 0;i<vect_qjDev.size();i++)
        {
           vect_qjDev[i].IsselectFlag = true;
        }
        for(unsigned int i = 0;i<vect_xhDev.size();i++)
        {
           vect_xhDev[i].IsselectFlag = true;
        }

    }
    void SetCheck(QString Name,bool checkstate)
    {

        if(Name == zonename)
        {
            IsSelect = IsdcSelect = IsqdSelect = IsqjSelect = IsxhSelect = checkstate;
        }
        else if(Name == QString::fromLocal8Bit("道岔"))
        {
            IsdcSelect = checkstate;
            for(unsigned int i = 0;i<vect_dcDev.size();i++)
            {
                  vect_dcDev[i].IsselectFlag = checkstate;
            }

        }
        else if(Name == QString::fromLocal8Bit("股道/无岔"))
        {
            IsqdSelect = checkstate;
            for(unsigned int i = 0;i<vect_qdDev.size();i++)
            {
                  vect_qdDev[i].IsselectFlag = checkstate;
            }

        }
        else if(Name == QString::fromLocal8Bit("区间"))
        {
            IsqjSelect = checkstate;
            for(unsigned int i = 0;i<vect_qjDev.size();i++)
            {
                  vect_qjDev[i].IsselectFlag = checkstate;
            }

        }
        else if(Name == QString::fromLocal8Bit("信号机"))
        {
            IsxhSelect = checkstate;
            for(unsigned int i = 0;i<vect_xhDev.size();i++)
            {
                  vect_xhDev[i].IsselectFlag = checkstate;
            }

        }
        else
        {
            for(unsigned int i = 0;i<vect_dcDev.size();i++)
            {
                if(Name == vect_dcDev[i].Devname)
                {
                  vect_dcDev[i].IsselectFlag = checkstate;
                  return ;
                }
            }
            for(unsigned int i = 0;i<vect_qdDev.size();i++)
            {
                if(Name == vect_qdDev[i].Devname)
                {
                  vect_qdDev[i].IsselectFlag = checkstate;
                  return ;
                }
            }
            for(unsigned int i = 0;i<vect_qjDev.size();i++)
            {
                if(Name == vect_qjDev[i].Devname)
                {
                  vect_qjDev[i].IsselectFlag = checkstate;
                  return ;
                }
            }
            for(unsigned int i = 0;i<vect_xhDev.size();i++)
            {
                if(Name == vect_xhDev[i].Devname)
                {
                  vect_xhDev[i].IsselectFlag = checkstate;
                  return ;
                }
            }
        }


    }
    void AddDevList(QStringList xhInfo,QStringList dcInfo,QStringList qjInfo,QStringList qdInfo,QStringList xhInfo_code,QStringList dcInfo_code,QStringList qjInfo_code,QStringList qdInfo_code)
    {
        for(int i = 0;i<dcInfo.size();i++)
        {
            if(dcInfo[i] == "" || IsInclude(dcInfo[i],0x11))
                continue;
            zonedev zone;
            zone.InitInfo();
            zone.Devname = dcInfo[i];
            zone.m_nCode = dcInfo_code[i].toInt();
            vect_dcDev.push_back(zone);
        }
        for(int i = 0;i<qjInfo.size();i++)
        {
            if(qjInfo[i] == ""|| IsInclude(qjInfo[i],0x22))
                continue;
            zonedev zone;
            zone.InitInfo();
            zone.Devname = qjInfo[i];
            zone.m_nCode = qjInfo_code[i].toInt();
            vect_qjDev.push_back(zone);

        }
        for(int i = 0;i<qdInfo.size();i++)
        {
            if(qdInfo[i] == "" ||IsInclude(qdInfo[i],0x33))
                continue;
            zonedev zone;
            zone.InitInfo();
            zone.Devname = qdInfo[i];
            zone.m_nCode = qdInfo_code[i].toInt();
            vect_qdDev.push_back(zone);
        }
        for(int i = 0;i<xhInfo.size();i++)
        {
            if(xhInfo[i] == "" ||IsInclude(xhInfo[i],0x44))
                continue;
            zonedev zone;
            zone.InitInfo();
            zone.Devname = xhInfo[i];
            zone.m_nCode = xhInfo_code[i].toInt();
            vect_xhDev.push_back(zone);
        }
    }
    void DevIdSpilt(QString GetInfo,int Type)
    {
        QStringList BuffList;
        if(Type == 0x11)
        {
            BuffList = GetInfo.split("&");
            if(BuffList.size() == 0)
                return ;

            dcDevInfo = GetInfo;
            vect_dcDev.clear();

            for(int i = 0;i<BuffList.size();i++)
            {
                if(BuffList[i] == "")
                    continue;
                zonedev zone;
                zone.InitInfo();
                zone.Devname = BuffList[i];

                vect_dcDev.push_back(zone);
            }
        }
        else if(Type == 0x22)
        {
            BuffList = GetInfo.split("&");
            if(BuffList.size() == 0)
                return ;
            qdDevInfo = GetInfo;
            vect_qdDev.clear();
            for(int i = 0;i<BuffList.size();i++)
            {
                if(BuffList[i] == "")
                    continue;
                zonedev zone;
                zone.InitInfo();
                zone.Devname = BuffList[i];

                vect_qdDev.push_back(zone);
            }
        }
        if(Type == 0x33)
        {
            BuffList = GetInfo.split("&");
            if(BuffList.size() == 0)
                return ;
            qjDevInfo = GetInfo;
            vect_qjDev.clear();
            for(int i = 0;i<BuffList.size();i++)
            {
                if(BuffList[i] == "")
                    continue;
                zonedev zone;
                zone.InitInfo();
                zone.Devname = BuffList[i];

                vect_qjDev.push_back(zone);
            }
        }
        if(Type == 0x44)
        {
            BuffList = GetInfo.split("&");
            if(BuffList.size() == 0)
                return ;
            xhDevInfo = GetInfo;
            vect_xhDev.clear();
            for(int i = 0;i<BuffList.size();i++)
            {
                if(BuffList[i] == "")
                    continue;
                zonedev zone;
                zone.InitInfo();
                zone.Devname = BuffList[i];

                vect_xhDev.push_back(zone);
            }
        }
    }
}zoneData; //分区定义


typedef  struct
{
    QString Typename;
    vector<zonedev *> vect_DevDev;

    void InitInfo(){
        Typename = "";
    }
}zoneType;

typedef  struct
{
    int  id;
    int  stationid;

    QString zonename;
    vector<zoneType *> vect_typeDev;

    void InitInfo(){
        id = stationid = 0xff;
        zonename = "";

    }
}zone; //分区定义

#define StationBasePath "data\\stationdata"
#define DCDev 0x11
#define QDDev 0x22
#define QJDev 0x33
#define XHDev 0x44
// person信息
typedef struct Person_t{
    QString name;   // 姓名
    QString sex;    // 性别
    int age;     // 年龄
    QString phone;  // 电话号码
    Person_t()
    {
        age = 0;
    }
} Person;

// 省份信息
typedef struct Province_t{
    QString name;
    QVector<Person*> people;
} Province;

// 树列号
enum COLUMN
{
    COLUMN_NAME = 0,
    COLUMN_Oper,
    COLUMN_State
};




#endif // BASESTRUCTURE_H
