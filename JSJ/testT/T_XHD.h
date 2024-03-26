#ifndef T_XHD_H
#define T_XHD_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

class T_XHD
{
public:
    explicit T_XHD();

    //成员变量声明
public:
    unsigned int m_nType;
    QString m_strName;   //信号机名称
    unsigned int m_nCode;
    QPointF pCenter;            //设备中心点
    QPointF pCenterConst;       //缩放及偏移使用
    unsigned int m_nXHDType;    //信号机类型
    unsigned int m_nSX;                 //上下行咽喉
    QRectF m_textRect;          //信号机文本区域
    QRectF m_textRectConst;     //预留站场缩放数据存储结构
    bool isSignalType;   //是否是虚拟信号
    unsigned int m_nSafeLamp;   //安全灯关
    bool isHigh;       //是否为高柱
    bool isDisPlayName;        //是否显示名称（该标识符只管单站，不负责多站界面视图）
    unsigned int m_nXHDState;  //信号机状态
    bool isLCBt_Down;
    bool isDCBt_Down;
    double nXHANSizeX;   //信号按钮大小
    double nXHANSizeY;   //信号按钮大小
    double nXHANSizeXConst;   //信号按钮大小
    double nXHANSizeYConst;   //信号按钮大小
    QRect m_rectLC;       //列车按钮区域
    QRect m_rectDC;       //调车按钮区域
    QRect m_rectYD;       //引导按钮区域
    QRect m_rectLCConst;
    QRect m_rectDCConst;
    int m_nStationID;
    bool isYDSD;
    bool signalDCAN;    //是否有单独的调车按钮
    int GLB_XHD;    //公里标
    int m_nGLXHNode;
    QString Dir_XH;
    int XH_LCode;
    int XH_RCode;
    bool isHDDSJC;
    int XH_WZInfo;
    int XH_Show;
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
    bool XHFSState;//信号封锁状态
    bool XHisPLCZ;//批量操作

    unsigned char showtype;
    //成员变量封装函数声明
public:
    void setType(unsigned int nType);
    unsigned int getType();
    void setXHDName(QString string);
    QString getXHDName();
    void setCode(unsigned int code);
    unsigned int getCode();
    void setXHDType(QString strType);
    unsigned int getXHDType();
    void setSX(unsigned int nSX);
    unsigned int getSX();
    void setSignalType(bool flag);
    bool getSignalType();
    void setSafeLamp(QString strSafelamp);
    unsigned int getSafeLamp();
    void setIsHigh(bool ishigh);
    bool getIsHigh();
    void setDisplayName(bool nFlag);
    bool getDisplayName();
    void setXHDState(unsigned int state);
    unsigned int getXHDState();
    void setIsLCDown(bool nFlag);
    bool getIsLCDown();
    void setIsDCDown(bool nFlag);
    bool getIsDCDown();
    void setStationID(int id);
    int getStationID();
    void setTextRect(QRect rect);
    void setIsYDSD(bool flag);
    bool getIsYDSD();
    void setLCAN_Rect(QRect rect);
    void setDCAN_Rect(QRect rect);
    void setYDAN_Rect(QRect rect);
    void setSignalDCAN(bool flag);
    bool getSignalDCAN();
    void setGLB_XHD(int glb);
    int getGLB_XHD();
    void setGLXHNode(int node);
    int getGLXHNode();
    void setDir_XH(QString dir);
    QString getDir_XH();
    void setXH_LCode(int code);
    int getXH_LCode();
    void setXH_RCode(int code);
    int getXH_RCode();
    void setisHDDSJC(bool flag);
    bool getisHDDSJC();
    void setXH_WZInfo(QString info);
    int getXH_WZInfo();
    void setXH_Show(int show);
    int getXH_Show();
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
    void setpCenter(QPoint pt);
    void clearState();
    //功能函数声明
public:
    void XHDInit(); //信号灯初始化
    void Draw(QPainter *painter, unsigned int nElapsed, double nDiploid, unsigned char type);
    void setXHD_Color(QColor *xhd1_Color, QColor *xhd2_Color, unsigned int nElapsed);

    //鼠标响应函数
    int MouseMove(QPoint pt);
signals:
};

#endif // T_XHD_H
