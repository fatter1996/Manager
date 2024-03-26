#include "trainrouteorder.h"

TrainRouteOrder::TrainRouteOrder()
{
    m_nOldRouteState = 0;
    m_nAutoTouch = false;
    m_strRouteState = "等待";
    m_strRouteDescrip = "***";
    m_nGDPos = 0;
    m_nManTouch = false;
    m_ntime = 0;

    m_nLHFlg = 1;	//默认为列车
    m_bYXJL = false;
    m_bBTJL = false;
    m_bDeleteFlag = false;

    m_bSuccessed = false;

    m_bElectric = true;//电力
    m_nLevelCX = 0; //超限等级
    m_nPlanNumber = 0;
    m_bConfirmed = false;
    m_nIndexRoute = -1;

    //初始化UDP
    memset(m_byArrayUDPJLOrderDate, 0, 30);
    for (int i = 0; i < 4; i++)
    {
        m_byArrayUDPJLOrderDate[i] = 0xEF;
        m_byArrayUDPJLOrderDate[30 - i - 1] = 0xFE;
    }
    m_byArrayUDPJLOrderDate[4] = 0x1E;
    m_byArrayUDPJLOrderDate[9] = 0x88;//信息分类码 （CTC车务终端->联锁仿真机）
    m_byArrayUDPJLOrderDate[10] = 0x01;//功能按钮类型-列车进路

    bXianLuSuo = false;
}
TrainRouteOrder::~TrainRouteOrder()
{

}

void TrainRouteOrder::SetState(int nState)
{
    m_nOldRouteState = nState;
    switch(nState)
    {
    case 0:
        m_strRouteState = "等待";
        break;
    case 1:
        m_strRouteState = "正在触发";//
        break;
    case 2:
        m_strRouteState = "触发完成";
        break;
    case 3:
        m_strRouteState = "占用";
        break;
    case 4:
    {
        m_strRouteState = "出清";
        m_timClean = QDateTime::currentDateTime();
//        m_timClean = QDateTime::currentDateTime().toString("yyyy-MM-dd   hh:mm:ss   ddd");
    }
        break;
    case 5:
        m_strRouteState = "已取消";
        break;
    default:
        break;

    }
}
