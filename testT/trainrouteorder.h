#ifndef TRAINROUTEORDER_H
#define TRAINROUTEORDER_H

#include <QMainWindow>
#include <QDateTime>

class TrainRouteOrder
{
public:
    TrainRouteOrder();
    virtual ~TrainRouteOrder();

public:
    QByteArray m_btRecvOrDepart;	//0x00 表示到达 0x01 表示出发
    int m_nLHFlg;	//1 列车 0 货车
    QByteArray m_btBeginOrEndFlg;//（本条进路所属的阶段计划类型）始发（终到）标志位--0xAA正常(接发) 0xBB始发 0xCC终到 0xDD通过

    QString m_strTrainNum;//车次

    int m_nTrackCode;
    QString m_strTrack;

    int m_nCodeReachStaEquip;//进站口信号设备号
    int m_nCodeDepartStaEquip;//出站口信号设备号

    bool m_nAutoTouch;	//自触
    bool m_nManTouch;	//人工触发
    bool m_bYXJL;       //是延续进路
    QString m_strDirection;//方向

    QString m_strBegin;
    QDateTime m_timBegin;
    QString m_strPlanned;
    QDateTime m_timPlanned;

    QString m_strXHDBegin;
    QString m_strXHDEnd;
    QString m_strXHDYXEnd;  //延续进路终端信号机

    bool m_bBTJL;      //是变通进路
    QStringList m_ArrXHDBt;//变通信号机
    QStringList strArrayBTJL;//变通进路数组

    QString m_strRouteState;	//"办理"or"等待"or"出清" or "触发完成"
    int m_nOldRouteState;	//0 等待 1 正在触发 2触发完成 3占用  4出清  5取消 此条进路已处于不可用状态  //0 等待 1 办理 2出清 3此条进路已处于不可用状态

    int m_nGDPos;
    int m_ntime;

    QString m_strRouteDescrip;//进路描述（"-"分割）
    QString m_strRouteDescripReal;//进路描述（实际触发","分割）
    bool m_bDeleteFlag;//删除标志
    bool m_bSuccessed;//办理成功失败表示

    bool m_bElectric;//电力
    int  m_nLevelCX; //超限等级
    bool m_bConfirmed;//已确认（对进路进行修改（股道、自触、进路描述），则认为进路已确认，不再被计划所修改，只更新时间）
    int m_nPlanNumber;//计划号
    int m_nIndexRoute;//匹配的进路表索引
    QDateTime m_timClean;//出清时间

public:
    void SetState(int nState);	//0"办理"or 1"等待"or 2"出清"
public:
    //进路信号机数组
    QStringList tempXhdArray;
    //进路按钮数组
    QStringList tempRouteBtnArray;
    //进路命令帧
    unsigned char m_byArrayUDPJLOrderDate[30];
    //线路所
    bool bXianLuSuo;
};

#endif // TRAINROUTEORDER_H
