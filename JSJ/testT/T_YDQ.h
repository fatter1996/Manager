#ifndef CYDQ_H
#define CYDQ_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

class T_YDQ
{
public:
    T_YDQ();

    //成员变量声明
public:
    int m_nStationID;   //所属车站ID
    int m_nType;      //设备类型
    QString m_strName;    //设备名称
    int m_nCode;     //应答器设备编号
    int m_nNumDQ;    //所属大区编号
    int m_nNumFQ;    //所属分区编号
    int m_nNumCZ;    //所属车站编号
    int m_nNumDY;    //应答器单元编号
    int m_nNumZN;    //组内编号
    QString m_strName_Record;   //档案编号
    int m_nFuncType;    //功能类型
    int m_nGLQDNode;    //所在区段设备编号
    int m_nPYL;         //所属区段偏移量
    int m_nYDQType;     //应答器类型
    int GLB_YDQ;        //公里标
    QPoint p;
    bool isDisPlayName;        //是否显示名称（该标识符只管单站，不负责多站界面视图）

    //成员变量封装函数声明
public:
    void setStationID(int id);
    int getStationID();
    void setType(int nType);
    int getType();
    void setYDQName(QString name);
    QString getYDQName();
    void setCode(int code);
    int getCode();
    void setNumDQ(int num);
    int getNumDQ();
    void setNumFQ(int num);
    int getNumFQ();
    void setNumCZ(int num);
    int getNumCZ();
    void setNumDY(int num);
    int getNumDY();
    void setNumZN(int num);
    int getNumZN();
    void setName_Record(QString recordname);
    QString getName_Record();
    void setFuncType(QString functype);
    int getFuncType();
    void setGLQDNode(int node);
    int getGLQDNode();
    void setPYL(int pyl);
    int getPYL();
    void setYDQType(QString type);
    int getYDQType();
    void setGLB_YDQ(int glb);
    int getGLB_YDQ();
    void setYDQp(QPoint pt);
    void setDisplayName(bool nFlag);
    bool getDisplayName();

    //成员功能函数声明
public:
    void Draw(QPainter *painter, unsigned int nElapsed, double nDiploid);
};

#endif // CYDQ_H
