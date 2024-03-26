#include "stationjsj.h"
#include "Const.h"
#include "JMShowPro/FlblDlg.h"
#include <qmenu.h>
#include "data.h"
CStationJSJ::CStationJSJ(CData *p)
{
    pData = p;
    LZZDBSFlag = false;
    LZBZDFlag = false;
    InitXHDMenue();
    InitDCDMenue();
    InitQDDMenue();
    InitOtherMenue();
}
void CStationJSJ::StatusChange(unsigned char *array)
{
    CGD *gd;
    CQD *qd;
    CGDDC *dc, *dc1;
    CXHD *xhd;
    int lenth;
    lenth = 1024;//500
    unsigned int nCount = 6;  //2020.3.23-BJT-升级为不定长通信
    unsigned int nCode = 0;

    int num = 0;
    setLSComState(true);  //联锁通信状态标识
    setLSComTime(8);   //与联锁持续无通信周期计数器
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC *)ement;
            int state = 0;
            if ((array[nCount] & 0x03) == 0x02)
            {
                state |= 0x80;
                gddc->setDCDBSFlag(false);
            }
            else if ((array[nCount] & 0x03) == 0x01)
            {
                state |= 0x40;
                gddc->setDCDBSFlag(false);
            }
            else if ((array[nCount] & 0x03) == 0x03)
            {
                state |= 0x80;
                gddc->setDCDBSFlag(true);
            }
            else if ((array[nCount] & 0x04) == 0x04)
            {
                state |= 0x40;
                gddc->setDCDBSFlag(true);
            }
            if ((array[nCount] & 0x10) == 0x10)
                state |= 0x20;
            if ((array[nCount] & 0x20) == 0x20)
                state |= 0x10;
            nCount++;
            //qDebug()<<"收到联锁车站设备状态信息，道岔设备："<<ement->getName()<<"     状态字节信息"<<state;
            gddc->setGDDC_StateOfLS(state);
            num++;
        }
    }
    num = 0;
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_XH)
        {
            int state1;
            int state = 0;
            int state2 = 0;
            int state3 = 0;
            int state4 = 0;
            CXHD *xhd = (CXHD *)ement;
            state = (array[nCount] & 0x0f);
            state1 = state << 4;
            if ((array[nCount] & 0x10) == 0x10)
                state2 |= 0x08;
            if (xhd->getZDBSJKFlag() && (array[nCount] & 0x20) == 0x20)
                xhd->setGZType(attr_XH_SJGZ);
            else
                xhd->clearGZType(attr_XH_SJGZ);
            state2 |= 0x07;
            //qDebug()<<"收到联锁车站设备状态信息，信号设备："<<ement->getName()<<"     状态字节信息"<<state1<<state2<<state3;
            xhd->setXHD_StateOfLS(state1, state2, state3, state4);
            num++;
            nCount++;
        }
    }
    num = 0;
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_QD)
        {
            CQD *qd = (CQD *)ement;
            int state1, state2, state3;
            state1 = state2 = state3 = 0;
            if (array[nCount] == 0x11)
                state1 |= 0;
            else if (array[nCount] == 0x33)
                state1 |= 0x20;
            else
                state1 |= 0x40;
            //qDebug()<<"收到联锁车站设备状态信息，区段设备："<<ement->getName()<<"     状态字节信息"<<state1<<state2;
            setQD_StateOfLS(qd, state1, state2, state3);
            num++;
            nCount++;
        }
    }
}
void CStationJSJ::InitXHDMenue()
{
    XHJZDSAction = new QAction("禁止信号断丝", this);
    XHJZDSAction->setData(0);
    XHJZDSAction->setCheckable(true);
    XH_L = new QAction("绿灯", this);
    XH_U = new QAction("黄灯", this);
    XH_B = new QAction("白灯", this);
    XH_YB = new QAction("引白", this);
    XH_L->setData(1);
    XH_U->setData(2);
    XH_B->setData(3);
    XH_YB->setData(4);
    XHFaultRecover = new QAction("全站信号故障恢复", this);
    XHFaultRecover->setData(5);

    XH_L->setCheckable(true);
    XH_U->setCheckable(true);
    XH_B->setCheckable(true);
    XH_YB->setCheckable(true);

    XH_SJGZ = new QAction("双接故障", this);
    XH_SJGZ->setData(6);
    XH_SJGZ->setCheckable(true);

    XH_BZDGZ = new QAction("半自动故障", this);
    XH_BZDGZ->setData(7);
    XH_BZDGZ->setCheckable(true);

    XH_JZGZ = new QAction("计轴故障", this);
    XH_JZGZ->setData(8);
    XH_JZGZ->setCheckable(true);
}
void CStationJSJ::InitDCDMenue()
{
    DCDWGZAction = new QAction("定位故障", this);
    DCFWGZAction = new QAction("反位故障", this);
    DCJCGZAction = new QAction("挤岔故障", this);
    DCDLGZAction = new QAction("启动电路故障", this);
    DCFaultRecover = new QAction("道岔故障恢复", this);

    DCHANDDW = new QAction("道岔室外扳动定位", this);
    DCHANDFW = new QAction("道岔室外扳动反位", this);

    DWFLBLZAction = new QAction("定位分路不良标记", this);
    FWFLBLZAction = new QAction("反位分路不良标记", this);
    CQFLBLZAction = new QAction("岔前分路不良标记", this);

    DCDWGZAction->setData(0);
    DCFWGZAction->setData(1);
    DCJCGZAction->setData(2);
    DCDLGZAction->setData(3);
    DCFaultRecover->setData(4);
    DCHANDDW->setData(9);
    DCHANDFW->setData(10);
    DWFLBLZAction->setData(11);
    FWFLBLZAction->setData(12);
    CQFLBLZAction->setData(13);

    DCDWGZAction->setCheckable(true);
    DCFWGZAction->setCheckable(true);
    DCJCGZAction->setCheckable(true);
    DCDLGZAction->setCheckable(true);

    DWFLBLZAction->setCheckable(true);
    FWFLBLZAction->setCheckable(true);
    CQFLBLZAction->setCheckable(true);
}

