#ifndef JMGLOBALSTRUCTURE_H
#define JMGLOBALSTRUCTURE_H

#include "GlobalHeaders/GlobalFuntion.h"
#include <QPainter>
#include <QPushButton>
//标准界面功能按钮数据结构声明
typedef  struct
{
    QString name1;
    QString name2;
    bool isHaveQF;
    int nFunCode;
    bool isDown;
    int nDownTime;
    int stationID;
    QRect rect_Btn;

    void init()
    {
        name1=name2="";
        isHaveQF=nFunCode=isDown=stationID=nDownTime=0;
        rect_Btn.setRect(0,0,0,0);
    }
}FunBtnStr;

typedef struct
{
    QString strName;
    int nQfCount;
    int nQfType;   //铅封类型  0x11-功能按钮    0x22-非常站控
    int nIndex;    //对应位置
    void init()
    {
        strName="";
        nQfCount=nQfType=nIndex=0;
    }
}QfListItem;

typedef struct
{
    QString strName;   // 名称为"9999"时表示改表示灯无名称
    int nNameSize;
    int nNameShowType;   //0-表示灯下方  1-表示灯上方  2-表示灯左边  3-表示灯右边
    QPoint pt;
    QPoint ptConst;
    QColor cBSD_Color;
    QColor cName_Color;
    void init()
    {
        strName="";
        nNameSize=9;
        nNameShowType=0;
        pt=ptConst=QPoint(-10,-10);
        cBSD_Color=Qt::black;
        cName_Color=Qt::white;
    }
}LSD_Data;

typedef struct
{
    QPoint pt1;
    QPoint pt2;
    QPoint pt1Const;
    QPoint pt2Const;
    int nLineType;
    int nLineSize;
    QColor cLineColor;
    void init()
    {
        nLineType=nLineSize=0;
        pt1=pt1Const=pt2=pt2Const=QPoint(-10,-10);
        cLineColor=Qt::white;
    }
}Line_Data;

typedef struct
{
    QPoint pt;
    QPoint ptConst;
    QString str_Name;
    QString str_FontName;
    int nFontSize;
    QColor cFontColor;
    void init()
    {
        str_Name="";
        str_FontName="宋体";
        nFontSize=0;
        pt=ptConst=QPoint(-10,-10);
        cFontColor=Qt::white;
    }
}Text_Data;
#endif // JMGLOBALSTRUCTURE_H
