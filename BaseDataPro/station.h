#ifndef CSTATION_H
#define CSTATION_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "BaseData.h"
#include "qd.h"
#include "xhd.h"
#include "gd.h"
#include "gddc.h"
#include "text.h"
#include "tg.h"
#include "zdbs.h"
#include "jtx.h"
#include "lzzdbs.h"
#include "BS.h"

typedef struct
{
    int ANCode;
    int ANType;
    int ANTime;
} JLAN;

class CStation: public QObject
{
    Q_OBJECT
public:
    explicit CStation(QObject *parent = nullptr);

    //成员变量声明
public:
    QVector<CBaseData *>DevArray;   //站场设备数组
    QVector<CLZZDBS*> v_LZBZDArray;
    QVector<CBS*> v_LZBSArray;
private:
    QString m_strStationName;   //站名
    unsigned int m_nStationID;
    unsigned int m_nStationType;   //0x99-区间
    QString m_strLS_Producer;
    bool isGT;
    bool isZCZX;
    QPoint offsetPt;
    int m_nCTCSoftID;
    int m_nLS_ProcessID;  //联锁进程唯一识别号
    int m_nLS_SoftID;    //联锁软件ID
    //bool isLSProcessFlag;  //联锁进程响应请求标识
    int m_nXLMNJSoftID;  //对应线路模拟机软件ID
    bool isLSComState;  //联锁通信状态标识
    int LSComTime;   //与联锁持续无通信周期计数器
    bool isSYHZSB;
    bool isXYHZSB;
    bool jtgdFlag;
    int nJTGDType;   //接通光带类型  0-延时后自动消失光带  1-需要再次点压接通光带以清除光带显示
    int nJTGDTime;   //接通光带自动延时 单位：s
    int nJTGDTimeCount;   //接通光带自动延时计数 单位：s
    bool isGDNameIncludeWCQD; //0-无岔区段名称隐藏显示不包括股道   1-股道名称也算无岔区段名称
    JLAN JLANArray[20];
    bool isSDJSFlag;

private:
    void add_forJLAN(unsigned int code, unsigned int type, unsigned int time);
    void anylzJLAN(unsigned char *REC_data);
    void JLANPro();

    //成员变量封装函数声明
public:
    bool LZZDBSFlag;
    bool LZBZDFlag;
    void setStationName(QString name);   //设置站名
    QString getStationName();            //获取站名
    void setStationID(unsigned int id);
    unsigned int getStationID();
    void setStationType(unsigned int type);
    unsigned int getStationType();
    void setLSProducer(QString producer);
    QString getLSProducer();
    void setIsGT(bool flag);
    bool getIsGT();
    void setIsZCZX(bool flag);
    bool getIsZCZX();
    void setOffSet(QPoint offset);
    QPoint getOffSet();
    void setCTCSoftID(int x);
    int getCTCSoftID();
    void setLSProcessID(int x);
    int getLSProcessID();
    void setLSSoftID(int x);
    int getLSSoftID();
    //void setLSProcessFlag(bool flag);
    //bool getLSProcessFlag();
    void setXLMNJSoftID(int x);
    int getXLMNJSoftID();
    void setLSComState(bool flag);
    bool getLSComState();
    void setLSComTime(int x);
    int getLSComTime();
    void setIsSYHZSB(bool flag);
    bool getIsSYHZSB();
    void setIsXYHZSB(bool flag);
    bool getIsXYHZSB();
    void setIsJtgd(bool flag);
    bool getIsJtgd();
    void setJTGD_Type(int x);
    int getJTGD_Type();
    void setJTGD_Time(int x);
    int getJTGD_Time();
    void setJTGDTimeCount(int x);
    int getJTGD_TimeCount();
    void setIsGDNameIncludeWCQD(bool flag);
    bool getIsGDNameIncludeWCQD();

    //功能函数声明
public:
    void set_ZCReset();
    void setQDZYFun(CQD *qd, bool flag);
    void setQDGZFun(CQD *qd, bool flag);
    CQD* getQDFromGDDCNode(int code);
    int getDCNodeFromDCName(QString name);

    void stationInit(int dymType);
    void volloverPro();
    void setDevStateToSafe();
    void drawStation(QPainter *painter, long nElapsed, double nDiploid, int JmType);
    void clearDrawGD();
    void setGD();
    void setMainGD();

    int packDev_StateForCTC(QByteArray &msg, int index, int modeType);
    void OperationalOfCTC(unsigned char *array);
    void resolutionDevStateOfLS(unsigned char *array);


    void setQD_StateOfLS(CQD *qd, int state1, int state2, int state3);
    void setQD_Color(CQD *qd, QColor color);
    void setYHZS_StateOfLS(int state);

    void timePro(long nElapsed);
public slots:
    void setShow_Menu(int type);  //响应系统菜单
signals:
    void signal_YHZSBState(unsigned char yhzs_State, int stationID);
    void signal_sendCmdOfCTC(int anFunType, int staID, int lsProID, int lsSoftID, int devCode1, int devCode2, int anType);
    void signal_ANCmdOfCTC(int anFunType, int devCode1, int devCode2);
};

#endif // CSTATION_H
