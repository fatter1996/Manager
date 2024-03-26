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

    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
    isHaveJZ = FALSE;
    isJZUseFlag = FALSE;
    isLZFCJLFlag = FALSE;
    isNeedBSQQ = FALSE;
    isNeedBSTYtime = 0;
    isNeedBSTY = FALSE;
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/

    nBS_BtnTime = 0;
    nFY_BtnTime = 0;
    nSG_BtnTime = 0;
    //计轴故障添加
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

    /*if (m_strName.GetAt(0) == 'X')    //取消该规则，直接交用station.txt数据控制
        m_nSX = 0;
    else 	if (m_strName.GetAt(0) == 'S')
        m_nSX = 1;*/
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
    pBlock->m_nType = BLOCK_BUTTON;    //闭塞按钮
    pBlock->pCenter.setX(m_textRect.left() - 30);
    pBlock->pCenter.setY(m_textRect.bottom() + 8);
    pBlock->BUTTONInit();
    pRecover = new CBUTTON;
    pRecover->m_nType = RECOVER_BUTTON;  //复原按钮
    pRecover->pCenter.setX(m_textRect.left());
    pRecover->pCenter.setY(m_textRect.bottom() + 8);
    pRecover->BUTTONInit();
    pFault = new CBUTTON;
    pFault->m_nType = FAULT_BUTTON;    //事故按钮
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
    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
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
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
}

