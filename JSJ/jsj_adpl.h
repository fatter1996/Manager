﻿#ifndef JSJ_ADPL_H
#define JSJ_ADPL_H

#include <QObject>
#include<QPushButton>
#include<QLabel>
#include <QWidget>
#include<QButtonGroup>
#include"jsj_services.h"
#include"jsj_plannode.h"



class QHBoxLayout;
class QVBoxLayout;

struct GetNodeAppend{
    unsigned int isfirst_or_LastOne;//判断是第一个还是最后一个
    QPushButton * Btn;//当前所填写阶段计划的站场按钮
    QLabel * GetNextlead;//获取下一站场阶段计划的站场按钮
    int LeadIndex = -1;//箭头布局位置
    QHBoxLayout * NextHboxLayout;//获取下一站场阶段计划的组
    unsigned int direct = -1;//来自上方或下方的信息 0x02 往下走 0x01 往上走
};
class jsj_adpl : public QObject
{
    Q_OBJECT
public:
    explicit jsj_adpl(QObject *parent = nullptr);
    ~jsj_adpl();
    void AddStaion(QString StationName,unsigned int Position,unsigned int index);
    void Get_YindaoLine_Show(QPushButton * Btn);
    unsigned int GetAhead_twards(int index);
public:
    jsj_planNode *Node;
private:
    unsigned int CurDirect = -1;//当前方向
    QVBoxLayout * layout;
    QButtonGroup * group;
    QList<GetNodeAppend>NodeAppendList;
protected:
    bool eventFilter(QObject *target, QEvent *event);
signals:

};

#endif // JSJ_ADPL_H
