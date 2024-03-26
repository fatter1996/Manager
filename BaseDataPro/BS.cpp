#include "BS.h"
#include "Arrow.h"
#include "BUTTON.h"
CBS::CBS(void)
{
    for (int i = 0; i < 4; i++)
    {
        m_BSState[i] = DOWN;
        m_Times[i] = 0;
        isChange[i] = FALSE;
    }
    //m_pRelayDlg = NULL;

    //20190121
#if 0
    pointBtnJZ.SetPoint(0, 0);
    pointBtnCZ.SetPoint(0, 0);
    rectBtnJZ.SetRect(0, 0, 0, 0);
    rectBtnCZ.SetRect(0, 0, 0, 0);
    rectTxtLZ.SetRect(0, 0, 0, 0);
#endif
    m_bBtnJZDown = m_bBtnCZDown = FALSE;

    GZType = 0;

    /*****************************2020.2.25-BJT-???????????????***********************************/
    isHaveJZ = FALSE;
    isJZUseFlag = FALSE;
    isLZFCJLFlag = FALSE;
    isNeedBSQQ = FALSE;
    isNeedBSTYtime = 0;
    isNeedBSTY = FALSE;
    /***************************end-2020.2.25-BJT-???????????????*********************************/

    nBS_BtnTime = 0;
    nFY_BtnTime = 0;
    nSG_BtnTime = 0;
    //???????????
    JZFaultFlag = FALSE;
    JZDifferentFlag = FALSE;
    JZCount = 0;
}

CBS::~CBS(void)
{
}
void CBS::initBS()
{
    m_textRectConst = m_textRect;
    inArrow = new CArrow;
    outArrow = new CArrow;
#ifndef ZCZX
    if (!m_nSX)
    {
        inArrow->p.setX(m_textRect.left() + 20);
        inArrow->p.setY(m_textRect.top());
        inArrow->m_nType = ARROW_RIGHT ;
        outArrow->p.setX(m_textRect.left() - 25);
        outArrow->p.setY(m_textRect.top());
        outArrow->m_nType = ARROW_LEFT;
    }
    else
    {
        outArrow->p.setX(m_textRect.left() + 20);
        outArrow->p.setY(m_textRect.top());
        outArrow->m_nType = ARROW_RIGHT;
        inArrow->p.setX(m_textRect.left() - 25);
        inArrow->p.setY(m_textRect.top());
        inArrow->m_nType =  ARROW_LEFT;
    }
#else
    if (m_nSX)
    {
        inArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        inArrow->m_nType = ARROW_RIGHT;
        outArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
        outArrow->m_nType = ARROW_LEFT;
    }
    else
    {
        outArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        outArrow->m_nType = ARROW_RIGHT;
        inArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
        inArrow->m_nType = ARROW_LEFT;
    }
#endif
    pBlock = new CBUTTON;
    pBlock->m_nType = BLOCK_BUTTON;
    pBlock->pCenter.setX(m_textRect.left() - 30);
    pBlock->pCenter.setY(m_textRect.bottom() + 8);
    pBlock->BUTTONInit();
    pRecover = new CBUTTON;
    pRecover->m_nType = RECOVER_BUTTON;
    pRecover->pCenter.setX(m_textRect.left());
    pRecover->pCenter.setY(m_textRect.bottom() + 8);
    pRecover->BUTTONInit();
    pFault = new CBUTTON;
    pFault->m_nType = FAULT_BUTTON;
    pFault->pCenter.setX(m_textRect.left() + 30);
    pFault->pCenter.setY(m_textRect.bottom() + 8);
    pFault->BUTTONInit();

    //20190121
    pointBtnJZ.setX(m_textRect.left() + 80);
    pointBtnJZ.setY(m_textRect.bottom());
    pointBtnCZ.setX(m_textRect.left() + 130);
    pointBtnCZ.setY(m_textRect.bottom());
    rectTxtLZ.setRect(m_textRect.left() + 80, m_textRect.top(), 15, 15);
    rectBtnJZ.setRect(pointBtnJZ.x() - OffPix1, pointBtnJZ.y() - OffPix1, 15, 15);
    rectBtnCZ.setRect(pointBtnCZ.x() - OffPix1, pointBtnCZ.y() - OffPix1, 15, 15);

#ifdef _BZD_INTERFACE
    m_JSBJPoint.setX(m_textRect.left() - 30);
    m_JSBJPoint.setY(m_textRect.bottom() + 30);

    m_FSBJPoint.setX(m_textRect.left());
    m_FSBJPoint.setY(m_textRect.bottom() + 30);

    m_GJFPoint.setX(m_textRect.left() + 30);
    m_GJFPoint.setY(m_textRect.bottom() + 30);

    InitRelay(0);
    InitRelay(1);

    m_nBZDJK[1].GJF = UP;
    m_nBZDJK[1].BSAJ = DOWN;
    m_nBZDJK[1].SGAJ = DOWN;
    m_nBZDJK[1].FUAJ = DOWN;

#endif
    /*****************************2020.2.25-BJT-???????????????***********************************/
    pointBtnLZFCJL.setX(m_textRect.left() - 30);
    pointBtnLZFCJL.setY(m_textRect.bottom() + 30);
    pointBtnJZuse.setX(m_textRect.left() + 45);
    pointBtnJZuse.setY(m_textRect.bottom() + 30);
    pointBtnJZStopuse.setX(m_textRect.left() + 110);
    pointBtnJZStopuse.setY(m_textRect.bottom() + 30);
    pointBtnLZFCJLConst = pointBtnLZFCJL;
    pointBtnJZuseConst = pointBtnJZuse;
    pointBtnJZStopuseConst = pointBtnJZStopuse;
    pointBtnJZZero.setX(m_textRect.left() + 175);
    pointBtnJZZero.setY(m_textRect.bottom() + 30);
    pointBtnJZZeroConst = pointBtnJZZero;
    /***************************end-2020.2.25-BJT-???????????????*********************************/
}
void CBS::InitRelay(BYTE i)
{
    m_nBZDJK[i].HDJ = DOWN;
    m_nBZDJK[i].TJJ = DOWN;
    m_nBZDJK[i].TCJ = DOWN;
    m_nBZDJK[i].BSJ = DOWN;
    m_nBZDJK[i].KTJ = DOWN;
    m_nBZDJK[i].ZKJ = DOWN;
    m_nBZDJK[i].XZJ = DOWN;
    m_nBZDJK[i].FUJ = DOWN;
    m_nBZDJK[i].GDJ = DOWN;
    m_nBZDJK[i].JSBJ = DOWN;
    m_nBZDJK[i].FSBJ = UP;
    m_nBZDJK[i].JBDH = DOWN;
    m_nBZDJK[i].JBDU = DOWN;
    m_nBZDJK[i].JBDL = DOWN;
    m_nBZDJK[i].FBDH = UP;
    m_nBZDJK[i].FBDU = DOWN;
    m_nBZDJK[i].FBDL = DOWN;

    m_nBZDJK[i].FDJtime = 0;
    m_nBZDJK[i].ZDJtime = 0;
    m_nBZDJK[i].HDJtime = 0;
    m_nBZDJK[i].XZJtime = 0;
    m_nBZDJK[i].ZKJtime = 0;
}

