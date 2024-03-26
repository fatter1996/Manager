#ifndef CDATA_H
#define CDATA_H
#include "BaseDataPro/station.h"
#include "BaseDataPro/stationjsj.h"
#include "SocketPro/SocketUDP.h"
#include"JSJ/jsj_services.h"
#include <QButtonGroup>
#include <QRadioButton>
class JSJ_Services;
class CStationManager;
typedef struct
{
    QString menueName;
    QPushButton *MenueButton;
    QButtonGroup *pSubGroup;
    void Init()
    {
        menueName = "";
        MenueButton = nullptr;
        pSubGroup = nullptr;
    }
} MangerStr;
typedef struct QSqlData
{
    QString IPName;
    QString LoginName;
    QString Password;
    QString SqlName;
    unsigned int Port;
} SqlDataStr;
typedef struct station
{
    CStationJSJ* pStationData;
    unsigned int StationID;
    unsigned int ChannelID;
    QString StationName;
    QString IP_OppAdress; //连接IP
    unsigned short IP_OppPort;//连接端口
    QString IP_OppTrainAdress; //连接IP
    unsigned short IP_OppTrainPort;//连接端口
    int comCount;
    station::station()
    {
        pStationData = nullptr;
        StationID = 0xffff;
        ChannelID = 0xffff;
        IP_OppAdress =  "";
        IP_OppPort = 0;
        IP_OppTrainAdress = "";
        IP_OppTrainPort = 0;
        StationName = "";
        comCount = 0;
    }
} stationStr;
//列车类型数据-
typedef  struct
{
    QString strType;//类型
    QString strTypeName;//车次类型名车
} TrainType;
class CData : public QObject
{
    Q_OBJECT
public:
    explicit CData(QObject *parent = nullptr);
public:
    ~CData();
    QVector<stationStr> AllStation;
    QButtonGroup *pButtonGroup=nullptr;
    QButtonGroup *pMenueButtonGroup;
    QVector<MangerStr> ManagerMenue;

    QVector<CTrain*> AllTrain;

    QVector<TrainType> v_TrainType;
    CStationManager *pManager;
    CTh_kb *pJM;
protected:
    CSocketUDP  m_UDPSock;
    CSocketUDP  m_UDPTrainSock;
    CSocketUDP m_TCCSock;
    SqlDataStr mySqlStr;


public:
    void readTrainTypeTXT();
    void ReadStationData(int index, int Num);
    void ReadLZZDBSData(CStationJSJ *pStation, int Num);
    void InitUDPConnect();
    void InitMenueAndButton(QString FileName);
    void InitButton(QPushButton *button, QString name);
    void InitMenueButton(QPushButton *button, QString name);
    void setButtonChecked(QPushButton *button, bool isChecked);
    void clearOtherButtonChecked(QPushButton *button);
    void SendUdpMessage(QByteArray array, int len, CStationJSJ *pStation);
    void SendUdpTrainMessage(QByteArray array, int len, CStationJSJ *pStation);
    void SendUdpMessage(CSocketUDP *sock, QByteArray array, int len, QString ip, unsigned short port);
    void timePro(long nElapsed);
    void Time100MS();
    int GetId_ByStation(CStationJSJ *pStation);
    void AnalyseTrainMsg(unsigned char *rec, int channelID, int index);
    void AnalyseBSMsg(unsigned char *rec, int channelID, int index, int len);
    void Connect(CStationJSJ* pStation);

    int PackTrainAddMessage(unsigned char *send, int index, CStationJSJ* pStation);
    void UpdateTrainInfo(int index, unsigned char type, const unsigned char cxInfo = 0xff, const QString newTrainNum = "");
private slots:
    void AnalyseUdpData(QByteArray data, QString IpAdress, unsigned short port);
    void AnalyseUdpTrianData(QByteArray data, QString IpAdress, unsigned short port);
    void AnalyTCCData(QByteArray data, QString IpAdress, unsigned short port);
    void TreeViewClick(const QModelIndex &index);
    void slotUpdateMenue(int index);
};

#endif // CDATA_H
