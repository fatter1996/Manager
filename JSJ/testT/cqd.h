#ifndef CQD_H
#define CQD_H
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include "Global.h"

class CQD
{
public:
    CQD();

public:
    //成员变量申明
    int m_nChildNum; //区段的组件个数
    int m_nChild[4]; //区段的组件数组
    int QD_LCode;  //区段左节点
    int QD_RCode;  //区段右节点
    int FQJ;    //区段方向切换继电器-每个区段都有----UP-正向  DOWN-反向
    int DMH_State;  //区段编码
    QPoint pt;
    QPoint ptConst;
    int DMH_ShowType;  //0-方框样式显示  1-字母样式显示
    QRect m_DrawRect;
    QRect m_DrawRectConst;
    bool isDisDPZPFlag;  //是否显示低频载频信息

    //成员变量封装函数
    void setFQJ(int state);
    int getFQJ();
    void setDMHState(int state);
    int getDMHState();
    void setPt(QPoint p);
    void setDMHShowType(int type);
    int getDMHShowType();
    void setDrawRect(QRect rect);
    void setIsDisDPZPFlag(bool flag);
    bool getIsDisDPZPFlag();

    //功能实现函数
    void Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset);
};

#endif // CQD_H
