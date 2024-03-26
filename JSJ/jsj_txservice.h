#ifndef JSJ_TXSERVICE_H
#define JSJ_TXSERVICE_H
#include <QObject>
#include<QUdpSocket>
#include<QTimer>
#include"jsj_services.h"
#define LOCALCODE       0x57
#define CTC             0x12
#define TYPE            0xbb
#define DDML_           0x99
#define XSML_           0x0F
#define LJKS            0xAA
#define ZDSJ            0xBB
#define JDJH            0x33
#define ZCCZ            0xEA
#define ShutDown        0xAB
#define HEARTBEAT       0x23
#define CLOCKSAME       0xaa
#define CONTROLCMD      0x2C
#define CENTRT_CONTROL  0x01
#define Station_CONTROL 0x02
#define FSZL_CONTROL    0X03
#define TCC_TOCTC_CONTROL 0x2A
#define LS_INFO         0xAA
#define ZCWRONG         0x01
#define CCHDS           0x03//车次号丢失
#define YJXF            0xAA//一键下发
#define BSZD            0x04//全站表示中断
#define GZ_Set          0x5B//故障设置
#define GZ_SetCode      0x5B
#define GZ_No_Set       0xA5//故障恢复
struct SendAddress{
    QString Ip;
    int Port;
    void initInfo(){
        Ip = "";
        Port = -1;
    }
};

class jsj_planNode;
class jsj_txService : public QUdpSocket
{
    Q_OBJECT
public:
    jsj_txService(QObject *parent = nullptr);
    bool bindPort();
    static jsj_txService &getDBService();
signals:
    void SetDDML_Slot(QString CommondNumber);
    void Set_jdjh_Slot(QString PlanNuMBER,QString CheciNumber);
    void ChangeControl_Cmd(ControlInfo&info);
private:
    IPInfo m_IPInfo;
    SendAddress tempAddress;
    QTimer * HeartTimer;
public:
    void UpDate_WrongSetStatus(unsigned char Cmd);
    void SetStatus(bool&Status);
    void Send_GZInfo(bool &isSet,unsigned char gzCode,unsigned int StationId);
    void Send_Currentto_CTC(DDML&ml);//调度命令
    void Send_xsmlto_CTC(DDML&ML, unsigned char SetCmd, unsigned char Startflag);//限速命令
    int  SendCMD(unsigned char *Arrary, int FrameLen);
    void InitCmd(unsigned char *Array, int FrameLen, unsigned char targetCode, unsigned int sendcode,
                 unsigned int StationId);
    void Send_Lone_StationJHto_CTC(JHINFO&iNFO,unsigned char TargetCode);//单站阶段计划
    void Send_MultiPlan_toCTC(jsj_planNode&sendNode,unsigned char TargetCode);//多站阶段计划
    void Send_Resetto_CTC(unsigned int StationId);//站场重置
    void Send_Clock_Same(unsigned int StationId, QDateTime time,unsigned char TargetCode);//时钟同步
    unsigned int GetIdByStationName(QString StationName);
    void Send_ControlCmd_Back(QString StationName, unsigned char before,
                              unsigned char After, unsigned char agreeflag,unsigned char targerCode);//控制模式转换回执
    SendAddress GetAddressByStationId(int StationId);
    QString GetUserControlInfo(unsigned char After);
    QString GetStationName(unsigned int Port);
    void Send_StartSoft(QString Ip, unsigned int Port, QString PathAll, bool isOffLine);
    void Send_ShutCmd_(unsigned int StationId);
private:
    void processTheDatagram(QByteArray array,QHostAddress Address,int Port);
    void GetHeartBeatJudge(int StationId, QHostAddress Address, int Port);
private:
    bool isDeal = false;
    int StationId_;
    QHostAddress Address_;
    int Port_;
private slots:
    void onSocketReadyRead();
    void HeartStartInfo();

};
#define TX_Send_SERVICES (jsj_txService::getDBService())
#endif // JSJ_TXSERVICE_H