//重新初始化闭塞坐标
#if 0
void CBS::ReInitBSPoint()
{
#ifndef ZCZX
    if (!m_nSX)
    {
        inArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        inArrow->m_nType = ARROW_RIGHT ;
        outArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
        outArrow->m_nType = ARROW_LEFT;
    }
    else
    {
        outArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        outArrow->m_nType = ARROW_RIGHT;
        inArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
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
    if (pBlock) //闭塞按钮
    {
        pBlock->center.SetPoint(m_textRect.left - 30, m_textRect.bottom + 8);
        pBlock->BUTTONInit();
    }
    if (pRecover) //复原按钮
    {
        pRecover->center.SetPoint(m_textRect.left, m_textRect.bottom + 8);
        pRecover->BUTTONInit();
    }
    if (pFault) //事故按钮
    {
        pFault->center.SetPoint(m_textRect.left + 30, m_textRect.bottom + 8);
        pFault->BUTTONInit();
    }
#ifdef _BZD_INTERFACE
    m_JSBJPoint.SetPoint(m_textRect.left - 30, m_textRect.bottom + 30);
    m_FSBJPoint.SetPoint(m_textRect.left, m_textRect.bottom + 30);
    m_GJFPoint.SetPoint(m_textRect.left + 30, m_textRect.bottom + 30);
#endif
    //m_textFont.CreatePointFont(12*10*72/96,"宋体");
}

//重新初始化闭塞坐标
void CBS::ReInitBSPoint(int offset_x, int offset_y)
{
    CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
    CTCCView *pView = (CTCCView *)(pFrame->GetActiveView());

    //基准坐标转换
    m_textRect.SetRect(m_textRectConst.left * pView->nDiploid, m_textRectConst.top * pView->nDiploid,
                       m_textRectConst.right * pView->nDiploid, m_textRectConst.bottom * pView->nDiploid);
    m_textRect.OffsetRect(offset_x, offset_y);

#ifndef ZCZX
    if (!m_nSX)
    {
        inArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        inArrow->m_nType = ARROW_RIGHT ;
        outArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
        outArrow->m_nType = ARROW_LEFT;
    }
    else
    {
        outArrow->p.SetPoint(m_textRect.left + 20, m_textRect.top);
        outArrow->m_nType = ARROW_RIGHT;
        inArrow->p.SetPoint(m_textRect.left - 25, m_textRect.top);
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
    if (pBlock) //闭塞按钮
    {
        pBlock->center.SetPoint(m_textRect.left - 30, m_textRect.bottom + 8);
        pBlock->BUTTONInit();
    }
    if (pRecover) //复原按钮
    {
        pRecover->center.SetPoint(m_textRect.left, m_textRect.bottom + 8);
        pRecover->BUTTONInit();
    }
    if (pFault) //事故按钮
    {
        pFault->center.SetPoint(m_textRect.left + 30, m_textRect.bottom + 8);
        pFault->BUTTONInit();
    }
#ifdef _BZD_INTERFACE
    m_JSBJPoint.SetPoint(m_textRect.left - 30, m_textRect.bottom + 30);
    m_FSBJPoint.SetPoint(m_textRect.left, m_textRect.bottom + 30);
    m_GJFPoint.SetPoint(m_textRect.left + 30, m_textRect.bottom + 30);
#endif
    //m_textFont.CreatePointFont(12*10*72/96,"宋体");
    //20190121
    pointBtnJZ.SetPoint(m_textRect.left + (80 * pView->nDiploid), m_textRect.bottom);
    pointBtnCZ.SetPoint(m_textRect.left + (130 * pView->nDiploid), m_textRect.bottom);
    rectTxtLZ.SetRect(m_textRect.left + (80 * pView->nDiploid), m_textRect.top, m_textRect.left + (130 * pView->nDiploid), m_textRect.bottom);
    rectBtnJZ.SetRect(pointBtnJZ.x - (OffPix1 * pView->nDiploid), pointBtnJZ.y - (OffPix1 * pView->nDiploid), pointBtnJZ.x + (OffPix1 * pView->nDiploid), pointBtnJZ.y + (OffPix1 * pView->nDiploid));
    rectBtnCZ.SetRect(pointBtnCZ.x - (OffPix1 * pView->nDiploid), pointBtnCZ.y - (OffPix1 * pView->nDiploid), pointBtnCZ.x + (OffPix1 * pView->nDiploid), pointBtnCZ.y + (OffPix1 * pView->nDiploid));

    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
    pointBtnLZFCJL.SetPoint(pointBtnLZFCJLConst.x * pView->nDiploid, pointBtnLZFCJLConst.y * pView->nDiploid);
    pointBtnJZuse.SetPoint(pointBtnJZuseConst.x * pView->nDiploid, pointBtnJZuseConst.y * pView->nDiploid);
    pointBtnJZStopuse.SetPoint(pointBtnJZStopuseConst.x * pView->nDiploid, pointBtnJZStopuseConst.y * pView->nDiploid);
    pointBtnJZZero.SetPoint(pointBtnJZZeroConst.x * pView->nDiploid, pointBtnJZZeroConst.y * pView->nDiploid);
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
}
#endif
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

void CBS::DRAW(QPainter *painter, float nDiploid, int nElapsed)
{

    //显示名称
    QFont font;
    font.setFamily("宋体");
    font.setPointSize(15);//字号
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
#if 0
    CPen newPen;
    CPen *pOldPen;
    CBrush newBrush;
    CBrush *oldBrush;
    CFont *oldFont;
    CSize  txtSize;
    COLORREF BtnColor;//颜色

    newPen.CreatePen(PS_SOLID, 1, WHITE);
    pOldPen = pDC->SelectObject(&newPen);

    if (m_bBtnJZDown == TRUE)
    {
        BtnColor = YELLOW;
        m_bBtnJZDown = FALSE;
    }
    else
    {
        BtnColor = GRAY;
    }
    newBrush.CreateSolidBrush(BtnColor);
    oldBrush = pDC->SelectObject(&newBrush);
    pDC->Rectangle(rectBtnJZ);
    pDC->SelectObject(oldBrush);
    newBrush.DeleteObject();

    if (m_bBtnCZDown == TRUE)
    {
        BtnColor = YELLOW;
        m_bBtnCZDown = FALSE;
    }
    else
    {
        BtnColor = GRAY;
    }
    newBrush.CreateSolidBrush(BtnColor);
    oldBrush = pDC->SelectObject(&newBrush);
    pDC->Rectangle(rectBtnCZ);
    pDC->SelectObject(oldBrush);
    newBrush.DeleteObject();

    pDC->SelectObject(pOldPen);
    newPen.DeleteObject();

    oldFont = pDC->SelectObject(&m_textFont);
    pDC->SetBkColor(BLACK);
    pDC->SetTextColor(WHITE);

    CRect  rectTxtJZ, rectTxtCZ;
    oldFont = pDC->SelectObject(&m_textFont);
    txtSize = pDC->GetTextExtent(BZDMN_LCJZ); //获取文字的大小

    rectTxtJZ.SetRect(pointBtnJZ.x - txtSize.cx / 2, pointBtnJZ.y + OffPix1 + 1, pointBtnJZ.x + txtSize.cx / 2, pointBtnJZ.y + OffPix1 + 1 + txtSize.cy);
    rectTxtCZ.SetRect(pointBtnCZ.x - txtSize.cx / 2, pointBtnCZ.y + OffPix1 + 1, pointBtnCZ.x + txtSize.cx / 2, pointBtnCZ.y + OffPix1 + 1 + txtSize.cy);

    pDC->DrawText(BZDMN_TXTLZ, rectTxtLZ, DT_CENTER); //绘制按钮名称
    pDC->DrawText(BZDMN_LCJZ, rectTxtJZ, DT_CENTER); //绘制按钮名称
    pDC->DrawText(BZDMN_LCCZ, rectTxtCZ, DT_CENTER); //绘制按钮名称
    pDC->SelectObject(oldFont);
#endif
    //DRAW BTN -END

#ifdef _BZD_INTERFACE
    //bzdProc();

#endif


#ifdef _BZD_INTERFACE
//   CBitmap btImage;
    //CPoint tmpPoint[] = {m_JSBJPoint,m_FSBJPoint,m_GJFPoint};
    //CString tmpName[] = {"JSBJ", "FSBJ", "GJF"};
    //BYTE tmpStat[] = {m_nBZDJK[1].JSBJ, m_nBZDJK[1].FSBJ,m_nBZDJK[1].GJF};

    //for (int i = 0; i < 3; i++)
    //{
    //	CRect tmpRect(tmpPoint[i].x, tmpPoint[i].y + 5, tmpPoint[i].x + 16, tmpPoint[i].y + 20);
    //	if (tmpStat[i] == UP)
    //		btImage.LoadBitmap(IDB_BITMAP_GBD);
    //	else
    //		btImage.LoadBitmap(IDB_BITMAP_GBU);
    //	DrawBitmap(pDC, &btImage, tmpRect.TopLeft(), tmpRect.Width(), tmpRect.Height());
    //	CPoint tmpText(tmpPoint[i].x, tmpPoint[i].y - 8);
    //	pDC->TextOut(tmpText.x, tmpText.y, tmpName[i]);
    //}
#endif

    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
#if 0
    if (TRUE == isHaveJZ)
    {

        CBitmap m_MemuImage;
        CPoint pt;
        CString name_Bt = _T("");

        pt.SetPoint(pointBtnLZFCJL.x, pointBtnLZFCJL.y);
        if (TRUE == isLZFCJLFlag)
        {
            m_MemuImage.LoadBitmap(IDB_TKFZANDOWN);
        }
        else
        {
            m_MemuImage.LoadBitmap(IDB_TKFZAN);
        }
        CBase::DrawBitmap(pDC, &m_MemuImage, pt, 15, 15, pView->nDiploid);
        name_Bt = _T("发车进路触发");
        pt.SetPoint((pointBtnLZFCJL.x + 7 * pView->nDiploid) - pDC->GetTextExtent(name_Bt).cx / 2, pointBtnLZFCJL.y + 20 * pView->nDiploid); //通过计算得到场联模拟设备文字显示坐标，自适应居中
        pDC->TextOut(pt.x, pt.y, name_Bt);

        pt.SetPoint(pointBtnJZuse.x, pointBtnJZuse.y);
        if (TRUE == isJZUseFlag)
        {
            m_MemuImage.LoadBitmap(IDB_TKFZANDOWN);
        }
        else
        {
            m_MemuImage.LoadBitmap(IDB_TKFZAN);
        }
        CBase::DrawBitmap(pDC, &m_MemuImage, pt, 15, 15, pView->nDiploid);
        name_Bt = _T("计轴使用");
        pt.SetPoint((pointBtnJZuse.x + 7 * pView->nDiploid) - pDC->GetTextExtent(name_Bt).cx / 2, pointBtnJZuse.y + 20 * pView->nDiploid); //通过计算得到场联模拟设备文字显示坐标，自适应居中
        pDC->TextOut(pt.x, pt.y, name_Bt);

        pt.SetPoint(pointBtnJZStopuse.x, pointBtnJZStopuse.y);
        if (TRUE == isJZUseFlag)
        {
            m_MemuImage.LoadBitmap(IDB_TKFZAN);
        }
        else
        {
            m_MemuImage.LoadBitmap(IDB_TKFZANDOWN);
        }
        CBase::DrawBitmap(pDC, &m_MemuImage, pt, 15, 15, pView->nDiploid);
        name_Bt = _T("计轴停用");
        pt.SetPoint((pointBtnJZStopuse.x + 7 * pView->nDiploid) - pDC->GetTextExtent(name_Bt).cx / 2, pointBtnJZStopuse.y + 20 * pView->nDiploid); //通过计算得到场联模拟设备文字显示坐标，自适应居中
        pDC->TextOut(pt.x, pt.y, name_Bt);

        pt.SetPoint(pointBtnJZZero.x, pointBtnJZZero.y);
        m_MemuImage.LoadBitmap(IDB_TKFZAN);
        CBase::DrawBitmap(pDC, &m_MemuImage, pt, 15, 15, pView->nDiploid);
        name_Bt = _T("计轴复零");
        pt.SetPoint((pointBtnJZZero.x + 7 * pView->nDiploid) - pDC->GetTextExtent(name_Bt).cx / 2, pointBtnJZZero.y + 20 * pView->nDiploid); //通过计算得到场联模拟设备文字显示坐标，自适应居中
        pDC->TextOut(pt.x, pt.y, name_Bt);
        CountNUM(pDC, CPoint(pt.x + 15, pt.y + 20), JZCount);
    }
#endif
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/
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
#endif
        pBlock->setState(ALL);
    }
    else if (i == 3)
    {
#ifdef _BZD_INTERFACE
        m_nBZDJK[1].SGAJ = UP;
        isChange[3] = TRUE;
#endif
        pFault->setState(ALL);
    }
    else if (i == 2)
    {
#ifdef _BZD_INTERFACE
        m_nBZDJK[1].FUAJ = UP;
        isChange[2] = TRUE;
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
    if ((pBlock->moveCursor(p)) && (!((isHaveJZ == TRUE) && (isJZUseFlag == TRUE)))) //2020.2.25-BJT-教师机配合邻站计轴
        return 1;
    if ((pRecover->moveCursor(p)) && (!((isHaveJZ == TRUE) && (isJZUseFlag == TRUE)))) //2020.2.25-BJT-教师机配合邻站计轴
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

    /*****************************2020.2.25-BJT-教师机配合邻站计轴***********************************/
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
    /***************************end-2020.2.25-BJT-教师机配合邻站计轴*********************************/

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
        //::AfxMessageBox("000");
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }
    else if ((m_nBZDJK[i].JSBJ == DOWN) && (m_nBZDJK[i].FUAJ == UP) && (m_nBZDJK[i].XZJ == UP) &&
             (m_nBZDJK[i].ZKJ == UP) && (m_nBZDJK[i].BSJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        //::AfxMessageBox("111");
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }
    else if ((m_nBZDJK[i].SGAJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        //::AfxMessageBox("222");
        m_nBZDJK[i].FDJ = UP;
        m_nBZDJK[i].FDJtime = 0;
    }

    else if ((m_nBZDJK[i].JSBJ == DOWN) && (m_nBZDJK[i].FUAJ == UP) && ((m_nBZDJK[i].TJJ == DOWN) || ((m_nBZDJK[i].TJJ == UP) && (m_nBZDJK[i].XZJ == UP) && (m_nBZDJK[i].ZKJ == UP)))
             && (m_nBZDJK[i].HDJ == UP) && (m_nBZDJK[i].TCJ == UP) && (m_nBZDJK[i].GDJ == UP) && (m_nBZDJK[i].FXJ == DOWN) && (m_nBZDJK[i].ZXJ == DOWN))
    {
        //::AfxMessageBox("333");
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
    /*if(i==1)
    {
        if(m_nBZDJK[1].BSAJ == UP)
        {
            CString str;
            str.Format("\n%s   闭塞按钮按下   HDJ=%02x   KTJ=%02x  TJJ=%02x   BSJ=%02x   FXJ=%02x   ZXJ=%02x   ZKJ=%02x   ZDJtime=%d\n",
                m_strName,m_nBZDJK[1].HDJ,m_nBZDJK[1].KTJ,m_nBZDJK[1].TJJ,m_nBZDJK[1].BSJ,m_nBZDJK[1].FXJ,m_nBZDJK[1].ZXJ,m_nBZDJK[1].ZKJ,m_nBZDJK[1].ZDJtime);
            ::OutputDebugString(str);
        }
    }*/
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
void CBS::CountNUM(QPainter *painter, QPoint p, int num)
{
#if 0
    CFont *oldFont;
    CFont m_textFont;
    m_textFont.CreateFont(15, 0, 0, 0,/*FW_MEDIUM*/FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_STROKE_PRECIS,
                          CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("楷体_GB2312"));

    oldFont = pDC->SelectObject(&m_textFont);
    pDC->SetBkColor(BLACK);
    pDC->SetTextColor(WHITE);
    CString ss;
    ss.Format("%.3d", num);
    pDC->TextOut(p.x, p.y, ss);
    pDC->SelectObject(oldFont);
    m_textFont.DeleteObject();
    //RECT(pDC,YELLOW,p.x-3,p.y-3,30,18);
#endif
}
#endif
