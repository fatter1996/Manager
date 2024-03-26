#ifndef CZDBS_H
#define CZDBS_H

#include "BaseData.h"
typedef struct
{
    QString strAN_Name;  //为空表示默认
    int nAN_Type;     //0-正方形按钮
    int nNamePos_Type;  //0-按钮下方  1-按钮上方   2-按钮左边  3-按钮右边
    int nNameSize;
    QColor cNameColor;
    void init()
    {
        strAN_Name="";
        nAN_Type=nNamePos_Type=nNameSize=0;
        cNameColor=Qt::white;
    }
    void init(QString name)
    {
        strAN_Name=name;
        nAN_Type=nNamePos_Type=nNameSize=0;
        cNameColor=Qt::white;
    }
}BtnInfo;

class CZDBS: public CBaseData
{
public:
    explicit CZDBS();

    //成员变量声明
private:
    int m_nGLXH;
    QVector<int>vect_JJQD;
    bool isNX;
    QPoint ArrowPt;
    QPoint ArrowPtConst;
    int m_nAnNum;
    QPoint p_GFAN;
    QPoint p_GFANConst;
    QPoint p_ZFZAN;
    QPoint p_ZFZANConst;
    QPoint p_FCFZAN;
    QPoint p_FCFZANConst;
    QPoint p_JCFZAN;
    QPoint p_JCFZANConst;
    QPoint p_MN;
    QPoint p_MNConst;

    QRect rect_GF;
    QRect rect_ZFZ;
    QRect rect_FCFZ;
    QRect rect_JCFZ;
    bool isDown_GF;
    bool isDown_ZFZ;
    bool isDown_FCFZ;
    bool isDown_JCFZ;
    int m_nDownTime_FCFZ;   //0xFF时不显示
    int m_nDownTime_JCFZ;

    QPoint p_BSD_YXFC; //允许发车灯
    QPoint p_BSD_YXFCConst; //允许发车灯
    QPoint p_BSD_FZ;  //辅助表示灯
    QPoint p_BSD_FZConst;  //辅助表示灯
    QPoint p_BSD_SG;  //闪光表示灯
    QPoint p_BSD_SGConst;  //闪光表示灯
    QPoint p_BSD_QJ;  //区间表示灯
    QPoint p_BSD_QJConst;  //区间表示灯
    QRect rect_FK;
    QRect rect_FKConst;

    int BSDState_YXFC;
    int BSDState_SG;
    int BSDState_FZ;
    int BSDState_QJ;
    int BSDState_QG;

    int m_nArrowStateJC;
    int m_nArrowStateFC;

    BtnInfo GFAN_info;
    BtnInfo ZFZAN_info;
    BtnInfo FCFZAN_info;
    BtnInfo JCFZAN_info;
public:
    void setGLXH(int x);
    int getGLXH();
    void addVectJJQD(int x);
    void setVectJJQD(QVector<int> x);
    QVector<int> getVectJJQD();
    void setIsNX(bool flag);
    bool getIsNX();
    void setArrowPt(QPoint pt);
    QPoint getArrowPt();
    void setAnNum(int x);
    int getAnNum();
    void setGFANPt(QPoint pt);
    QPoint getGFANPt();
    void setZFZANPt(QPoint pt);
    QPoint getZFZANPt();
    void setFCFZANPt(QPoint pt);
    QPoint getFCFZANPt();
    void setJCFZANPt(QPoint pt);
    QPoint getJCFZANPt();
    void setMNPt(QPoint pt);
    QPoint getMNPt();
    void setIsDown_GF(bool flag);
    bool getIsDown_GF();
    void setIsDown_ZFZ(bool flag);
    bool getIsDown_ZFZ();
    void setIsDown_FCFZ(bool flag);
    bool getIsDown_FCFZ();
    void setIsDown_JCFZ(bool flag);
    bool getIsDown_JCFZ();
    void setDownTime_FCFZ(int x);
    int getDownTime_FCFZ();
    void setDownTime_JCFZ(int x);
    int getDownTime_JCFZ();
    void setBSD_YXFCPt(QPoint pt);
    QPoint getBSD_YXFCPt();
    void setBSD_SGPt(QPoint pt);
    QPoint getBSD_SGPt();
    void setBSD_FZPt(QPoint pt);
    QPoint getBSD_FZPt();
    void setBSD_QJPt(QPoint pt);
    QPoint getBSD_QJPt();
    void setRect_FK(QRect rect);
    QRect getRect_FK();
    void setBSDState_YXFC(int state);
    int getBSDState_YXFC();
    void setBSDState_SG(int state);
    int getBSDState_SG();
    void setBSDState_FZ(int state);
    int getBSDState_FZ();
    void setBSDState_QJ(int state);
    int getBSDState_QJ();
    void setBSDState_QG(int state);
    int getBSDState_QG();
    void setArrowStateJC(int x);
    int getArrowStateJC();
    void setArrowStateFC(int x);
    int getArrowStateFC();
    void setGFAN_info(QString name,int size,int postype,int antype,QColor color);
    void setZFZAN_info(QString name,int size,int postype,int antype,QColor color);
    void setFCFZAN_info(QString name,int size,int postype,int antype,QColor color);
    void setJCFZAN_info(QString name,int size,int postype,int antype,QColor color);

public:
    virtual void Draw(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,int type,bool zczx);
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void Draw_Th_kb(QPainter *painter,long nElapsed, double nDiploid,QPoint offset,bool zczx);

    void setZDBS_StateOfLS(int state1,int state2,int state3,int state4,int state5);
    void getZDBS_StateForCTC(int *state1,int *state2,int *state3,int *state4,int *state5);
};

#endif // CZDBS_H
