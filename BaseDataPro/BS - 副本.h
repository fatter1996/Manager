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

    QPoint pointBtnJZ, pointBtnCZ;//�г���վ���г���վ��20190121
    QRect  rectBtnJZ,  rectBtnCZ, rectTxtLZ;//��վ
    bool   m_bBtnJZDown, m_bBtnCZDown;

    /*****************************2020.2.25-BJT-��ʦ�������վ����***********************************/
    bool isHaveJZ;
    bool isJZUseFlag;
    bool isLZFCJLFlag;   //��վģ�ⷢ����·�����ڴ������Ա��Զ�����
    bool isNeedBSQQ;    //�Ƿ���Ҫ�������������ʶ
    bool isNeedBSTY;    //�Ƿ���Ҫ��������ͬ���ʶ
    bool isNeedBSTYtime;    //��Ҫ����ͬ���ʱ
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
    /***************************end-2020.2.25-BJT-��ʦ�������վ����*********************************/
    int nBS_BtnTime;
    int nFY_BtnTime;
    int nSG_BtnTime;

    void initBS();
    void ReInitBSPoint();//���³�ʼ����������
    void ReInitBSPoint(int offset_x, int offset_y);//ƫ����//���³�ʼ����������
    void DRAW(QPainter *painter, float nDiploid, int nElapsed);
    int moveCursor(QPoint p);

    void SetState(int i);
    void SetState(int i, BYTE nState);

    BYTE m_BSState[4];   //ZDJ,FDJ,ZXJ,FXJ
    BYTE m_Times[4];      //״̬����ʱ��
    BOOL isChange[4];    //״̬�Ƿ�ı�

    //ͨ�ż̵���������
    TH_BZDJK m_nBZDJK[2];  //0--��վ�ӿ�  1--�Է�վ�ӿ�

    BYTE GZType;/*��������*/

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

    void CountNUM(QPainter *painter, QPoint p, int num); //������ʾ
};
