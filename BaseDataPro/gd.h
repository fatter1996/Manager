#ifndef CGD_H
#define CGD_H

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "qd.h"
typedef struct
{
    QPoint pt_Center;  //站台坐标点-横线的中点坐标
    QPoint pt_CenterConst;  //站台坐标点-横线的中点坐标
    int nWidth;    //横线段长度
    int nHeigh;    //两条竖线段高度
    int nLineSize;  //线条粗细
    bool isUP;    //站台开口方向
    QColor cLineColor;
    void init()
    {
        nWidth = nHeigh = nLineSize = 0;
        pt_Center = pt_CenterConst = QPoint(-10, -10);
        isUP = false;
        cLineColor = Qt::green;
    }
} ZT_Data;

class CGD : public CQD
{
public:
    explicit CGD();

    //成员变量声明
private:
    QPointF p1, p2, p3, p4, p12, p34, pz12, pz34;
    QPointF p1Const, p2Const, p3Const, p4Const, p12Const, p34Const, pz12Const, pz34Const;   //缩放及偏移使用
    unsigned int m_nGDType;             //轨道区段类型
    unsigned int m_nZ;                  //折点个数

    bool isGDFLBL;             //是否分录不良
    //int GLB_QDleft;
    //int GLB_QDright;
    //bool isSXDC;
    bool isZXGD;
    bool isCXGD;
    bool isJJGD;
    //bool GD_LCode;
    //bool GD_RCode;
    //QString CarrierFrequency;
    //int Dir_DMH;
    //int m_nCodeFHXH;   //区间闭塞分区小轨道电路关联防护信号机
    //bool isNoShow;
    QString m_strMark1;
    QString m_strMark2;

    bool isHaveGDTW;   //是否有股道停稳
    QString m_strGDTWName;  //股道停稳表示灯名称
    QPoint pt_GDTWBSD;   //股道停稳表示灯坐标
    QPoint pt_GDTWBSDConst;   //股道停稳表示灯坐标
    QPoint pt_GDTWName;  //股道停稳表示灯名称坐标
    QPoint pt_GDTWNameConst;  //股道停稳表示灯名称坐标
    int m_nGDTWTime;     //股道停稳倒计时，0xff表示当前无倒计时
    QVector<ZT_Data>vect_ZTArray;

    //成员变量封装函数声明
public:
    void setGDType(QString str_type);
    unsigned int getGDType();
    void setZ(unsigned int nZ);
    unsigned int getZ();
    void setGDFLBL(bool nFlag);
    bool getGDFLBL();
    void setp1(QPointF pt);
    QPointF getp1();
    void setp2(QPointF pt);
    QPointF getp2();
    void setp3(QPointF pt);
    QPointF getp3();
    void setp4(QPointF pt);
    QPointF getp4();
    void setp12(QPointF pt);
    QPointF getp12();
    void setp34(QPointF pt);
    QPointF getp34();
    void setpz12(QPointF pt);
    QPointF getpz12();
    void setpz34(QPointF pt);
    QPointF getpz34();
    //void setGLB_QDleft(int glb);
    //int getGLB_QDleft();
    //void setGLB_QDright(int glb);
    //int getGLB_QDright();
    //void setIsSXDC(bool flag);
    //bool getIsSXDC();
    void setIsZXGD(bool flag);
    bool getIsZXGD();
    void setIsCXGD(bool flag);
    bool getIsCXGD();
    void setIsJJGD(bool flag);
    bool getIsJJGD();
    //void setGD_LCode(int code);
    //int getGD_LCode();
    //void setGD_RCode(int code);
    //int getGD_RCode();
    //void setCarrierFrequency(QString str);
    //QString getCarrierFrequency();
    //void setDir_DMH(bool flag);
    //int getDir_DMH();

    //void setCodeFHXH(int fhxh);
    //int getCodeFHXH();
    //void setisNoShow(bool flag);
    //bool getisNoShow();
    void setMarkStr1(QString str);
    QString getMarkStr1();
    void setMarkStr2(QString str);
    QString getMarkStr2();
    void setIsHaveGDTW(bool flag);
    bool getIsHaveGDTW();
    void setGDTWName(QString name);
    QString getGDTWName();
    void setPt_GDTWBSD(QPoint pt);
    QPoint getPt_GDTWBSD();
    void setPt_GDTWName(QPoint pt);
    QPoint getPt_GDTWName();
    void setGDTWTime(int x);
    int getGDTWTime();
    void setZTData(QVector<ZT_Data> ztarray);
    void addZTData(ZT_Data ztdata);
    void addZTData(QPoint pt, int wid, int heigh, int linesize, bool isup, QColor color);
    QVector<ZT_Data> getZTData();
    ZT_Data getZTData(int x);


    //成员功能函数声明
public:
    void GDInit(int type); //股道初始化
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx);
    void Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid, QPoint offset);
    void Draw_FLBL_WBX(QPainter *painter, QPointF pt1, QPointF pt2, double nDiploid); //绘制分录不良外包线
    void Draw_ZT(QPainter *painter, long nElapsed, double nDiploid, QPoint offset);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);
    virtual void setDevStateToSafe();

    void DrawTrain(QPainter *painter, long nElapsed, double nDiploid, QPoint offset);
    //void gd_StatePro();
};

#endif // CGD_H