void CStationJSJ::InitQDDMenue()
{
    QDGZAction = new QAction("区段故障", this);
    QDFLBLAction = new QAction("分路不良", this);
    QDFaultRecover = new QAction("区段故障恢复", this);
    QDFLBFlagLAction = new QAction("分路不良标记", this);
    QDGZAction->setData(6);
    QDFLBLAction->setData(5);
    QDFaultRecover->setData(7);
    QDFLBFlagLAction->setData(9);
    QDGZAction->setCheckable(true);
    QDFLBLAction->setCheckable(true);
    QDFLBFlagLAction->setCheckable(true);
}

void CStationJSJ::InitOtherMenue()
{
    CancelAction = new QAction("取消", this);
    CancelAction->setData(0);
    AddTrianAction = new QAction("增加演练车次", this);
    AddTrianAction->setData(20);
}
void CStationJSJ::MenueDev(int ID, int index)
{
    CXHD *pXHD = nullptr;
    CGDDC *pDC = nullptr;
    CGD *pGD = nullptr;
    if (index < DevArray.size())
    {
        if (DevArray[index]->getDevType() == Dev_XH)
        {
            pXHD = (CXHD *)DevArray[index];
            MenueXHD(ID, pXHD);
        }
        else if (DevArray[index]->getDevType() == Dev_DC)
        {
            pDC = (CGDDC *)DevArray[index];
            MenueDC(ID, pDC);
        }
        else if (DevArray[index]->getDevType() == Dev_GD)
        {
            pGD = (CGD *)DevArray[index];
            MenueGD(ID, pGD);
        }
    }
}

void CStationJSJ::MenueXHD(int ID, CXHD *pXHD)
{
    CBS *pBS = nullptr;
    for (QVector<CBS*>::iterator it = v_LZBSArray.begin(); it < v_LZBSArray.end(); it++)
    {
        if ((*it)->getName() == pXHD->getName())
        {
            pBS = *it;
            break;
        }
    }
    if (pXHD != nullptr)
    {
        switch (ID)
        {
        case 0:
            if (pXHD->getGZType(attr_XH_JZDS))
            {
                PackGZMessage(DS1GZM, pXHD->getCode(), false);
                pXHD->clearGZType(attr_XH_JZDS);
            }
            else
            {
                PackGZMessage(DS1GZM, pXHD->getCode());
                pXHD->setGZType(attr_XH_JZDS);
            }
            break;
        case 1:
            if (pXHD->getGZType(attr_XH_DSL))
            {
                PackGZMessage(DSGZM_L, pXHD->getCode(), false);
                pXHD->clearGZType(attr_XH_DSL);
            }
            else
            {
                PackGZMessage(DSGZM_L, pXHD->getCode());
                pXHD->setGZType(attr_XH_DSL);
            }
            break;
        case 2:
            if (pXHD->getGZType(attr_XH_DSU))
            {
                PackGZMessage(DSGZM_U, pXHD->getCode(), false);
                pXHD->clearGZType(attr_XH_DSU);
            }
            else
            {
                PackGZMessage(DSGZM_U, pXHD->getCode());
                pXHD->setGZType(attr_XH_DSU);
            }
            break;
        case 3:
            if (pXHD->getGZType(attr_XH_DSB))
            {
                PackGZMessage(DSGZM_B, pXHD->getCode(), false);
                pXHD->clearGZType(attr_XH_DSB);
            }
            else
            {
                PackGZMessage(DSGZM_B, pXHD->getCode());
                pXHD->setGZType(attr_XH_DSB);
            }
            break;
        case 4:
            if (pXHD->getGZType(attr_XH_DSYB))
            {
                PackGZMessage(DSGZM_YB, pXHD->getCode(), false);
                pXHD->clearGZType(attr_XH_DSYB);
            }
            else
            {
                PackGZMessage(DSGZM_YB, pXHD->getCode());
                pXHD->setGZType(attr_XH_DSYB);
            }
            break;
        case 5:
            PackGZMessage(XHGZQX, pXHD->getCode(), false);
            pXHD->clearAllGZ();
            break;
        case 6:
            if (pXHD->getGZType(attr_XH_SJGZ))
            {
                PackGZMessage(ZDBSSJHF, pXHD->getCode());
                pXHD->clearGZType(attr_XH_SJGZ);
            }
            else
            {
                PackGZMessage(ZDBSSJGZ, pXHD->getCode());
                pXHD->setGZType(attr_XH_SJGZ);
            }
            break;
        case 7:
            if (pBS)
            {
                if (pXHD->getGZType(attr_XH_BZDGZ))
                {
                    PackGZMessage(BZDBSHF, pBS->getCode());
                    pXHD->clearGZType(attr_XH_BZDGZ);
                }
                else
                {
                    PackGZMessage(BZDBSGZ, pBS->getCode());
                    pXHD->setGZType(attr_XH_BZDGZ);
                }
            }
            break;
        case 8:
            if (pBS)
            {
                if (pXHD->getGZType(attr_XH_JZGZ))
                {
                    PackGZMessage(JZGZ, pBS->getCode());
                    pXHD->clearGZType(attr_XH_JZGZ);
                }
                else
                {
                    PackGZMessage(JZGZHF, pBS->getCode());
                    pXHD->setGZType(attr_XH_JZGZ);
                }
            }
            break;
        case 9:
            if (pBS)
            {
                if (pXHD->getGZType(attr_XH_JZBYZ))
                {
                    PackGZMessage(JZDifferent, pBS->getCode());
                    pXHD->clearGZType(attr_XH_JZBYZ);
                }
                else
                {
                    PackGZMessage(JZDifferentHF, pBS->getCode());
                    pXHD->setGZType(attr_XH_JZBYZ);
                }
            }
            break;
        default:
            break;
        }
    }
}

