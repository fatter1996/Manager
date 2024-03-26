#pragma once
#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <QObject>

#define WORKTYPE_SERVER     1
#define WORKTYPE_CLIENT     2
#define WORKTYPE    WORKTYPE_SERVER

struct ConfigInfo
{
    //TCP-IP
    QString serverIp;
    uint serverPORT;

    //UDP-IP
    QString localIp;
    uint localPORT;

    //语音
    int sampleRate = 0; //采样率
    int channelCount = 0; //声道数
    int sampleSize = 0; //样本数

    //车站信息
    QString name;
public:
    void init();
}; //配置文件读取,设为全局变量



class GlobalData
{
public:
    static ConfigInfo* m_pConfigInfo;
    static int getBigEndian_2Bit(QByteArray bigArray);
    static QByteArray getLittleEndian_2Bit(int bigArray);
    static QDateTime getDateTimeByArray(QByteArray data);
    static QByteArray getDateTimeArray(QDateTime time);
    static QByteArray getIPeArray(QString ip);
};



#endif // UNIVERSAL_H