void CBS::InitState()
{
    InitRelay(0);
    InitRelay(1);

    m_nBZDJK[1].GJF = UP;
    m_nBZDJK[1].BSAJ = DOWN;
    m_nBZDJK[1].SGAJ = DOWN;
    m_nBZDJK[1].FUAJ = DOWN;
}

void CBS::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx)
{
    QFont font;
    font.setFamily("宋体");

    font.setPointSize(12 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    painter->drawText(m_textRect.left(), m_textRect.top(), m_strName);

    inArrow->DRAW(painter, nDiploid, nElapsed);
    outArrow->DRAW(painter, nDiploid, nElapsed);
    pBlock->DRAW(painter, nDiploid, nElapsed);
    pFault->DRAW(painter, nDiploid, nElapsed);
    pRecover->DRAW(painter, nDiploid, nElapsed);

    //DRAW BTN 20190121
    Qt::GlobalColor BtnColor;
    if (m_bBtnJZDown == TRUE)
    {
        BtnColor = Qt::yellow;
        m_bBtnJZDown = FALSE;
    }
    else
    {
        BtnColor = Qt::gray;
    }
    painter->fillRect(rectBtnJZ, QBrush(BtnColor));

    if (m_bBtnCZDown == TRUE)
    {
        BtnColor = Qt::yellow;
        m_bBtnCZDown = FALSE;
    }
    else
    {
        BtnColor = Qt::gray;
    }
    painter->fillRect(rectBtnCZ, QBrush(BtnColor));
    QRect  rectTxtJZ, rectTxtCZ;
    QFontMetrics fm = painter->fontMetrics();
    rectTxtJZ.setRect(pointBtnJZ.x() - fm.width(BZDMN_LCJZ) / 2, pointBtnJZ.y() + OffPix1 + 1, fm.width(BZDMN_LCJZ), pointBtnJZ.y() + OffPix1 + 1 + fm.height());
    rectTxtCZ.setRect(pointBtnCZ.x() - fm.width(BZDMN_LCCZ) / 2, pointBtnCZ.y() + OffPix1 + 1, fm.width(BZDMN_LCCZ), pointBtnCZ.y() + OffPix1 + 1 + fm.height());

    //painter->drawText(rectTxtLZ, BZDMN_TXTLZ);
    painter->drawText(rectTxtJZ, BZDMN_LCJZ);
    painter->drawText(rectTxtCZ, BZDMN_LCCZ);
    /*****************************2020.2.25-BJT-???????????????***********************************/
#if 1
    if (TRUE == isHaveJZ)
    {
        QPoint pt(pointBtnLZFCJL.x(), pointBtnLZFCJL.y());
        QString name_Bt = "";
        QString image = "";
        if (TRUE == isLZFCJLFlag)
        {
            image = "://bmp/IDB_TKFZANDOWN.bmp";
        }
        else
        {
            image = "://bmp/IDB_TKFZAN.bmp";
        }

        draw_Pixmap(painter, image, pointBtnLZFCJL.x(), pointBtnLZFCJL.y(), 15, 15, nDiploid);
        name_Bt = "发车进路触发";
        QFontMetrics fm = painter->fontMetrics();
        pt.setX((pointBtnLZFCJL.x() + 7 * nDiploid) - fm.width(name_Bt) / 2);
        pt.setY(pointBtnLZFCJL.y() + 20 * nDiploid);
        painter->drawText(pt.x(), pt.y() + 15 * nDiploid / 2, name_Bt);

        pt.setX(pointBtnJZuse.x());
        pt.setY(pointBtnJZuse.y());
        if (TRUE == isJZUseFlag)
        {
            image = "://bmp/IDB_TKFZANDOWN.bmp";
        }
        else
        {
            image = "://bmp/IDB_TKFZAN.bmp";
        }
        draw_Pixmap(painter, image, pt.x(), pt.y(), 15, 15, nDiploid);
        name_Bt = "计轴使用";
        pt.setX(pointBtnJZuse.x() + 7 * nDiploid - fm.width(name_Bt) / 2);
        pt.setY(pointBtnJZuse.y() + 20 * nDiploid);
        painter->drawText(pt.x(), pt.y() + 15 * nDiploid / 2, name_Bt);
        if (TRUE == isJZUseFlag)
        {
            image = "://bmp/IDB_TKFZAN.bmp";
        }
        else
        {
            image = "://bmp/IDB_TKFZANDOWN.bmp";
        }
        draw_Pixmap(painter, image, pointBtnJZStopuse.x(), pointBtnJZStopuse.y(), 15, 15, nDiploid);
        name_Bt = "计轴停用";
        pt.setX((pointBtnJZStopuse.x() + 7 * nDiploid) - fm.width(name_Bt) /   2);
        pt.setY(pointBtnJZStopuse.y() + 20 * nDiploid); //?????????????????豸????????????????????
        painter->drawText(pt.x(), pt.y() + 15 * nDiploid / 2, name_Bt);
        image = "://bmp/IDB_TKFZAN.bmp";
        draw_Pixmap(painter, image, pointBtnJZZero.x(), pointBtnJZZero.y(), 15, 15, nDiploid);
        name_Bt = "计轴复零";
        pt.setX((pointBtnJZZero.x() + 7 * nDiploid) - fm.width(name_Bt) / 2);
        pt.setY(pointBtnJZZero.y() + 20 * nDiploid);
        painter->drawText(pt.x(), pt.y() + 15 * nDiploid / 2, name_Bt);
        CountNUM(painter, QPoint(pt.x() + 15, pt.y() + 20), JZCount);
    }
#endif
    /***************************end-2020.2.25-BJT-???????????????*********************************/
}

void CBS::SetState(int i)
{
    if (i < 0 || i > 6)
        return;
    if (i == 1)
    {
#ifndef _BZD_INTERFACE
        isChange[2] = TRUE;

        m_BSState[2] = UP;
#else
        m_nBZDJK[1].BSAJ = UP;
        isChange[1] = TRUE;
        nBS_BtnTime = 12;
        m_Times[i] = 0;
#endif
        pBlock->setState(ALL);
    }
    else if (i == 3)
    {
#ifdef _BZD_INTERFACE
        m_nBZDJK[1].SGAJ = UP;
        isChange[3] = TRUE;
        nSG_BtnTime = 12;
        m_Times[i] = 0;
#endif
        pFault->setState(ALL);
    }
    else if (i == 2)
    {
#ifdef _BZD_INTERFACE
        m_nBZDJK[1].FUAJ = UP;
        isChange[2] = TRUE;
        nFY_BtnTime = 12;
        m_Times[i] = 0;
#endif
        pRecover->setState(ALL);
#ifndef _BZD_INTERFACE
        isChange[3] = TRUE;
        m_BSState[3] = UP;
#endif
    }
#ifdef _BZD_INTERFACE
    else
    {
        BYTE *tmpStat[] = {&m_nBZDJK[1].JSBJ, &m_nBZDJK[1].FSBJ, &m_nBZDJK[1].GJF};
        *tmpStat[i - 4] = (*tmpStat[i - 4] == UP) ? DOWN : UP;
//		if (i == 4 && *tmpStat[0] == UP)
//			*tmpStat[1] = DOWN;
//		else if (i == 5 && *tmpStat[1] == UP)
//			*tmpStat[0] = DOWN;
    }
#endif
}

void CBS::SetState(int i, BYTE nState)
{
    m_BSState[i] = nState;
}

int CBS::moveCursor(QPoint p)
{
    if ((pBlock->moveCursor(p)) && (!((isHaveJZ == TRUE) && (isJZUseFlag == TRUE)))) //2020.2.25-BJT-???????????????
        return 1;
    if ((pRecover->moveCursor(p)) && (!((isHaveJZ == TRUE) && (isJZUseFlag == TRUE)))) //2020.2.25-BJT-???????????????
        return 2;
    if (pFault->moveCursor(p))
        return 3;
#ifdef _BZD_INTERFACE
    //CPoint tmpPoint[] = {m_JSBJPoint,m_FSBJPoint,m_GJFPoint};
    //for (int i = 0; i < 3; i++)
    //{
    //	CRect tmpRect(tmpPoint[i].x, tmpPoint[i].y + 5, tmpPoint[i].x + 16, tmpPoint[i].y + 20);
    //	if (PtInRect(tmpRect, p))
    //		return (i + 4);
    //}
#endif

    if (rectBtnJZ.contains(p))
        return 8;
    if (rectBtnCZ.contains(p))
        return 9;

    /*****************************2020.2.25-BJT-???????????????***********************************/
    if (TRUE == isHaveJZ)
    {
        QRect m_rect;
        m_rect.setRect(pointBtnLZFCJL.x(), pointBtnLZFCJL.y(), 15, + 15);
        if ((m_rect.contains(p)) && ((isHaveJZ == TRUE) && (isJZUseFlag == TRUE)))
        {
            return 10;
        }

        m_rect.setRect(pointBtnJZuse.x(), pointBtnJZuse.y(), 15, 15);
        if (m_rect.contains(p))
        {
            return 11;
        }

        m_rect.setRect(pointBtnJZStopuse.x(), pointBtnJZStopuse.y(), 15, 15);
        if (m_rect.contains(p))
        {
            return 12;
        }

        m_rect.setRect(pointBtnJZZero.x(), pointBtnJZZero.y(), 15, 15);
        if (m_rect.contains(p))
        {
            return 13;
        }
    }
    /***************************end-2020.2.25-BJT-???????????????*********************************/

    return 0;
}

#ifdef _BZD_INTERFACE
void CBS::BZDSetHDJ(BYTE i)
{
    if ((m_nBZDJK[i].JSBJ == UP) && (m_nBZDJK[i].GDJ == DOWN) && (m_nBZDJK[i].TJJ == DOWN) && (m_nBZDJK[i].TCJ == UP))
    {
        m_nBZDJK[i].HDJ = UP;
        m_nBZDJK[i].HDJtime = 0;
    }
    else if ((m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].TCJ == UP))
    {
        m_nBZDJK[i].HDJ = UP;
        m_nBZDJK[i].HDJtime = 0;
    }
    else if ((m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].ZXJ == UP) && (m_nBZDJK[i].ZKJ == DOWN) && (m_nBZDJK[i].TCJ == DOWN))
    {
        m_nBZDJK[i].HDJ = UP;
        m_nBZDJK[i].HDJtime = 0;
    }
    else if ((m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].HDJtime < T1S))
        m_nBZDJK[i].HDJ = UP;
    else
        m_nBZDJK[i].HDJ = DOWN;
}