void CStationJSJ::MenueDC(int ID, CGDDC *pDC)
{
    if (pDC != nullptr)
    {
        switch (ID)
        {
        case 0:
            if (pDC->getGZType(attr_DC_DWGZ))
            {
                PackGZMessage(DCDWGZ, pDC->getCode(), false);
                pDC->clearGZType(attr_DC_DWGZ);
            }
            else
            {
                PackGZMessage(DCDWGZ, pDC->getCode());
                pDC->setGZType(attr_DC_DWGZ);
            }
            break;
        case 1:
            if (pDC->getGZType(attr_DC_FWGZ))
            {
                PackGZMessage(DCFWGZ, pDC->getCode(), false);
                pDC->clearGZType(attr_DC_FWGZ);
            }
            else
            {
                PackGZMessage(DCFWGZ, pDC->getCode());
                pDC->setGZType(attr_DC_FWGZ);
            }
            break;
        case 2:
            if (pDC->getGZType(attr_DC_JCGZ))
            {
                PackGZMessage(DCJCGZ, pDC->getCode(), false);
                pDC->clearGZType(attr_DC_JCGZ);
            }
            else
            {
                PackGZMessage(DCJCGZ, pDC->getCode());
                pDC->setGZType(attr_DC_JCGZ);
            }
            break;
        case 3:
            if (pDC->getGZType(attr_DC_DLGZ))
            {
                PackGZMessage(DCDLGZ, pDC->getCode(), false);
                pDC->clearGZType(attr_DC_DLGZ);
            }
            else
            {
                PackGZMessage(DCDLGZ, pDC->getCode());
                pDC->setGZType(attr_DC_DLGZ);
            }
            break;
        case 4:
            PackGZMessage(DCGZQX, pDC->getCode(), false);
            pDC->clearGZType(attr_DC_DWGZ);
            pDC->clearGZType(attr_DC_FWGZ);
            pDC->clearGZType(attr_DC_DLGZ);
            pDC->clearGZType(attr_DC_JCGZ);
            break;
        case 5:
            if (pDC->getGZType(attr_QD_FLBLGZ))
            {
                PackGZMessage(QDFLBL, pDC->getCode(), false);
                pDC->clearGZType(attr_QD_FLBLGZ);
            }
            else
            {
                PackGZMessage(QDFLBL, pDC->getCode());
                pDC->setGZType(attr_QD_FLBLGZ);
            }
            break;
        case 6:
            if (pDC->getGZType(attr_QD_QDGZ))
            {
                PackGZMessage(QDGZM, pDC->getCode(), false);
                pDC->clearGZType(attr_QD_QDGZ);
            }
            else
            {
                PackGZMessage(QDGZM, pDC->getCode());
                pDC->setGZType(attr_QD_QDGZ);
            }
            break;
        case 7:
            PackGZMessage(QDGZQX, pDC->getCode(), false);
            pDC->clearGZType(attr_QD_QDGZ);
            pDC->clearGZType(attr_QD_FLBLGZ);
            break;
        case 9:
            PackGZMessage(DCQZDW, pDC->getCode());
            break;
        case 10:
            PackGZMessage(DCQZFW, pDC->getCode());
            break;
        case 11:
            if (pDC->getIsFLBL_DW())
            {
                PackGZMessage(DWFLBL, pDC->getCode(), false);
                pDC->setIsFLBL_DW(false);
            }
            else
            {
                PackGZMessage(DWFLBL, pDC->getCode());
                pDC->setIsFLBL_DW(true);
            }
            break;
        case 12:
            if (pDC->getIsFLBL_FW())
            {
                PackGZMessage(FWFLBL, pDC->getCode(), false);
                pDC->setIsFLBL_FW(false);
            }
            else
            {
                PackGZMessage(FWFLBL, pDC->getCode());
                pDC->setIsFLBL_FW(true);
            }
            break;
        case 13:
            if (pDC->getIsFLBL_CQ())
            {
                PackGZMessage(CQFLBL, pDC->getCode(), false);
                pDC->setIsFLBL_CQ(false);
            }
            else
            {
                PackGZMessage(CQFLBL, pDC->getCode());
                pDC->setIsFLBL_CQ(true);
            }
            break;
        case 20:
        {
            CTrainAddDlg addTrain(nullptr, pData, pDC->getQDCode(), this);
            addTrain.exec();
            break;
        }
        default:
            break;
        }
    }
}

void CStationJSJ::MenueGD(int ID, CGD *pGD)
{
    if (pGD != nullptr)
    {
        switch (ID)
        {
        case 5:
            if (pGD->getGZType(attr_QD_FLBLGZ))
            {
                PackGZMessage(QDFLBL, pGD->getCode(), false);
                pGD->clearGZType(attr_QD_FLBLGZ);
            }
            else
            {
                PackGZMessage(QDFLBL, pGD->getCode());
                pGD->setGZType(attr_QD_FLBLGZ);
            }
            break;
        case 6:
            if (pGD->getGZType(attr_QD_QDGZ))
            {
                PackGZMessage(QDGZM, pGD->getCode(), false);
                pGD->clearGZType(attr_QD_QDGZ);
            }
            else
            {
                PackGZMessage(QDGZM, pGD->getCode());
                pGD->setGZType(attr_QD_QDGZ);
            }
            break;
        case 7:
            PackGZMessage(QDGZQX, pGD->getCode(), false);
            pGD->clearGZType(attr_QD_QDGZ);
            pGD->clearGZType(attr_QD_FLBLGZ);
            break;
        case 9:
            if (pGD->getGDFLBL())
            {
                PackGZMessage(QDFLBLFLAG, pGD->getCode(), false);
                pGD->setGDFLBL(false);
            }
            else
            {
                PackGZMessage(QDFLBLFLAG, pGD->getCode());
                pGD->setGDFLBL(true);
            }
            break;
        case 20:
        {
            CTrainAddDlg addTrain(nullptr, pData, pGD->getCode(), this);
            addTrain.exec();
            break;
        }
        default:
            break;
        }
    }
}

void CStationJSJ::MenueOther(int ID)
{
    CXHD *pXHD = nullptr;
    CTG *pTG = nullptr;
    switch (ID)
    {
    case 0:
    {
        ClearButton();
        emit ClearMenueButton();
        break;
    }
    default:
        break;
    }
}
bool CStationJSJ::DevLButtonDown(QPoint p, CTh_kb *pJm)
{
    int nMoveCursor = 0;
    for (int i = 0; i < DevArray.size(); i++)
    {
        nMoveCursor = DevArray[i]->moveCursor(p);
        if (nMoveCursor > 0)
        {
            if (DevArray[i]->getDevType() == Dev_XH)
            {
                xhdLButtonDown(i, nMoveCursor, pJm);
                return true;
            }
            if (DevArray[i]->getDevType() == Dev_DC)
            {
                gddcLButtonDown(i, nMoveCursor, pJm);
                return true;
            }
            if (DevArray[i]->getDevType() == Dev_GD)
            {
                gdLButtonDown(i, nMoveCursor, pJm);
                return true;
            }
            else if (DevArray[i]->getDevType() == Dev_TG)
            {
                tgLButtonDown(i, nMoveCursor);
                return true;
            }
            else if (DevArray[i]->getDevType() == Dev_LZZDBS)
            {
                LZZDBSLButtonDown(i, nMoveCursor);
                return true;
            }
            else if (DevArray[i]->getDevType() == Dev_LZBZD)
            {
                LZBSLButtonDown(i, nMoveCursor);
                return true;
            }
        }
    }
    return false;
}

