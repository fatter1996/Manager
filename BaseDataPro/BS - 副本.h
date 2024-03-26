#pragma once
#include "BaseData.h"
#include "GlobalHeaders/Global.h"
#define TIME_ELAPSE    4

#define _BZD_INTERFACE


#define ZDIAN_BS       0x5A
#define FDIAN_BS       0xA5

#define T1S            0x06
#define T2S            0x06
#define T3S            0x09
#define T4S            0x09
#define T5S            0x0E
#define OffPix1  7
class CArrow;
class CBUTTON;
typedef struct _BZDJK
{
    BYTE FDJ;
    BYTE ZDJ;
    BYTE ZXJ;
    BYTE FXJ;
    BYTE HDJ;
    BYTE TJJ;
    BYTE TCJ;
    BYTE BSJ;
    BYTE FUJ;
    BYTE KTJ;
    BYTE ZKJ;
    BYTE XZJ;
    BYTE GDJ;
    BYTE JSBJ;
    BYTE FSBJ;
    BYTE JBDH;
    BYTE JBDU;
    BYTE JBDL;
    BYTE FBDH;
    BYTE FBDU;
    BYTE FBDL;

    BYTE SGAJ;
    BYTE BSAJ;
    BYTE FUAJ;
    BYTE GJF;

    BYTE inFlag;
    BYTE outFlag;

    UINT FDJtime;
    UINT ZDJtime;
    UINT HDJtime;
    UINT XZJtime;
    UINT ZKJtime;
} TH_BZDJK;

class CBS  : public CBaseData
{
public:
    CBS(void);
    ~CBS(void);
    int m_nSX;
    CArrow *inArrow,
           *outArrow;
    CBUTTON *pBlock,
            *pFault,
            *pRecover;

    QPoint m_JSBJPoint, m_FSBJPoint, m_GJFPoint;

    QPoint pointBtnJZ, pointBtnCZ;//列车进站，列车出站，20190121
    QRect  rectBtnJZ,  rectBtnCZ, rectTxtLZ;//邻站
    bool   m_bBtnJZDown, m_bBtnCZDown;

    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
    bool isHaveJZ;
    bool isJZUseFlag;
    bool isLZFCJLFlag;   //邻站模拟发车进路，用于触发半自闭自动办理
    bool isNeedBSQQ;    //是否需要触发闭塞请求标识
    bool isNeedBSTY;    //是否需要触发闭塞同意标识
    bool isNeedBSTYtime;    //需要闭塞同意计时
    QPoint pointBtnLZFCJL;
    QPoint pointBtnJZuse;
    QPoint pointBtnJZStopuse;
    QPoint pointBtnLZFCJLConst;
    QPoint pointBtnJZuseConst;
    QPoint pointBtnJZStopuseConst;
    QPoint pointBtnJZZero;
    QPoint pointBtnJZZeroConst;
    bool JZFaultFlag;
    bool JZDifferentFlag;
    unsigned int JZCount;
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
    int nBS_BtnTime;
    int nFY_BtnTime;
    int nSG_BtnTime;

    void initBS();
    void ReInitBSPoint();//重新初始化闭塞坐标
    void ReInitBSPoint(int offset_x, int offset_y);//偏移量//重新初始化闭塞坐标
    void DRAW(QPainter *painter, float nDiploid, int nElapsed);
    int moveCursor(QPoint p);

    void SetState(int i);
    void SetState(int i, BYTE nState);

    BYTE m_BSState[4];   //ZDJ,FDJ,ZXJ,FXJ
    BYTE m_Times[4];      //状态保持时间
    BOOL isChange[4];    //状态是否改变

    //通号继电器组合组合
    TH_BZDJK m_nBZDJK[2];  //0--本站接口  1--对方站接口

    BYTE GZType;/*故障类型*/

    void InitRelay(BYTE i);
private:
    void BZDSetHDJ(BYTE i);
    void BZDSetXZJ(BYTE i);
    void BZDSetTJJ(BYTE i);
    void BZDSetFDJ(BYTE i);
    void BZDSetZDJ(BYTE i);
    void BZDSetZKJ(BYTE i);
    void BZDSetGDJ(BYTE i);
    void BZDSetKTJ(BYTE i);
    void BZDSetTCJ(BYTE i);
    void BZDSetBSJ(BYTE i);
    void BZDSetFUJ(BYTE i);
    void BZDSetJBD(BYTE i);
    void BZDSetFBD(BYTE i);
    void BZDSetZXJFXJ(BYTE i);
public:
    void bzdProc();

    void SetBZDinFlag();
    void SetBZDoutFlag();

    void SetArrowState();

    void CountNUM(QPainter *painter, QPoint p, int num); //计数显示
};
