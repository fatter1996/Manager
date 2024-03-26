#ifndef CXHD_H
#define CXHD_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "BaseData.h"
class CXHD : public CBaseData
{
public:
    explicit CXHD();

    //成员变量声明
private:
    unsigned int m_nXHDType;    //信号机类型
    bool isSignalType;   //是否是虚拟信号
    unsigned int m_nSafeLamp;   //安全灯光
    bool isHigh;       //是否为高柱
    unsigned int m_nXHDState;  //信号机状态
    //unsigned int m_nXHD_lsState;  //信号机状态
    bool isLCBt_Down;
    bool isDCBt_Down;
    bool isYDBt_Down;
    double nXHANSize;   //信号按钮大小
    double nXHANSizeConst;   //信号按钮大小
    QRectF m_rectLC;       //列车按钮区域
    QRectF m_rectDC;       //调车按钮区域
    QRectF m_rectYD;       //引导按钮区域
    QRectF m_rectLCConst;
    QRectF m_rectDCConst;
    QRectF m_rectYDConst;
    bool isYDSD;
    bool signalDCAN;    //是否有单独的调车按钮
    bool XH_LCSD;
    bool XH_LCZD;
    bool XH_DCSD;
    bool XH_DCZD;
    bool XH_YXSD;
    bool XH_YXZD;
    bool XH_CYFX;
    bool XH_BGAN;
    bool XH_PDJZ;
    bool XH_LCZH;
    QRect xhd_Rect;
    bool isMD;
    bool isLCANFB;
    bool isDCANFB;
    bool isYDANFB;
    int m_nTimeType;   //0x11-无倒计时  0x22-人解30s  0x33-人解180s   0x44-坡道解锁180s  0x55-引导首区段故障保持15s
    int m_nTimeCount;  //0xFF-无倒计时  其他-实时值
    int m_nTimePosType; //计时显示位置类型  0-覆盖名称 1-右 2-上 3-左 4-下
    unsigned char CmdButtonFlag;//存储的命令 下发后可排列进路 0x11-列车按钮  0x22-调车按钮  0-不是存储的按钮
    bool ZDBSJKFlag;
    //成员变量封装函数声明
public:
    void setZDBSJKFlag(bool flag);
    bool getZDBSJKFlag();
    void setXHDType(QString strType);
    unsigned int getXHDType();
    void setSignalType(bool flag);
    bool getSignalType();
    void setSafeLamp(QString strSafelamp);
    unsigned int getSafeLamp();
    void setIsHigh(bool ishigh);
    bool getIsHigh();
    void setXHDState(unsigned int state);
    unsigned int getXHDState();
    void setIsLCDown(bool nFlag);
    bool getIsLCDown();
    void setIsDCDown(bool nFlag);
    bool getIsDCDown();
    void setIsYDDown(bool nFlag);
    bool getIsYDDown();
    void setIsYDSD(bool flag);
    bool getIsYDSD();
    void setLCAN_Rect(QRectF rect);
    void setDCAN_Rect(QRectF rect);
    void setYDAN_Rect(QRectF rect);
    void setSignalDCAN(bool flag);
    bool getSignalDCAN();
    void setXH_LCSD(bool flag);
    bool getXH_LCSD();
    void setXH_LCZD(bool flag);
    bool getXH_LCZD();
    void setXH_DCSD(bool flag);
    bool getXH_DCSD();
    void setXH_DCZD(bool flag);
    bool getXH_DCZD();
    void setXH_YXSD(bool flag);
    bool getXH_YXSD();
    void setXH_YXZD(bool flag);
    bool getXH_YXZD();
    void setXH_CYFX(bool flag);
    bool getXH_CYFX();
    void setXH_BGAN(bool flag);
    bool getXH_BGAN();
    void setXH_PDJZ(bool flag);
    bool getXH_PDJZ();
    void setXH_LCZH(bool flag);
    bool getXH_LCZH();
    void setIsMD(bool flag);
    bool getIsMD();
    void setIsLCANFB(bool flag);
    bool getIsLCANFB();
    void setIsDCANFB(bool flag);
    bool getIsDCANFB();
    void setIsYDANFB(bool flag);
    bool getIsYDANFB();
    void setTimeType(int x);
    int getTimeType();
    void setTimeCount(int x);
    int getTimeCount();
    void setTimePosType(int x);
    int getTimePosType();

    void setCmdButtonFlag(unsigned anType);
    unsigned char getCmdButtonFlag();

    //功能函数声明
public:
    void XHDInit(int type); //信号灯初始化
    void XHDInit_Th_kb(); //标准界面信号灯初始化
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx);
    void Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid, QPoint offset);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);
    void setXHD_Color(QColor *xhd1_Color, QColor *xhd2_Color, int nElapsed);
    void xhd_StatePro();
    virtual void setDevStateToSafe();

    void setXHD_StateOfLS(int state1, int state2, int state3, int state4);
    void getXHD_StateForCTC(int *state);
};

#endif // CXHD_H