void CStationJSJ::OnRButtonDown(QMouseEvent * event)
{
    QMenu menu;
    QMenu submenu;
    int size = DevArray.size();
    int nMoveCursor = -1;
    QPoint globalPt = event->globalPos();
    QPoint pt = event->pos();
    CXHD *pXHD = nullptr;
    CGDDC *pDC = nullptr;
    CGD *pGD = nullptr;
    int ID = -1;
    QAction *action = nullptr;
    for (int j = 0; j < size; j++)
    {
        nMoveCursor = DevArray[j]->moveCursor(pt);
        if (nMoveCursor > 0)
        {
            if (DevArray[j]->getDevType() == Dev_XH)
            {
                pXHD = (CXHD *)DevArray[j];
                menu.addAction(XHJZDSAction);

                submenu.setTitle("允许信号断丝");
                submenu.addAction(XH_L);
                submenu.addAction(XH_U);
                submenu.addAction(XH_B);
                submenu.addAction(XH_YB);
                menu.addMenu(&submenu);
                menu.addAction(XHFaultRecover);

                XHJZDSAction->setChecked(pXHD->getGZType(attr_XH_JZDS));
                XH_L->setChecked(pXHD->getGZType(attr_XH_DSL));
                XH_U->setChecked(pXHD->getGZType(attr_XH_DSU));
                XH_B->setChecked(pXHD->getGZType(attr_XH_DSB));
                XH_YB->setChecked(pXHD->getGZType(attr_XH_DSYB));
                if (pXHD->getZDBSJKFlag())
                {
                    menu.addSeparator();
                    menu.addAction(XH_SJGZ);
                    XH_SJGZ->setChecked(pXHD->getGZType(attr_XH_SJGZ));
                }
                else
                {
                    for (QVector<CBS*>::iterator it = v_LZBSArray.begin(); it < v_LZBSArray.end(); it++)
                    {
                        if ((*it)->getName() == pXHD->getName())
                        {
                            menu.addSeparator();
                            menu.addAction(XH_BZDGZ);
                            XH_BZDGZ->setChecked(pXHD->getGZType(attr_XH_BZDGZ));
                            if ((*it)->isHaveJZ)
                            {
                                menu.addAction(XH_JZGZ);
                                XH_JZGZ->setChecked(pXHD->getGZType(attr_XH_JZGZ));
                            }
                            break;
                        }
                    }
                }
            }
            else if (DevArray[j]->getDevType() == Dev_DC)
            {
                pDC = (CGDDC *)DevArray[j];
                menu.addAction(DCDWGZAction);
                menu.addAction(DCFWGZAction);
                menu.addAction(DCJCGZAction);
                menu.addAction(DCDLGZAction);
                menu.addAction(DCFaultRecover);
                menu.addSeparator();
                menu.addAction(QDFLBLAction);
                menu.addAction(QDGZAction);
                menu.addAction(QDFaultRecover);
                menu.addSeparator();
                menu.addAction(DCHANDDW);
                menu.addAction(DCHANDFW);
                menu.addSeparator();
                menu.addAction(DWFLBLZAction);
                menu.addAction(FWFLBLZAction);
                menu.addAction(CQFLBLZAction);
                DCDWGZAction->setChecked(pDC->getGZType(attr_DC_DWGZ));
                DCFWGZAction->setChecked(pDC->getGZType(attr_DC_FWGZ));
                DCJCGZAction->setChecked(pDC->getGZType(attr_DC_JCGZ));
                DCDLGZAction->setChecked(pDC->getGZType(attr_DC_DLGZ));
                QDFLBLAction->setChecked(pDC->getGZType(attr_QD_FLBLGZ));
                QDGZAction->setChecked(pDC->getGZType(attr_QD_QDGZ));
                DWFLBLZAction->setChecked(pDC->getIsFLBL_DW());
                FWFLBLZAction->setChecked(pDC->getIsFLBL_FW());
                CQFLBLZAction->setChecked(pDC->getIsFLBL_CQ());
                menu.addSeparator();
                menu.addAction(AddTrianAction);
            }
            else if (DevArray[j]->getDevType() == Dev_GD)
            {
                pGD = (CGD *)DevArray[j];
                menu.addAction(QDFLBLAction);
                menu.addAction(QDGZAction);
                menu.addAction(QDFaultRecover);
                menu.addSeparator();
                menu.addAction(QDFLBFlagLAction);

                QDGZAction->setChecked(pGD->getGZType(attr_QD_QDGZ));
                QDFLBLAction->setChecked(pGD->getGZType(attr_QD_FLBLGZ));
                QDFLBFlagLAction->setChecked(pGD->getGDFLBL());
                menu.addSeparator();
                menu.addAction(AddTrianAction);
            }
            action = menu.exec(globalPt);
            if (action != nullptr)
            {
                ID = action->data().toInt();
                MenueDev(ID, j);
            }
            break;
        }
    }
    if (nMoveCursor <= 0)
    {
        menu.addAction(CancelAction);
        if ((ButtonArray.size() == 0) && (pData->pJM->nCurrentCode == 0))
            CancelAction->setEnabled(false);
        else
            CancelAction->setEnabled(true);
        action = menu.exec(globalPt);
        if (action != nullptr)
        {
            ID = action->data().toInt();
            MenueOther(ID);
        }
    }
}

