#ifndef TStation_H
#define TStation_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "TGlobal.h"
#include"T_GDDC.h"
#include "T_XHD.h"
#include "T_GD.h"
#include "T_Text.h"
#include "T_jtx.h"
#include "T_YDQ.h"

typedef struct
{
    QVector<T_XHD*> v_AN;
    QVector<T_GDDC*> v_DC;
    QVector<T_GD*> v_QD;
    int ExamID;
    int XHXS;//信号显示
    int CODE;//驱动场景编号
    bool Reopen;
    unsigned state;
    void Init()
    {
        v_AN.clear();
        v_DC.clear();
        v_QD.clear();
        XHXS = 0;
        state = 0;
        ExamID = -1;
        Reopen = false;
    }
    void JLInit()
    {
        Reopen = false;
        state = 0;
        for (int i = 0; i < v_DC.size(); i++)
        {
            v_DC[i]->setQDColor(BLUE);
        }
        for (int i = 0; i < v_QD.size(); i++)
        {
            v_QD[i]->setQDColor(BLUE);
        }
        XHXS = 0;
    }
} JLStr;
typedef struct
{
    int TitleID;
    QVector<T_XHD*> v_AN;
    QVector<QString> v_XHState;
    QVector<T_GDDC*> v_DC;
    QVector<QString> v_DCState;
    QVector<T_GD*> v_QD;
    QVector<QString> v_QDState;
    void Init()
    {
        TitleID = -1;
        v_AN.clear();
        v_XHState.clear();
        v_DC.clear();
        v_DCState.clear();
        v_QD.clear();
        v_QDState.clear();
    }
} SceneStr;
class TStation : public QObject
{
    Q_OBJECT
public:
    explicit TStation(QObject *parent = nullptr);

    //成员变量声明
public:
    QString StationName = "";//站名 同配置文件名称
    QVector<T_GDDC*>GDDCArray;   //信号设备数据数组
    QVector<T_XHD*>XHDArray;   //信号设备数据数组
    QVector<T_GD*>GDArray;    //轨道设备数据数组
    QVector<T_Text>TextArray;    //界面文本数据数组
    QVector<T_jtx>JTXArray;    //界面文本数据数组
    QVector<T_YDQ>YDQArray;    //界面文本数据数组
    QString m_strStationName;   //站名
    typedef struct
    {
        unsigned char type;
        int selected;
        void Init()
        {
            type = 0;
            selected = -1;
        }
    } SelectedStr;
    typedef  struct
    {
        int m_nChildNum; //区段的组件个数
        unsigned int m_nChild[4]; //区段的组件数组
    } QD_Str;
    QVector<QD_Str>QDArray;

    int m_nStationID;
    int m_nStationType;
    QString m_strLS_Producer;
    bool isGT;
    bool isZCZX;
    int m_nRBCCode;
    QPoint offsetPt;

    SelectedStr m_Selected;

    QVector<JLStr> v_JL;
    JLStr CurrentJL;//当前进路
    QVector<T_XHD*> v_ANArray;
    QVector<SceneStr> v_SceneArray;

    QVector<T_XHD*> v_CTCANArray;//CTC按钮暂存

    bool cancelFlag;
    QTimer *CTCTimer;


    bool QDNameFlag; // true 为显示 false为隐藏
    bool DCNameFlag; // true 为显示 false为隐藏
    bool XHNameFlag; // true 为显示 false为隐藏

    //成员变量封装函数声明
public:
    void setStationName(QString name);   //设置站名
    QString getStationName();            //获取站名
    void setStationID(int id);
    int getStationID();
    void setStationType(int type);
    int getStationType();
    void setLSProducer(QString producer);
    QString getLSProducer();
    void setIsGT(bool flag);
    bool getIsGT();
    void setIsZCZX(bool flag);
    bool getIsZCZX();
    void setRBCCode(int rbcCode);
    int getRBCCode();
    void setOffSet(QPoint offset);
    int getOffSetX();
    int getOffSetY();

    void setScene(int TitleID);
    void InitAllFactor();
    void JLConfigRead(QString FileName);
    void SceneRead(QString FileName);

    T_XHD* GetXHByName(QString name);
    T_GDDC* GetGDDCByName(QString name);
    T_GD* GetGDByName(QString name);
    void JLProcOnTimer();
    void HandleANOnTimer();
    void DeaLWithAN(T_XHD *pXHD);
    void SetCancle();
    void CancelCurrentJL();
    void ConvertCTCCmd(QVector<T_XHD *> v_ANArray);
    void ClearScene();

    void GetSelectDevFun(QRect Rect,  QStringList *QJdev, QStringList *DCdev, QStringList *XHdev, QStringList *QDdev);
//    void ClearSelectState();

//    bool IsIcludeRect(QRect Includerect,QRect Judgerect);
//    void ReadTextData(QString FileName);
//    int HexStringToInt(QString str, int length); //将十六进制表示的字符串转换为整型数据
//    QPoint StringToPoint(QString str);
//    QRect StringToRect(QString str);
//    int StringGLBToInt(QString str); //公里标字符串转换为米单位整型
    //功能函数声明
public:
    void stationInit();
    void drawStation(QPainter *painter, int nElapsed, double nDiploid, unsigned char type);
    void setGD();
    void setMainGD();
signals:
    void SendCaseCode(int code);
private slots:
    void PressCTCButton();

    void QDNameShowOrHide();
    void DCNameShowOrHide();
    void XHNameShowOrHide();
};

#endif // TStation_H