void CBS::BZDSetXZJ(BYTE i)
{
    if (((m_nBZDJK[i].XZJ == UP) || (m_nBZDJK[i].ZDJ == UP)) &&
            ((m_nBZDJK[i].FSBJ == UP) || (m_nBZDJK[i].KTJ == DOWN)) &&
            (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FUJ == DOWN) && (m_nBZDJK[i].FDJ == DOWN))
    {
        m_nBZDJK[i].XZJ = UP;
        m_nBZDJK[i].XZJtime = 0;
    }
    else if ((m_nBZDJK[i].ZDJ == DOWN) && (m_nBZDJK[i].KTJ == UP) && (m_nBZDJK[i].FSBJ == UP) &&
             (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FUJ == DOWN) && (m_nBZDJK[i].FDJ == DOWN))
    {
        m_nBZDJK[i].XZJ = UP;
        m_nBZDJK[i].XZJtime = 0;
    }
    else if ((m_nBZDJK[i].XZJ == UP) && (m_nBZDJK[i].XZJtime < T1S))
        m_nBZDJK[i].XZJ = UP;
    else
        m_nBZDJK[i].XZJ = DOWN ;
}

void CBS::BZDSetTJJ(BYTE i)
{
    if ((m_nBZDJK[i].FUJ == DOWN) && (m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].ZXJ == DOWN) && (m_nBZDJK[i].BSJ == UP))
        m_nBZDJK[i].TJJ = UP;
    else if ((m_nBZDJK[i].TJJ == UP) && ((m_nBZDJK[i].FUJ == DOWN) || (m_nBZDJK[i].BSJ == DOWN)) && (m_nBZDJK[i].GDJ == DOWN))
        m_nBZDJK[i].TJJ = UP;
    else
        m_nBZDJK[i].TJJ = DOWN;
}

