#ifndef T_jtx_H
#define T_jtx_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
class T_jtx
{
public:
    T_jtx();

    //成员变量声明
public:
    QPointF p1,p2, p3, p4,pCenter;
    QPoint p1Const, p2Const, p3Const, p4Const,pCenterConst;
    unsigned int m_nType;               //设备类型
    unsigned int m_nJTXType;            //尽头线类型
    QString m_strName;         //尽头线名称
    int m_nStationID;

    //成员变量封装函数
public:
    void setType(unsigned int nType);
    unsigned int getType();
    void setJTXType(unsigned int nType);
    unsigned int getJTXType();
    void setJTXName(QString name);
    QString getJTXName();
    void setStationID(int id);
    int getStationID();
    void setp1(QPoint pt);
    void setp2(QPoint pt);
    void setp3(QPoint pt);
    void setp4(QPoint pt);
    void setpCenter(QPoint pt);

    //成员功能函数实现
public:
    void JTXInit();
    void Draw(QPainter *painter,int nElapsed,float nDiploid);
};

#endif // T_jtx_H
