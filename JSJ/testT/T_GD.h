#ifndef T_GD_H
#define T_GD_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

class T_GD
{
public:
    T_GD();

    //成员变量声明
public:
    QPointF p1, p2, p3, p4, p12, p34, pz12, pz34;
    QPointF p1Const, p2Const, p3Const, p4Const, p12Const, p34Const, pz12Const, pz34Const;   //缩放及偏移使用
    QPointF pCenter;            //设备中心点
    QPointF pCenterConst;       //缩放及偏移使用
    unsigned int m_nType;               //设备类型
    unsigned int m_nGDType;             //轨道区段类型
    QString m_strName;         //轨道名称
    unsigned int m_nZ;                  //折点个数
    unsigned int m_nSX;                 //上下行咽喉
    unsigned int m_nCode;               //设备编号
    bool isDisPlayName;        //是否显示名称
    QColor gdColor;          //轨道区段颜色
    bool isLostTrainNum;       //是否列车丢失--暂时跟着车次走，先预留
    bool isLock;               //是否封锁
    bool isGDFLBL;             //是否分录不良
    QRectF m_textRect;          //轨道区段文本区域
    QRectF m_textRectConst;     //预留站场缩放数据存储结构
    int m_nStationID;
    int GLB_QDleft;
    int GLB_QDright;
    bool isSXDC;
    bool isZXGD;
    bool isCXGD;
    bool isJJGD;
    bool GD_LCode;
    bool GD_RCode;
    QString CarrierFrequency;
    QString Dir_DMH;
    bool GDPowerSupply;//股道供电
    bool GDFSState;//股道封锁状态
    bool GDisPLCZ;//批量操作

    //成员变量封装函数声明
public:
    void setType(unsigned int nType);
    unsigned int getType();
    void setGDType(QString str_type);
    unsigned int getGDType();
    void setGDName(QString name);
    QString getGDName();
    void setZ(unsigned int nZ);
    unsigned int getZ();
    void setSX(unsigned int nSX);
    unsigned int getSX();
    void setCode(unsigned int code);
    unsigned int getCode();
    void setDisplayName(bool nFlag);
    bool getDisplayName();
    void setQDColor(QColor color);
    QColor getQDColor();
    void setLostTraNum(bool nFlag);
    bool getLostTraNum();
    void setLock(bool nFlag);
    bool getLock();
    void setGDFLBL(bool nFlag);
    bool getGDFLBL();
    void setStationID(int id);
    int getStationID();
    void setp1(QPoint pt);
    void setp2(QPoint pt);
    void setp3(QPoint pt);
    void setp4(QPoint pt);
    void setp12(QPoint pt);
    void setp34(QPoint pt);
    void setpz12(QPoint pt);
    void setpz34(QPoint pt);
    void setpCenter(QPoint pt);
    void setTextRect(QRect rect);
    void setGLB_QDleft(int glb);
    int getGLB_QDleft();
    void setGLB_QDright(int glb);
    int getGLB_QDright();
    void setIsSXDC(bool flag);
    bool getIsSXDC();
    void setIsZXGD(bool flag);
    bool getIsZXGD();
    void setIsCXGD(bool flag);
    bool getIsCXGD();
    void setIsJJGD(bool flag);
    bool getIsJJGD();
    void setGD_LCode(int code);
    int getGD_LCode();
    void setGD_RCode(int code);
    int getGD_RCode();
    void setCarrierFrequency(QString str);
    QString getCarrierFrequency();
    void setDir_DMH(QString str);
    QString getDir_DMH();

    //成员功能函数声明
public:
    void GDInit(); //股道初始化
    void Draw(QPainter *painter, unsigned int nElapsed, double nDiploid);
};

#endif // T_GD_H
