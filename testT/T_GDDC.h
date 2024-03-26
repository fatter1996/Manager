#ifndef T_GDDC_H
#define T_GDDC_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

class T_GDDC
{
public:
    T_GDDC();

    //成员变量声明
public:
    unsigned int m_nType;      //设备类型
    QString m_strName;         //轨道名称
    unsigned int m_nSX;        //上下行咽喉
    unsigned int m_nCode;      //设备编号
    unsigned int m_nQDCode;    //关联区段设备编号
    QRectF m_textRect;          //轨道区段文本区域
    QRectF m_textRectConst;     //预留站场缩放数据存储结构
    unsigned int m_nCxjy;      //超限绝缘节
    unsigned int m_nZ;         //折点
    unsigned int m_nJyj;       //绝缘节
    QPointF p1, p2, p3, p4, p5, p6, p12, p34, p56, pz12, pz34, pz56, pCenter;
    QPointF p1Const, p2Const, p3Const, p4Const, p5Const, p6Const, p12Const, p34Const, p56Const, pz12Const, pz34Const, pz56Const, pCenterConst;
    int m_nQ;        //岔前设备
    int m_nD;        //定位设备
    int m_nF;        //反位设备
    bool isDisPlayName;        //是否显示名称
    bool isMainGD;
    unsigned int m_nDCWZ;
    QColor gdColor;          //轨道区段颜色
    QPointF p34C, p56C;   //用于存放岔心关键坐标，不从数据读取，而是再init中计算得到
    QPointF p34CConst, p56CConst;
    int m_nSDDCCode;
    QString m_TrainNum;
    bool m_TrainDown;
    bool m_bDrawGD;
    int m_nStationID;
    bool isSDDC;
    int m_nOneToMore;
    int m_nSameQDDCCode;
    bool isWGDW;
    int GLB_DCCQ;    //公里标
    int GLB_DCCX;    //公里标
    int GLB_DCDW;    //公里标
    int GLB_DCFW;    //公里标
    bool isTSDC;
    QString CarrierFrequency;
    QString Dir_DMH;
    bool DCPowerSupply;//道岔供电
    bool DCFSState;//道岔封锁状态//
    bool DCisPLCZ;//批量操作

    //成员封装函数声明
public:
    void setType(unsigned int type);
    unsigned int getType();
    void setGDDCName(QString name);
    QString getGDDCName();
    void setSX(unsigned int nSX);
    unsigned int getSX();
    void setCode(unsigned int code);
    unsigned int getCode();
    void setQDCode(unsigned int code);
    unsigned int getQDCode();
    void setCxjy(unsigned int nCxjy);
    bool getCxjy(unsigned int nCxjy);
    void setZ(unsigned int nZ);
    bool getZ(unsigned int nZ);
    void setJyj(unsigned int nJyj);
    bool getJyj(unsigned int nJyj);
    void setCQdev(int dev);
    int getCQdev();
    void setDWdev(int dev);
    int getDWdev();
    void setFWdev(int dev);
    int getFWdev();
    void setDisplayName(bool nFlag);
    bool getDisplayName();
    void setIsMainGD(bool nFlag);
    bool getIsMainGD();
    void setDCWZ(unsigned int dcwz);
    unsigned int  getDCWZ();
    void setQDColor(QColor color);
    QColor getQDColor();
    void setSDDCCode(int sddc_code);
    int getSDDCCode();
    int setDrawGD(bool bDrawGD);
    void setStationID(int id);
    int getStationID();
    void setIsSDDC(bool flag);
    bool getIsSDDC();
    void setOneToMore(int onetomore);
    int getOneToMore();
    void setTextRect(QRect rect);
    void setp1(QPoint pt);
    void setp2(QPoint pt);
    void setp3(QPoint pt);
    void setp4(QPoint pt);
    void setp5(QPoint pt);
    void setp6(QPoint pt);
    void setp12(QPoint pt);
    void setp34(QPoint pt);
    void setp56(QPoint pt);
    void setpz12(QPoint pt);
    void setpz34(QPoint pt);
    void setpz56(QPoint pt);
    void setpCenter(QPoint pt);
    void setSameQDDCCode(int code);
    int getSameQDDCCode();
    void setIsWGDW(bool flag);
    bool getIsWGDW();
    void setGLB_DCCQ(int glb);
    int getGLB_DCCQ();
    void setGLB_DCCX(int glb);
    int getGLB_DCCX();
    void setGLB_DCDW(int glb);
    int getGLB_DCDW();
    void setGLB_DCFW(int glb);
    int getGLB_DCFW();
    void setIsTSDC(bool flag);
    bool getIsTSDC();
    void setCarrierFrequency(QString str);
    QString getCarrierFrequency();
    void setDir_DMH(QString str);
    QString getDir_DMH();

    //成员功能函数声明
public:
    void GDDCInit(); //轨道道岔初始化
    void Draw(QPainter *painter, unsigned int nElapsed, double nDiploid);
    void setDCColor(QColor *cq_Color, QColor *dw_Color, QColor *fw_Color); //设置道岔绘制颜色
};

#endif // T_GDDC_H
