#include "Universal.h"
#include <QDateTime>
#include <QSettings>
#include <QDir>

ConfigInfo* GlobalData::m_pConfigInfo = nullptr;

void ConfigInfo::init()
{
    QString path = QDir::currentPath() + "/data/AudioCallConfig.ini";
    QSettings* config = new QSettings(path, QSettings::IniFormat);
    //TCP-IP
    serverIp = config->value("/TCP/ServerIp").toString();
    serverPORT =  config->value("/TCP/ServerPort").toUInt();

    //UDP-IP
    localIp = config->value("/UDP/LocalIp").toString();
    localPORT = config->value("/UDP/LocalPort").toUInt();

    //语音
    sampleRate = config->value("/Audio/SampleRate").toInt();
    channelCount = config->value("/Audio/ChannelCount").toInt();
    sampleSize = config->value("/Audio/SampleSize").toInt();

    //车站信息
    name = config->value("/Station/Name").toString();
    delete config;
}

int GlobalData::getBigEndian_2Bit(QByteArray bigArray)
{
    int array = ((unsigned char)bigArray[0] << 8) + (unsigned char)bigArray[1];
    int L = array / 0x100;
    int H = array % 0x100;
    return (H << 8) + L;
}

QByteArray GlobalData::getLittleEndian_2Bit(int bigArray)
{
    if(bigArray == 0)
        return QByteArray::fromHex("0000");
    int L = bigArray % 0x100;
    int H = bigArray / 0x100;
    return QByteArray::fromHex(QString::number((L << 8) + H, 16).toLocal8Bit());
}

QDateTime GlobalData::getDateTimeByArray(QByteArray data)
{
    QDateTime dateTime;
    int year = getBigEndian_2Bit(data.left(2));
    int month = data[2];
    int day = data[3];
    int hour = data[4];
    int minute = data[5];
    int second = data[6];
    QDate date;
    date.setDate(year, month, day);
    QTime time;
    time.setHMS(hour, minute, second);
    dateTime.setDate(date);
    dateTime.setTime(time);

    return dateTime;
}

QByteArray GlobalData::getDateTimeArray(QDateTime time)
{
    QByteArray data;
    data.append(getLittleEndian_2Bit(time.date().year()));
    data.append(time.date().month());
    data.append(time.date().day());
    data.append(time.time().hour());
    data.append(time.time().minute());
    data.append(time.time().second());
    return data;
}

QByteArray GlobalData::getIPeArray(QString ip)
{
    QStringList ipStr = ip.split('.');
    QByteArray data;
    for(uint i = 0; i < ipStr.size(); i++)
    {
        data.append(ipStr.at(i).toUInt());
    }
    return data;
}
