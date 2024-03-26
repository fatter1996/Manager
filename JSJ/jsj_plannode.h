#ifndef JSJ_PLANNODE_H
#define JSJ_PLANNODE_H

#include <QObject>
#include"jsj_services.h"
#define Down        0x01
#define YZZX        0x02
#define Zxed        0x03

#define SS_SF       0x01                                //所属_始发
#define SS_JF       0x02                                //所属_接发
#define SS_ZD       0x03                                //所属_终到
#define Y_ZX        0x01                                //已下发
#define N_ZX        0x02                                //未下发

class jsj_planNode : public QObject
{

    Q_OBJECT
public:
    explicit jsj_planNode(QObject *parent = nullptr);

signals:

public:
    bool isGet = false;
    PalenInfo tempPlan;
    unsigned int PlaneNumber;
    unsigned int YunxinType_ = 0x01;
    unsigned int liecheType_ = 0x01;
    unsigned int Type = 0x01;
    QString PlanName;//当前多站阶段计划名称
    QString PlanStr;
    QString Checi;
    QString ChaoXianInfo;
    unsigned int cxindex = 0x01;
    QString YunxingLeixing;//运行类型
    QString QY_type;
    unsigned int QY_Index = 0x01;//牵引类型索引
    QString Checi_Type;
    int Status = N_ZX;//当前多站计划的执行状态
    int Direct = -1;
    QList<PalenInfo>CurMultiPlanInfo;
};

#endif // JSJ_PLANNODE_H