void CBS::BZDSetFDJ(BYTE i)
{
    if ((m_nBZDJK[i].TCJ == DOWN) && (m_nBZDJK[i].HDJ == UP) &&
            ((m_nBZDJK[i].TJJ == DOWN && m_nBZDJK[i].XZJ == UP && m_nBZDJK[i].ZKJ == UP) || m_nBZDJK[i].TJJ == UP)
            && (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }
    else if ((m_nBZDJK[i].JSBJ == DOWN) && (m_nBZDJK[i].FUAJ == UP) && (m_nBZDJK[i].XZJ == UP) &&
             (m_nBZDJK[i].ZKJ == UP) && (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }
    else if ((m_nBZDJK[i].SGAJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }

    else if ((m_nBZDJK[i].JSBJ == DOWN) && (m_nBZDJK[i].FUAJ == UP) && ((m_nBZDJK[i].TJJ == DOWN) || ((m_nBZDJK[i].TJJ == UP) && (m_nBZDJK[i].XZJ == UP) && (m_nBZDJK[i].ZKJ == UP)))
             && (m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].TCJ == UP) && (m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }
    else if ((m_nBZDJK[i].FDJ == UP) && (m_nBZDJK[i].FDJtime < T3S))
    {
        m_nBZDJK[i].FDJ = UP;
    }
    else
    {
        m_nBZDJK[i].FDJ = DOWN;
    }
}

void CBS::BZDSetZDJ(BYTE i)
{
//    if (i == 1)
//    {
//        if (m_nBZDJK[1].BSAJ == UP)
//        {
//            QString str;
//            str = QString("%1 HDJ=%2   KTJ=%3  TJJ=%4   BSJ=%5   FXJ=%6   ZXJ=%7   ZKJ=%8   ZDJtime=%9\n").arg(m_strName).arg(m_nBZDJK[1].HDJ).arg(m_nBZDJK[1].KTJ).arg(m_nBZDJK[1].TJJ).arg(m_nBZDJK[1].BSJ).arg(m_nBZDJK[1].FXJ).arg(m_nBZDJK[1].ZXJ).arg(m_nBZDJK[1].ZKJ).arg(m_nBZDJK[1].ZDJtime);
//            qDebug() << (str);
//        }
//    }
    if ((m_nBZDJK[i].HDJ == DOWN) && ((m_nBZDJK[i].KTJ == UP) || ((m_nBZDJK[i].BSAJ == UP) && (m_nBZDJK[i].TJJ == UP))) &&
            (m_nBZDJK[i].BSJ == DOWN) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].ZDJ = UP;
        m_nBZDJK[i].ZDJtime = 0;
    }
    else if ((m_nBZDJK[i].HDJ == DOWN) && (m_nBZDJK[i].BSAJ == UP) && (m_nBZDJK[i].TJJ == DOWN) && (m_nBZDJK[i].ZKJ == DOWN) &&
             (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        m_nBZDJK[i].ZDJ = UP;
        m_nBZDJK[i].ZDJtime = 0;
    }
    else if ((m_nBZDJK[i].ZDJ == UP) && (m_nBZDJK[i].ZDJtime < T3S)) //T3S
    {
        m_nBZDJK[i].ZDJ = UP;
    }
    else
    {
        m_nBZDJK[i].ZDJ = DOWN;
    }
}

void CBS::BZDSetZKJ(BYTE i)
{
    if ((((m_nBZDJK[i].XZJ == UP) && (m_nBZDJK[i].FXJ == UP)) || (m_nBZDJK[i].ZKJ == UP)) &&
            (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FUJ == DOWN) && (m_nBZDJK[i].FDJ == DOWN))
    {
        m_nBZDJK[i].ZKJ = UP;
        m_nBZDJK[i].ZKJtime = 0;
    }
    else if ((m_nBZDJK[i].ZKJ == UP) && (m_nBZDJK[i].ZKJtime < T1S))
    {
        m_nBZDJK[i].ZKJ = UP;
    }
    else
    {
        m_nBZDJK[i].ZKJ = DOWN;
    }
}

void CBS::BZDSetGDJ(BYTE i)
{
    if (((m_nBZDJK[i].ZKJ == UP) || (m_nBZDJK[i].TCJ == UP)) && (m_nBZDJK[i].GJF == UP))
    {
        m_nBZDJK[i].GDJ = UP;
    }
    else
    {
        m_nBZDJK[i].GDJ = DOWN;
    }
}

void CBS::BZDSetKTJ(BYTE i)
{
    if ((((m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].ZXJ == UP)) || (m_nBZDJK[i].KTJ == UP)) && (m_nBZDJK[i].ZKJ == UP))
    {
        m_nBZDJK[i].KTJ = UP;
    }
    else
    {
        m_nBZDJK[i].KTJ = DOWN;
    }
}

void CBS::BZDSetTCJ(BYTE i)
{
    if ((m_nBZDJK[i].BSJ == DOWN) && ((m_nBZDJK[i].TCJ == UP) || ((m_nBZDJK[i].TJJ == UP) && (m_nBZDJK[i].ZXJ == UP))))
    {
        m_nBZDJK[i].TCJ = UP;
    }
    else
    {
        m_nBZDJK[i].TCJ = DOWN;
    }
}

void CBS::BZDSetBSJ(BYTE i)
{
    if ((m_nBZDJK[i].KTJ == DOWN) && (m_nBZDJK[i].GDJ == DOWN) && (m_nBZDJK[i].BSJ == UP) &&
            ((m_nBZDJK[i].HDJ == UP) || (m_nBZDJK[i].FDJ == UP) || (m_nBZDJK[i].TJJ == DOWN) || (m_nBZDJK[i].BSAJ == DOWN)))
    {
        m_nBZDJK[i].BSJ = UP;
    }
    else if ((m_nBZDJK[i].KTJ == UP) && (m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].BSJ == UP) &&
             ((m_nBZDJK[i].HDJ == UP) || (m_nBZDJK[i].FDJ == UP) || (m_nBZDJK[i].TJJ == DOWN) || (m_nBZDJK[i].BSAJ == DOWN)))
    {
        m_nBZDJK[i].BSJ = UP;
    }
    else if ((m_nBZDJK[i].XZJ == UP) && (m_nBZDJK[i].BSJ == UP) &&
             ((m_nBZDJK[i].HDJ == UP) || (m_nBZDJK[i].FDJ == UP) || (m_nBZDJK[i].TJJ == DOWN) || (m_nBZDJK[i].BSAJ == DOWN)))
    {
        m_nBZDJK[i].BSJ = UP;
    }
    else if (m_nBZDJK[i].FUJ == UP)
    {
        m_nBZDJK[i].BSJ = UP;
    }
    else
    {
        m_nBZDJK[i].BSJ = DOWN;
    }
}

void CBS::BZDSetFUJ(BYTE i)
{
    if ((m_nBZDJK[i].FUAJ == UP) && (m_nBZDJK[i].TCJ == UP) && (m_nBZDJK[i].XZJ == DOWN) && (m_nBZDJK[i].FXJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else if ((m_nBZDJK[i].TCJ == DOWN) && (m_nBZDJK[i].XZJ == DOWN) && (m_nBZDJK[i].FXJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else if (((m_nBZDJK[i].SGAJ == UP) || (m_nBZDJK[i].FUJ == UP)) && (m_nBZDJK[i].FDJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else if ((m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].FDJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else  if ((m_nBZDJK[i].GDJ == UP) && ((m_nBZDJK[i].FUJ == UP) || (m_nBZDJK[i].SGAJ == UP)) &&
              (m_nBZDJK[i].TCJ == DOWN) && (m_nBZDJK[i].XZJ == DOWN) && (m_nBZDJK[i].FXJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else  if ((m_nBZDJK[i].GDJ == UP) && ((m_nBZDJK[i].FUJ == UP) || (m_nBZDJK[i].SGAJ == UP)) &&
              (m_nBZDJK[i].FUAJ == UP) && (m_nBZDJK[i].TCJ == UP) && (m_nBZDJK[i].XZJ == DOWN) && (m_nBZDJK[i].FXJ == UP))
    {
        m_nBZDJK[i].FUJ = UP;
    }
    else
    {
        m_nBZDJK[i].FUJ = DOWN;
    }

}

void CBS::BZDSetJBD(BYTE i)
{
    if (m_nBZDJK[i].TCJ == UP)
    {
        m_nBZDJK[i].JBDH = UP;
        m_nBZDJK[i].JBDL = DOWN;
        m_nBZDJK[i].JBDU = DOWN;
    }
    else if ((m_nBZDJK[i].HDJ == DOWN) && (m_nBZDJK[i].FDJ == DOWN) &&
             (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].TJJ == UP) && (m_nBZDJK[i].TCJ == DOWN))
    {
        m_nBZDJK[i].JBDU = UP;
        m_nBZDJK[i].JBDH = DOWN;
        m_nBZDJK[i].JBDL = DOWN;
    }
    else if ((m_nBZDJK[i].BSJ == DOWN) && (m_nBZDJK[i].TJJ == UP) && (m_nBZDJK[i].TCJ == DOWN))
    {
        m_nBZDJK[i].JBDL = UP;
        m_nBZDJK[i].JBDH = DOWN;
        m_nBZDJK[i].JBDU = DOWN;
    }
    else
    {
        m_nBZDJK[i].JBDH = DOWN;
        m_nBZDJK[i].JBDU = DOWN;
        m_nBZDJK[i].JBDL = DOWN;
    }
}

void CBS::BZDSetFBD(BYTE i)
{
    if ((m_nBZDJK[i].KTJ == UP) && (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].TJJ == DOWN) && (m_nBZDJK[i].TCJ == DOWN))
    {
        m_nBZDJK[i].FBDL = UP;
        m_nBZDJK[i].FBDH = DOWN;
        m_nBZDJK[i].FBDU = DOWN;
    }
    else if (((m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].TCJ == UP)) ||
             ((m_nBZDJK[i].BSJ == DOWN) && (m_nBZDJK[i].TJJ == DOWN) && (m_nBZDJK[i].TCJ == DOWN)))
    {
        m_nBZDJK[i].FBDH = UP;
        m_nBZDJK[i].FBDL = DOWN;
        m_nBZDJK[i].FBDU = DOWN;
    }
    else if ((m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].KTJ == DOWN) &&
             (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].TJJ == DOWN) && (m_nBZDJK[i].TCJ == DOWN))
    {
        m_nBZDJK[i].FBDU = UP;
        m_nBZDJK[i].FBDH = DOWN;
        m_nBZDJK[i].FBDL = DOWN;
    }
    else
    {
        m_nBZDJK[i].FBDH = DOWN;
        m_nBZDJK[i].FBDU = DOWN;
        m_nBZDJK[i].FBDL = DOWN;
    }

}
void CBS::BZDSetZXJFXJ(BYTE i)
{
    if (m_nBZDJK[i].inFlag == ZDIAN_BS)
    {
        m_nBZDJK[i].ZXJ = UP;
        m_nBZDJK[i].FXJ = DOWN;
    }
    else if (m_nBZDJK[i].inFlag == FDIAN_BS)
    {
        m_nBZDJK[i].ZXJ = DOWN;
        m_nBZDJK[i].FXJ = UP;
    }
    else
    {
        m_nBZDJK[i].ZXJ = DOWN;
        m_nBZDJK[i].FXJ = DOWN;
    }
}

unsigned int CBS::getDevType()
{
    return Dev_LZBZD;
}

void CBS::SetBZDinFlag()
{
    m_nBZDJK[0].inFlag = m_nBZDJK[1].outFlag;
    m_nBZDJK[1].inFlag = m_nBZDJK[0].outFlag;
}

void CBS::SetBZDoutFlag()
{
    for (int i = 0; i < 2; i++)
    {
        m_nBZDJK[i].outFlag = 0;
        if (m_nBZDJK[i].ZDJ == UP)
        {
            m_nBZDJK[i].outFlag = ZDIAN_BS;
        }
        else if (m_nBZDJK[i].FDJ == UP)
        {
            m_nBZDJK[i].outFlag = FDIAN_BS;
        }
    }
}

void CBS::bzdProc()
{
#ifdef _THJK
    BZDSetHDJ(0);
    BZDSetXZJ(0);
    BZDSetTJJ(0);
    BZDSetFDJ(0);
    BZDSetZDJ(0);
    BZDSetZKJ(0);
    BZDSetGDJ(0);
    BZDSetKTJ(0);
    BZDSetTCJ(0);
    BZDSetBSJ(0);
    BZDSetFUJ(0);
    BZDSetJBD(0);
    BZDSetFBD(0);
#endif
    //BZDSetZXJFXJ(0);

    //SetBZDinFlag();
    //SetBZDoutFlag();
    BYTE *tmpRelay[] = {NULL, &m_nBZDJK[1].BSAJ, &m_nBZDJK[1].FUAJ, &m_nBZDJK[1].SGAJ};
    for (int i = 1; i < 4; i++)
    {
        if (isChange[i])
        {
            if (m_Times[i]++ >= 9)
            {
                isChange[i] = FALSE;
#ifndef _BZD_INTERFACE
                m_BSState[i] = DOWN;
#else
                *tmpRelay[i] = DOWN;
#endif
            }
        }
        else
        {
            m_Times[i] = 0;
        }
    }

    BZDSetHDJ(1);
    BZDSetXZJ(1);
    BZDSetTJJ(1);
    BZDSetFDJ(1);
    BZDSetZDJ(1);
    BZDSetZKJ(1);
    BZDSetGDJ(1);
    BZDSetKTJ(1);
    BZDSetTCJ(1);
    BZDSetBSJ(1);
    BZDSetFUJ(1);
    BZDSetJBD(1);
    BZDSetFBD(1);
    BZDSetZXJFXJ(1);

    SetArrowState();
    UINT *tmpTimeElapsed[2][5] =
    {
        &m_nBZDJK[0].FDJtime, &m_nBZDJK[0].ZDJtime, &m_nBZDJK[0].HDJtime, &m_nBZDJK[0].XZJtime, &m_nBZDJK[0].ZKJtime,
        &m_nBZDJK[1].FDJtime, &m_nBZDJK[1].ZDJtime, &m_nBZDJK[1].HDJtime, &m_nBZDJK[1].XZJtime, &m_nBZDJK[1].ZKJtime
    };
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            (*tmpTimeElapsed[i][j])++;
            if (j < 2 && *tmpTimeElapsed[i][j] >= T4S)
                *tmpTimeElapsed[i][j] = T4S;
            else if (j >= 2 && *tmpTimeElapsed[i][j] >= T1S)
                *tmpTimeElapsed[i][j] = T1S;
        }
    }
    if (nBS_BtnTime > 0)
    {
        nBS_BtnTime--;
    }
    else if (nBS_BtnTime == 0)
    {
        pBlock->setState(0);
        nBS_BtnTime--;
    }
    if (nFY_BtnTime > 0)
    {
        nFY_BtnTime--;
    }
    else if (nFY_BtnTime == 0)
    {
        pRecover->setState(0);
        nFY_BtnTime--;
    }
    if (nSG_BtnTime > 0)
    {
        nSG_BtnTime--;
    }
    else if (nSG_BtnTime == 0)
    {
        pFault->setState(0);
        nSG_BtnTime--;
    }

    JZLogicProc();
    JZTimerProc();
}

void CBS::SetArrowState()
{
    if (m_nBZDJK[1].FBDH == UP)
        outArrow->SetState(STATE_RED);
    else if (m_nBZDJK[1].FBDU == UP)
        outArrow->SetState(STATE_YELLOW);
    else if (m_nBZDJK[1].FBDL == UP)
        outArrow->SetState(STATE_GREEN);
    else
        outArrow->SetState(STATE_GRAY);

    if (m_nBZDJK[1].JBDH == UP)
        inArrow->SetState(STATE_RED);
    else if (m_nBZDJK[1].JBDU == UP)
        inArrow->SetState(STATE_YELLOW);
    else if (m_nBZDJK[1].JBDL == UP)
        inArrow->SetState(STATE_GREEN);
    else
        inArrow->SetState(STATE_GRAY);
}
void CBS::CountNUM(QPainter * painter, QPoint p, int num)
{
    QString ss;
    ss = QString("%1").arg(num, 3, 10, QChar('0'));
    painter->drawText(p.x(), p.y() + 9, ss);
    painter->setBrush(QBrush(Qt::NoBrush));
    painter->drawRect(QRect(p.x() - 3, p.y() - 3, 30, 18));
}

void CBS::DealwithClicked(int index)
{
    if (index > 0 && index < 8)
    {
        SetState(index);
    }
    else
    {
        if (8 == index) //接车
        {
            m_bBtnJZDown = TRUE;
            SetState(4);
            SetState(5);
            SetState(6);
            QMessageBox::information(nullptr, "提示", "邻站列车进站"); //AfxMessageBox(_T("邻站列车进站"));
            SetState(4);
            SetState(5);
            SetState(6);
        }
        else if (9 == index) //发车
        {
            m_bBtnCZDown = TRUE;
            SetState(5);
            SetState(6);
            QMessageBox::information(nullptr, "提示", "邻站列车出站"); //AfxMessageBox(_T("邻站列车出站"));
            SetState(5);
            SetState(6);
            /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
            if (isLZFCJLFlag == TRUE)
            {
                isLZFCJLFlag = FALSE;   //邻站列车出站则伴随发车进路解锁
            }
            /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
        }
        /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
        else if (10 == index)
        {
            QMessageBox::information(nullptr, "提示", "邻站发车进路模拟"); //::AfxMessageBox(_T("邻站发车进路模拟"));
            isLZFCJLFlag = !(isLZFCJLFlag);
            isNeedBSQQ = TRUE;
        }
        else if (11 == index)
        {
            isJZUseFlag = TRUE;
        }
        else if (12 == index)
        {
            isJZUseFlag = FALSE;
        }
        else if (13 == index)
        {
            JZCount = 0;
        }
    }
}
/*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
void CBS::JZLogicProc()
{
    if (isHaveJZ && (TRUE == isJZUseFlag) && !JZFaultFlag)
    {
        if ((m_nBZDJK[1].FBDH == DOWN) && (m_nBZDJK[1].FBDL == DOWN) && (m_nBZDJK[1].FBDU == DOWN)
                && (m_nBZDJK[1].JBDH == DOWN) && (m_nBZDJK[1].JBDL == DOWN) && (m_nBZDJK[1].JBDU == DOWN)
                && (isLZFCJLFlag == TRUE) && (isNeedBSQQ == TRUE)) //发车进路触发计轴手续自动办理
        {
            SetState(1);
            isNeedBSQQ = FALSE; //生效一次后即刻将其落下
        }
        else if ((isLZFCJLFlag == TRUE) && (isNeedBSQQ == TRUE))
        {
            isNeedBSQQ = FALSE; //不满足触发条件时即重置该标志量的值
        }
        else if ((m_nBZDJK[1].JBDU == UP) && (m_nBZDJK[1].JBDH == DOWN) && (m_nBZDJK[1].JBDL == DOWN)
                 && (m_nBZDJK[1].FBDU == DOWN) && (m_nBZDJK[1].FBDH == DOWN) && (m_nBZDJK[1].FBDL == DOWN) && (isNeedBSTY == FALSE))
        {
            //pBS->SetState(1);
            isNeedBSTY = TRUE;
            isNeedBSTYtime = 0;    //2020.4.20-BJT-修改为延时4个周期后自动同意
        }
        else if ((m_nBZDJK[1].FBDH == DOWN) && (m_nBZDJK[1].FBDL == DOWN) && (m_nBZDJK[1].FBDU == DOWN)
                 && (m_nBZDJK[1].JBDH == DOWN) && (m_nBZDJK[1].JBDL == DOWN) && (m_nBZDJK[1].JBDU == DOWN)
                 && (isLZFCJLFlag == FALSE) && (isNeedBSQQ == TRUE))
        {
            SetState(2);
            isNeedBSQQ = FALSE; //生效一次后即刻将其落下
        }
        else if ((isLZFCJLFlag == FALSE) && (isNeedBSQQ == TRUE))
        {
            isNeedBSQQ = FALSE; //不满足自动取消闭塞时即重置该标志量的值
        }
        else if ((m_nBZDJK[1].FBDH == UP) && (m_nBZDJK[1].FBDL == DOWN) && (m_nBZDJK[1].FBDU == DOWN)
                 && (m_nBZDJK[1].JBDH == UP) && (m_nBZDJK[1].JBDL == DOWN) && (m_nBZDJK[1].JBDU == DOWN))
        {
            if (JZDifferentFlag && JZCount == 0)
                SetState(2);
            else if (!JZDifferentFlag)
                SetState(2);
        }
        pRecover->setState(0);
        //pBS->pFault->setState(0);
    }
    //轴数处理
    if (m_nBZDJK[1].FSBJ == DOWN && m_nBZDJK[1].FBDH == UP && m_nBZDJK[1].JBDH == DOWN && m_nBZDJK[1].GDJ == DOWN)
        JZCount = 100;
    else if (m_nBZDJK[1].JBDH == UP && m_nBZDJK[1].FBDH == DOWN && !JZDifferentFlag)
        JZCount = 100;
    else if (m_nBZDJK[1].JBDH == UP && m_nBZDJK[1].FBDH == DOWN && JZDifferentFlag)
        JZCount = 90;
    if (!JZDifferentFlag && (m_nBZDJK[1].FBDH == DOWN) && (m_nBZDJK[1].FBDL == DOWN) && (m_nBZDJK[1].FBDU == DOWN)
            && (m_nBZDJK[1].JBDH == DOWN) && (m_nBZDJK[1].JBDL == DOWN) && (m_nBZDJK[1].JBDU == DOWN))
        JZCount = 0;
}
void CBS::JZTimerProc()
{

    if ((TRUE == isHaveJZ) && (TRUE == isJZUseFlag) && (!JZFaultFlag))
    {
        if (m_nBZDJK[1].JBDU == UP)
        {
            if (isNeedBSTYtime >= 10)
            {
                SetState(1);
                pBlock->setState(ALL);
            }
            else
            {
                isNeedBSTYtime ++;
            }
        }
        else
        {
            isNeedBSTYtime = 0;
            isNeedBSTY = FALSE;
            pBlock->setState(0);
        }
        pRecover->setState(0);
        if ((isLZFCJLFlag == FALSE) && ((m_nBZDJK[1].FBDL == UP) || (m_nBZDJK[1].FBDU == UP)))
        {
            SetState(2);   //取消发车进路出发时，对应取消请求闭塞状态
            isNeedBSQQ = FALSE;
        }
    }
    else
    {
        isNeedBSTYtime = 0;
        isNeedBSTY = FALSE;
    }
}
/***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
#endif
