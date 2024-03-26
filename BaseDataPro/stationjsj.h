#ifndef CSTATIONJSJ_H
#define CSTATIONJSJ_H
#include "station.h"
#include "JMShowPro/Th_kb.h"
#include "train.h"
#include "JMShowPro/trainadddlg.h"
#include <QObject>
class CData;
typedef struct
{
    int DevCode;
    int ANType;
    int index;
} ButtonStr;
class CStationJSJ : public CStation
{
    Q_OBJECT
public:
    CStationJSJ(CData *p);

    void StatusChange(unsigned char *RecvArray);
    CData *pData;
    QVector<CTrain> v_train;
private:
    //信号
    QAction *XHJZDSAction;
    QAction *XHYXDSAction;
    QAction *XH_L;
    QAction *XH_U;
    QAction *XH_B;
    QAction *XH_YB;
    QAction *XHFaultRecover;
    QAction *XH_SJGZ;//双接故障
    QAction *XH_BZDGZ;//半自动故障
    QAction *XH_JZGZ;//计轴故障
    //道岔
    QAction *DCDWGZAction;
    QAction *DCFWGZAction;
    QAction *DCJCGZAction;
    QAction *DCDLGZAction;
    QAction *DCFaultRecover;

    QAction *DWFLBLZAction;
    QAction *FWFLBLZAction;
    QAction *CQFLBLZAction;

    QAction *DCHANDDW;
    QAction *DCHANDFW;
    //区段
    QAction *QDGZAction;
    QAction *QDFLBLAction;
    QAction *QDFaultRecover;
    QAction *QDFLBFlagLAction;
    //命令取消菜单
    QAction *CancelAction;
    QAction *AddTrianAction;
    //按钮数组
    QVector<ButtonStr> ButtonArray;

public:
    void InitXHDMenue();
    void InitDCDMenue();
    void InitQDDMenue();
    void InitOtherMenue();
    void MenueDev(int ID, int index);
    void MenueXHD(int ID, CXHD *pXHD);
    void MenueDC(int ID, CGDDC *pDC);
    void MenueGD(int ID, CGD *pGD);
    void MenueOther(int ID);
    bool DevLButtonDown(QPoint pt, CTh_kb *pJm);
    void OnRButtonDown(QMouseEvent *event);
    void xhdLButtonDown(int devCount, int nMoveCursorm, CTh_kb *pJm);
    void gddcLButtonDown(int devCount, int nMoveCursorm, CTh_kb *pJm);
    void gdLButtonDown(int devCount, int nMoveCursorm, CTh_kb *pJm);
    void tgLButtonDown(int devCount, int nMoveCursor);
    void LZZDBSLButtonDown(int devCount, int nMoveCursor);
    void LZBSLButtonDown(int devCount, int nMoveCursor);
    int PackGZMessage(int Funcode, int Devcode, const bool GZSet = true);
    int PackOperationMessage(int antype, int Funcode, int Devcode);

    void setBSState(unsigned char *rec, int len);
    void InitZDBSState();
    void InitBSState();

    int PackButtonMessage();
    int PackBSMessage();
    int PackZDBSFlag();
    int PackBSFlag();

    void ButtonClicked(QPushButton *pBtn);
    void ClearButton();

    void updateTrainPosition(unsigned int devCode, CTrain train);
    void AnalyseTrainMsg(unsigned char *rec);

    QString getQDName(unsigned int QDCode, CTrain *pTrain);

    void TimeProc();
signals:
    void ClearMenueButton();
private slots:
    void DealMenuAction(QAction* action);
    void slot_FlblDlgOK(int zcCount, int devCount, bool flag1, bool flag2, bool flag3); //接收分路不良确定按钮

};

#endif // CSTATIONJSJ_H