void CStationJSJ::xhdLButtonDown(int devCount, int nMoveCursor, CTh_kb *pJm)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    int staID = getStationID();
    int staLSProcessID = getLSProcessID();
    int staLSSoftID = getLSSoftID();
    int devCode = DevArray[devCount]->getCode();
    CXHD *xhd = (CXHD*)DevArray[devCount];
    if (pJm->nCurrentCode == Fun_ANJF) //按钮解封
    {
        if (nMoveCursor == 1)
        {
            PackOperationMessage(DCAN, Fun_ANJF, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "调车按钮解封";
        }
        else if (nMoveCursor == 2)
        {
            PackOperationMessage(LCAN, Fun_ANJF, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "列车按钮解封";
        }
        else if (nMoveCursor == 3)
        {
            PackOperationMessage(YDAN, Fun_ANJF, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "引导按钮解封";
        }
    }
    else if (pJm->nCurrentCode == Fun_ANFS) //按钮封锁
    {
        if ((nMoveCursor == 1) && (xhd->getIsDCANFB() == false))
        {
            PackOperationMessage(DCAN, Fun_ANFS, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "调车按钮封锁";
        }
        else if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(LCAN, Fun_ANFS, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "列车按钮封锁";
        }
        else if ((nMoveCursor == 3) && (xhd->getIsYDANFB() == false))
        {
            PackOperationMessage(YDAN, Fun_ANFS, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "引导按钮封锁";
        }
    }
    else if (pJm->nCurrentCode == Fun_PDJS) //坡道解锁
    {
        if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(Fun_PDJS, LCAN, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "坡道解锁";
        }
    }
    else if (pJm->nCurrentCode == Fun_ZQX) //总取消
    {
        if ((nMoveCursor == 1) && (xhd->getIsDCANFB() == false))
        {
            PackOperationMessage(Fun_ZQX, Fun_ZQX, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "调车总取消";
        }
        else if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(Fun_ZQX, Fun_ZQX, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "列车总取消";
        }
    }
    else if (pJm->nCurrentCode == Fun_ZRJ) //总人解
    {
        if ((nMoveCursor == 1) && (xhd->getIsDCANFB() == false))
        {
            PackOperationMessage(Fun_ZRJ, Fun_ZRJ, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "调车总人解";
        }
        else if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(Fun_ZRJ, Fun_ZRJ, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "列车总人解";
        }
        else if ((nMoveCursor == 3) && (xhd->getIsYDANFB() == false))
        {
            PackOperationMessage(Fun_ZRJ, Fun_ZRJ, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "引导总人解";
        }
    }
    else if (pJm->nCurrentCode == Fun_DD) //点灯
    {
        if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(LCAN, Fun_DD, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "点灯";
        }
    }
    else if (pJm->nCurrentCode == Fun_MD) //灭灯
    {
        if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            PackOperationMessage(LCAN, Fun_MD, devCode);
            pJm->nCurrentCode = 0;
            qDebug() << DevArray[devCount]->getName() << "灭灯";
        }
    }
    else
    {
        ButtonStr button;
        if ((nMoveCursor == 1) && (xhd->getIsDCANFB() == false))
        {
            button.ANType = DCAN;
            button.DevCode = devCode;
            button.index = devCount;
            ButtonArray.push_back(button);
            xhd->setCmdButtonFlag(0x22);
        }
        else if ((nMoveCursor == 2) && (xhd->getIsLCANFB() == false))
        {
            button.ANType = LCAN;
            button.DevCode = devCode;
            button.index = devCount;
            ButtonArray.push_back(button);
            xhd->setCmdButtonFlag(0x11);
        }
        else if ((nMoveCursor == 3) && (xhd->getIsYDANFB() == false))
        {
            if (pJm->isQFDlgOK(Fun_XHANDOWN, pJm->strQFPassWork, pJm->nQFDlgTitleType))
            {
                PackOperationMessage(Fun_XHANDOWN, YDAN, devCode);
                qDebug() << DevArray[devCount]->getName() << "引导按钮按下";
            }
        }
    }
}

void CStationJSJ:: gddcLButtonDown(int devCount, int nMoveCursor, CTh_kb *pJm)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    CGDDC *gddc;
    gddc = (CGDDC *)DevArray[devCount];
    if (nMoveCursor == 11)
    {
        gddc->onLBtnDownJG();
    }
    else if (nMoveCursor == 12)
    {
        gddc->onLBtnDownXG();
    }
    else
    {
        int staID = gddc->getStationID();
        int staLSProcessID = getLSProcessID();
        int staLSSoftID = getLSSoftID();
        int gddcCode = gddc->getCode();
        int qdCode = gddc->getQDCode();
        if (pJm->nCurrentCode == Fun_QGJ) //区故解
        {
            PackOperationMessage(Fun_QGJ, Fun_QGJ, qdCode);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "区故解";
        }
        else if (pJm->nCurrentCode == Fun_DCZD)  //总定位
        {
            PackOperationMessage(Fun_DCZD, Fun_DCZD, gddcCode);
            pJm->SetDLBStart(staID);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "总定位";
        }
        else if (pJm->nCurrentCode == Fun_DCZF)  //总反位
        {
            PackOperationMessage(Fun_DCZF, Fun_DCZF, gddcCode);
            pJm->SetDLBStart(staID);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "总反位";
        }
        else if (pJm->nCurrentCode == Fun_DCDS)  //单锁
        {
            PackOperationMessage(Fun_DCDS, Fun_DCDS, gddcCode);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "单锁";
        }
        else if (pJm->nCurrentCode == Fun_DCDJ)  //单解
        {
            PackOperationMessage(Fun_DCDJ, Fun_DCDJ, gddcCode);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "单解";
        }
        else if (pJm->nCurrentCode == Fun_DCFS)  //道岔封锁
        {
            PackOperationMessage(Fun_DCFS, Fun_DCFS, gddcCode);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "道岔封锁";
        }
        else if (pJm->nCurrentCode == Fun_DCJF)  //道岔解封
        {
            PackOperationMessage(Fun_DCJF, Fun_DCJF, gddcCode);
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "道岔解封";
        }
        else if (pJm->nCurrentCode == Fun_FLBL)  //分路不良
        {
            bool flag1 = gddc->getIsFLBL_CQ();
            bool flag2 = gddc->getIsFLBL_DW();
            bool flag3 = gddc->getIsFLBL_FW();
            CFLBLDlg *flbl_dlg = new CFLBLDlg();
            flbl_dlg->init(0, devCount, 0x11, flag1, flag2, flag3);
            connect(flbl_dlg, SIGNAL(signal_FlblDlgOK(int, int, bool, bool, bool)), this, SLOT(slot_FlblDlgOK(int, int, bool, bool, bool)));
            flbl_dlg->exec();
            pJm->nCurrentCode = 0;
            qDebug() << gddc->getName() << "分路不良";
        }
    }
}

void CStationJSJ::gdLButtonDown(int devCount, int nMoveCursorm, CTh_kb *pJm)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    CGD *gd;
    gd = (CGD *)DevArray[devCount];
    if (pJm->nCurrentCode == Fun_QGJ) //区故解
    {
        PackOperationMessage(Fun_QGJ, Fun_QGJ, gd->getCode());
        pJm->nCurrentCode = 0;
        qDebug() << gd->getName() << "区故解";
    }
    else if (pJm->nCurrentCode == Fun_FLBL)  //分路不良
    {
        bool flag1 = gd->getGDFLBL();
        CFLBLDlg *flbl_dlg = new CFLBLDlg();
        flbl_dlg->init(0, devCount, 0x22, flag1, false, false);
        connect(flbl_dlg, SIGNAL(signal_FlblDlgOK(int, int, bool, bool, bool)), this, SLOT(slot_FlblDlgOK(int, int, bool, bool, bool)));
        flbl_dlg->exec();
        pJm->nCurrentCode = 0;
        qDebug() << gd->getName() << "分路不良";
    }
}

void CStationJSJ::tgLButtonDown(int devCount, int nMoveCursor)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    int devCode = DevArray[devCount]->getCode();
    CTG *tg = (CTG *)DevArray[devCount];
    ButtonStr button;
    if (nMoveCursor == 1)
    {
        button.ANType = TGAN;
        button.DevCode = devCode;
        button.index = devCount;
        ButtonArray.push_back(button);
        tg->setCmdflag(true);
    }
}

void CStationJSJ::LZZDBSLButtonDown(int devCount, int nMoveCursor)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    int index = nMoveCursor - 1;
    CLZZDBS *zdbs = (CLZZDBS *)DevArray[devCount];
    zdbs->DealwithClicked(index);
}

void CStationJSJ::LZBSLButtonDown(int devCount, int nMoveCursor)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    CBS *bs = (CBS *)DevArray[devCount];
    bs->DealwithClicked(nMoveCursor);
}

int CStationJSJ::PackGZMessage(int Funcode, int Devcode, bool GZSet)
{
    QByteArray array(100, 0);
    int index = 4;
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    GZSet ? array[index++] = 0x5a : array[index++] = 0xa5; //故障设置及恢复
    array[index++] = 1;
    memcpy(array.data() + index, &Funcode, 2);
    index += 2;
    memcpy(array.data() + index, &Devcode, 2);
    index += 2;
    index += 4;
    index += 2;//预留
    index += 4; //帧尾

    index = 60;//联锁解析固定字节60
    for (int i = 0; i < 4; i++)
    {
        array[index - i - 1] = FRAMTAIL;
    }
    if (pData != nullptr)
    {
        pData->SendUdpMessage(array, index, this);
    }
    return index;
}

int CStationJSJ::PackOperationMessage(int antype, int Funcode, int Devcode)
{
    QByteArray array(100, 0);
    int index = 9;
    unsigned char subcode;
    if (Funcode == Fun_CLEAR)
    {
        ClearButton();
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    array[index++] = 0x88;
    switch (Funcode)
    {
    case YDAN:
        subcode = 0x12;
        break;
    case Fun_ZQX:
        subcode = 0x04;
        break;
    case Fun_ZRJ:
        subcode = 0x14;
        break;
    case Fun_DCZD:
        subcode = 0x06;
        break;
    case Fun_DCZF:
        subcode = 0x07;
        break;
    case Fun_DCDS:
        subcode = 0x08;
        break;
    case Fun_DCDJ:
        subcode = 0x09;
        break;
    case Fun_DCFS:
    case Fun_ANFS:
        subcode = 0x10;
        break;
    case Fun_DCJF:
    case Fun_ANJF:
        subcode = 0x11;
        break;
    case Fun_DD:
        subcode = 0x39;
        break;
    case Fun_MD:
        subcode = 0x40;
        break;
    case Fun_SDJS:
        subcode = 0x23;
        break;
    case Fun_QGJ:
        subcode = 0x25;
        break;
    case Fun_XYDZS:
    {
        int size = DevArray.size();
        CXHD *pXHD;
        for (int i = 0; i < size; i++)
        {
            if (DevArray[i]->getDevType() == Dev_XH)
            {
                pXHD = (CXHD*)DevArray[i];
                if (!pXHD->getSX())
                {
                    Devcode = pXHD->getCode();
                    break;
                }
            }
        }
        subcode = 0x13;
        break;
    }
    case Fun_SYDZS:
    {
        int size = DevArray.size();
        CXHD *pXHD;
        for (int i = 0; i < size; i++)
        {
            if (DevArray[i]->getDevType() == Dev_XH)
            {
                pXHD = (CXHD*)DevArray[i];
                if (pXHD->getSX())
                {
                    Devcode = pXHD->getCode();
                    break;
                }
            }
        }
        subcode = 0x13;
        break;
    }
    default:
        subcode = 0;
        break;
    }
    array[index++] = subcode;
    memcpy(array.data() + index, &Devcode, 2);
    index += 2;

    index = 0x1E;//兼容已有协议  固定字节
    memcpy(array.data() + 4, &index, 2);
    for (int i = 0; i < 4; i++)
    {
        array[index - i - 1] = FRAMTAIL;
    }
    if (pData != nullptr)
    {
        pData->SendUdpMessage(array, index, this);
    }
    return index;
}

void CStationJSJ::setBSState(unsigned char *rec, int len)
{
    int bzdLen = v_LZBSArray.size() * 2;
    unsigned char state[1024] = {0};
    CBS *pBS = nullptr;
    memcpy(state, rec, bzdLen * 2);
    for (int i = 0; i < v_LZBSArray.size(); i++)
    {
        pBS = (CBS *)v_LZBSArray[i];
        BYTE ZDJ = DOWN;
        BYTE FDJ = DOWN;
        BOOL flag = FALSE;
        if (state[2 * i] == 0x55) //1+2*i  --2020.2.12-BJT-场联等邻站设备配合扩展协议
        {
            ZDJ = UP;
            pBS->m_nBZDJK[1].inFlag = ZDIAN_BS;
            flag = TRUE;
        }
        if (state[2 * i + 1] == 0x55) //1+2*i+1  --2020.2.12-BJT-场联等邻站设备配合扩展协议
        {
            FDJ = UP;
            pBS->m_nBZDJK[1].inFlag = FDIAN_BS;
            flag = TRUE;
        }
        if (flag == FALSE)
        {
            pBS->m_nBZDJK[1].inFlag = 0;
        }
        pBS->m_nBZDJK[1].ZXJ = ZDJ;
        pBS->m_nBZDJK[1].FXJ = FDJ;
    }
    memcpy(state, rec + bzdLen, len - bzdLen);
    for (unsigned int i = 0; i < v_LZBZDArray.size(); i++)
    {
        for (unsigned int j = 0; j < v_LZBZDArray.size(); j++)
        {
            if (v_LZBZDArray[j]->m_nZDBSIDinLS == i)
            {
                if (state[6 * i] == 0x11)
                    v_LZBZDArray[j]->LZQJKXState = UP;
                else
                    v_LZBZDArray[j]->LZQJKXState = DOWN;
                if (state[6 * i + 1] == 0x11)
                    v_LZBZDArray[j]->LZXHZCJState = UP;
                else
                    v_LZBZDArray[j]->LZXHZCJState = DOWN;
                if (state[6 * i + 2] == 0x11)
                    v_LZBZDArray[j]->LZJCFZANflag = UP;
                else
                    v_LZBZDArray[j]->LZJCFZANflag = DOWN;
                if (state[6 * i + 3] == 0x11)
                    v_LZBZDArray[j]->LZFCFZANflag = UP;
                else
                    v_LZBZDArray[j]->LZFCFZANflag = DOWN;
                v_LZBZDArray[j]->LZFaCheArrow = state[6 * i + 4];

                if (state[6 * i + 4] == 0xcc)
                    v_LZBZDArray[j]->LZFaCheEnabled = DOWN;
                v_LZBZDArray[j]->LZGFANState = ((0x55 == state[6 * i + 5]) ? (UP) : (DOWN));
                break;
            }
        }
    }
    PackBSMessage();
}

void CStationJSJ::InitZDBSState()
{
    for (QVector<CLZZDBS* >::iterator it = v_LZBZDArray.begin(); it != v_LZBZDArray.end(); it++)
    {
        (*it)->InitState();
    }
}

void CStationJSJ::InitBSState()
{
    for (QVector<CBS*>::iterator it = v_LZBSArray.begin(); it != v_LZBSArray.end(); it++)
    {
        (*it)->InitState();
    }
}

int CStationJSJ::PackButtonMessage()
{
    QByteArray array(100, 0);
    int index = 9;
    unsigned char subcode = 0;
    int DevCode = 0;
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    array[index++] = 0x88;
    if (ButtonArray.size() < 2)
        return 0;
    if (ButtonArray[0].ANType == LCAN)
        subcode = 0x01;
    else if (ButtonArray[0].ANType == TGAN)
        subcode = 0x03;
    else if (ButtonArray[0].ANType == DCAN)
        subcode = 0x02;
    array[index++] = subcode;
    for (QVector<ButtonStr>::iterator it = ButtonArray.begin(); it < ButtonArray.end(); it++)
    {
        subcode = 0;
        DevCode = 0;
        if (it->ANType == LCAN)
            subcode = 0x00;
        else if (it->ANType == DCAN)
            subcode = 0x01;
        else if (it->ANType == TGAN)
            subcode = 0x02;
        DevCode = (subcode << 14) | it->DevCode;
        memcpy(array.data() + index, &DevCode, 2);
        index = index + 2;
    }

    index = 0x1E;//兼容已有协议  固定字节
    memcpy(array.data() + 4, &index, 2);
    for (int i = 0; i < 4; i++)
    {
        array[index - i - 1] = FRAMTAIL;
    }
    if (pData != nullptr)
    {
        pData->SendUdpMessage(array, index, this);
    }
    return index;
}

int CStationJSJ::PackBSMessage()
{
    QByteArray buf(100, 0);
    int len;
    buf[0] = 0xBF;
    buf[1] = 0xBF;
    buf[2] = 0xBF;
    buf[3] = 0xBF;

    buf[6] = LZBZDFlag ? 0x55 : 0xaa;
    buf[7] = LZZDBSFlag ? 0x55 : 0xaa;

    int bsnum = 0;
    CBS *pBS = nullptr;

    len = 8;

    if (LZBZDFlag)
    {
        for (int index = 0; index < v_LZBSArray.size(); index++)
        {
            BOOL flag = false;
            pBS = v_LZBSArray[index];
            if (pBS->m_nBZDJK[1].ZDJ == UP)
            {
                pBS->m_nBZDJK[1].outFlag = ZDIAN_BS;
                flag = true;
                buf[len + 2 * index] = 0x55;
            }
            else
            {
                buf[len + 2 * index] = 0xaa;
            }
            if (pBS->m_nBZDJK[1].FDJ == UP)
            {
                pBS->m_nBZDJK[1].outFlag = FDIAN_BS;
                flag = true;
                buf[len + 2 * index + 1] = 0x55;
            }
            else
            {
                buf[len + 2 * index + 1] = 0xaa;
            }
            if (false == flag)
            {
                pBS->m_nBZDJK[1].outFlag = 0;
            }
        }
    }
    len = 8 + 3 * v_LZBSArray.size();
    if (LZZDBSFlag)
    {
        for (int index = 0; index < v_LZBZDArray.size(); index++)
        {
            for (int index1 = 0; index1 < v_LZBZDArray.size(); index1++)
            {
                if (v_LZBZDArray[index1]->m_nZDBSIDinLS == index) //动态适应顺序，按照联锁中配置顺序打包数据
                {
                    buf[len + index * 6] = 0x11; //教师机默认区间区段空闲

                    if (v_LZBZDArray[index1]->XHZCJState == UP)
                    {
                        buf[len + index * 6 + 1] = 0x11;
                    }
                    else
                    {
                        buf[len + index * 6 + 1] = 0x22;
                    }
                    if (v_LZBZDArray[index1]->JCFZAN == UP)
                        buf[len + index * 6 + 2] = 0x11; //接车辅助按钮
                    else
                        buf[len + index * 6 + 2] = 0x22;
                    if (v_LZBZDArray[index1]->FCFZAN == UP)
                        buf[len + index * 6 + 3] = 0x11; //发车辅助按钮
                    else
                        buf[len + index * 6 + 3] = 0x22;
                    buf[len + index * 6 + 4] = v_LZBZDArray[index1]->FaCheArrow;
                    buf[len + index * 6 + 5] = ((UP == v_LZBZDArray[index1]->GFANState) ? (0x55) : (0xaa));
                    break;
                }
            }
        }
    }
    len = len + v_LZBZDArray.size() * 6;
    len = len + 4;
    buf[4] = len;
    buf[len - 1] = 0xFB;
    buf[len - 2] = 0xFB;
    buf[len - 3] = 0xFB;
    buf[len - 4] = 0xFB;
    if (pData != nullptr)
    {
        pData->SendUdpTrainMessage(buf, len, this);
    }
    return len;
}

int CStationJSJ::PackZDBSFlag()
{
    QByteArray array(100, 0);
    int index = 4;
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    array[index++] = LZZDBSFlag ? 0xB5 : 0xB6;
    array[index++] = 1;

    index = 60;//联锁解析固定字节60
    for (int i = 0; i < 4; i++)
    {
        array[index - i - 1] = FRAMTAIL;
    }
    if (pData != nullptr)
    {
        pData->SendUdpMessage(array, index, this);
    }
    return index;
}

int CStationJSJ::PackBSFlag()
{
    QByteArray array(100, 0);
    int index = 4;
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    array[index++] = LZBZDFlag ? 0xB1 : 0xB2;
    array[index++] = 1;

    index = 60;//联锁解析固定字节60
    for (int i = 0; i < 4; i++)
    {
        array[index - i - 1] = FRAMTAIL;
    }
    if (pData != nullptr)
    {
        pData->SendUdpMessage(array, index, this);
    }
    return index;
}


void CStationJSJ::ButtonClicked(QPushButton *pBtn)
{
    qDebug() << pBtn->text();
    if (pBtn->text() == "命令下发")
    {
        PackButtonMessage();
        ClearButton();
    }
    else if (pBtn->text() == "命令取消")
    {
        ClearButton();
        emit ClearMenueButton();
    }
    else if (pBtn->text() == "自动闭塞模拟")
    {
        LZZDBSFlag = !LZZDBSFlag;
        if (LZZDBSFlag)
            InitZDBSState();
        PackZDBSFlag();
    }
    else if (pBtn->text() == "半自动闭塞模拟")
    {
        LZBZDFlag = !LZBZDFlag;
        if (LZBZDFlag)
            InitBSState();
        PackBSFlag();
    }
}

void CStationJSJ::ClearButton()
{
    CXHD *pXHD;
    CTG *pTG;
    for (int i = 0; i < ButtonArray.size(); i++)
    {
        if (ButtonArray[i].index < DevArray.size())
        {
            qDebug() << DevArray[ButtonArray[i].index]->getDevType();
            if (DevArray[ButtonArray[i].index]->getDevType() == Dev_XH)
            {
                pXHD  = (CXHD *)DevArray[ButtonArray[i].index];
                pXHD->setCmdButtonFlag(0);
            }
            else if (DevArray[ButtonArray[i].index]->getDevType() == Dev_TG)
            {
                pTG  = (CTG *)DevArray[ButtonArray[i].index];
                pTG->setCmdflag(false);
            }
        }
    }
    ButtonArray.clear();
}

void CStationJSJ::updateTrainPosition(unsigned int devCode, CTrain train)
{
    //    int size = DevArray.size();
    //    CGD *pGD;
    //    for (int i = 0; i < size; i++)
    //    {
    //        if (DevArray[i]->getDevType() == Dev_GD)
    //        {
    //            pGD = (CGD*)DevArray[i];
    //            if (pXHD->getSX())
    //            {
    //                Devcode = pXHD->getCode();
    //                break;
    //            }
    //        }
    //    }
}

void CStationJSJ::AnalyseTrainMsg(unsigned char *rec)
{

}

QString CStationJSJ::getQDName(unsigned int QDCode, CTrain *pTrain)
{
    int size = DevArray.size();
    CGDDC *pDC = nullptr;
    CGD *pGD = nullptr;
    QString result = "";
    for (int i = 0; i < size; i++)
    {
        if (DevArray[i]->getDevType() == Dev_GD)
        {
            pGD = (CGD*)DevArray[i];
            if (pGD->getCode() == QDCode)
            {
                pGD->pTrain = pTrain;
                result = DevArray[i]->getName();
            }
            else if (pGD->pTrain == pTrain)
                pGD->pTrain = nullptr;
        }
        else if (DevArray[i]->getDevType() == Dev_DC)
        {
            pDC = (CGDDC *)DevArray[i];
            if (pDC->getQDCode() == QDCode)
            {
                pDC->pTrain = pTrain;
                result = pDC->getName();
            }
            else if (pDC->pTrain == pTrain)
                pDC->pTrain = nullptr;
        }
    }
    return result;
}

void CStationJSJ::TimeProc()
{
    for (int i = 0; i < v_LZBSArray.size(); i++)
    {
        v_LZBSArray[i]->bzdProc();
    }
}

void CStationJSJ::DealMenuAction(QAction * action)
{
}

void CStationJSJ::slot_FlblDlgOK(int zcCount, int devCount, bool flag1, bool flag2, bool flag3)
{
    if (devCount >= DevArray.size())
    {
        return;
    }
    if (DevArray[devCount]->getDevType() == Dev_DC)
    {
        CGDDC *gddc;
        gddc = (CGDDC *)DevArray[devCount];

        if (gddc->getIsFLBL_CQ() != flag1)
        {
            PackGZMessage(CQFLBL, gddc->getCode(), flag1);
        }
        if (gddc->getIsFLBL_DW() != flag2)
        {
            PackGZMessage(DWFLBL, gddc->getCode(), flag2);
        }
        if (gddc->getIsFLBL_FW() != flag3)
        {
            PackGZMessage(FWFLBL, gddc->getCode(), flag3);
        }

        gddc->setIsFLBL_CQ(flag1);
        gddc->setIsFLBL_DW(flag2);
        gddc->setIsFLBL_FW(flag3);
    }
    else if (DevArray[devCount]->getDevType() == Dev_GD)
    {
        CGD *gd;
        gd = (CGD *)DevArray[devCount];
        if (gd->getGDFLBL() != flag1)
        {
            PackGZMessage(QDFLBLFLAG, gd->getCode(), flag1);
        }
        gd->setGDFLBL(flag1);
    }
}


