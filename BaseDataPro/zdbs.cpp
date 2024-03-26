
#include "zdbs.h"

CZDBS::CZDBS()
{
    m_nGLXH = 0xffff;
    isNX = 0;
    ArrowPt = QPoint(0, 0);
    ArrowPtConst = QPoint(0, 0);
    m_nAnNum = 0;
    p_GFAN = QPoint(0, 0);
    p_GFANConst = QPoint(0, 0);
    p_ZFZAN = QPoint(0, 0);
    p_ZFZANConst = QPoint(0, 0);
    p_FCFZAN = QPoint(0, 0);
    p_FCFZANConst = QPoint(0, 0);
    p_JCFZAN = QPoint(0, 0);
    p_JCFZANConst = QPoint(0, 0);
    p_MN = QPoint(0, 0);
    p_MNConst = QPoint(0, 0);
    rect_GF = QRect(0, 0, 0, 0);
    rect_ZFZ = QRect(0, 0, 0, 0);
    rect_FCFZ = QRect(0, 0, 0, 0);
    rect_JCFZ = QRect(0, 0, 0, 0);
    isDown_GF = false;
    isDown_ZFZ = false;
    isDown_FCFZ = false;
    isDown_JCFZ = false;
    m_nDownTime_FCFZ = 0xFF;
    m_nDownTime_JCFZ = 0xFF;
    p_BSD_YXFC = QPoint(0, 0); //允许发车灯
    p_BSD_YXFCConst = QPoint(0, 0); //允许发车灯
    p_BSD_FZ = QPoint(0, 0); //辅助表示灯
    p_BSD_FZConst = QPoint(0, 0); //辅助表示灯
    p_BSD_SG = QPoint(0, 0); //闪光表示灯
    p_BSD_SGConst = QPoint(0, 0); //闪光表示灯
    p_BSD_QJ = QPoint(0, 0); //区间表示灯
    p_BSD_QJConst = QPoint(0, 0); //区间表示灯
    rect_FK = QRect(0, 0, 0, 0);
    rect_FKConst = QRect(0, 0, 0, 0);
    BSDState_YXFC = BSD_Black;
    BSDState_SG = BSD_Black;
    BSDState_FZ = BSD_Black;
    BSDState_QJ = BSD_Black;
    BSDState_QG = BSD_Black;
    m_nArrowStateJC = Arrow_Black;
    m_nArrowStateFC = Arrow_Black;
    GFAN_info.init("允许改方");
    ZFZAN_info.init("总辅助");
    FCFZAN_info.init("发车辅助");
    JCFZAN_info.init("接车辅助");
}
void CZDBS::Draw_ToolTip(QPainter *painter, double nDiploid)
{
    QFont font;
    QPoint pt;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    //绘制鼠标进入文字提示信息
    if ((true == getToolTipFlag()) && ("" != getToolTipStr()))
    {
        pt.setX(pCenter.x() + 5 * nDiploid);
        pt.setY(pCenter.y() - 10 * nDiploid);
        font.setFamily("宋体");
        font.setPointSize(9 * nDiploid);//字号
        font.setItalic(false);//斜体

        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getToolTipStr());

        painter->setPen(QPen(QColor(38, 38, 38), 1));
        painter->setBrush(QColor(252, 245, 221));
        painter->drawRect(pt.x(), pt.y(), rec.width() + 10 * nDiploid, rec.height() + 4 * nDiploid);

        painter->setFont(font);//设置字体
        painter->setPen(QColor(38, 38, 38));//设置画笔颜色
        //绘制文本
        painter->drawText(pt.x() + 5 * nDiploid, pt.y() + 12 * nDiploid, getToolTipStr());
    }
}
int CZDBS::moveCursor(QPoint p)
{
    if (rect_ZFZ.contains(p))
    {
        return 1;
    }
    else if (rect_FCFZ.contains(p))
    {
        return 2;
    }
    else if (rect_JCFZ.contains(p))
    {
        return 3;
    }
    else if (getAnNum() == 4)
    {
        if (rect_GF.contains(p))
        {
            return 4;
        }
    }
    return 0;
}
void CZDBS::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx)
{
    if (type == 0x55)
    {
        Draw_Th_kb(painter, nElapsed, nDiploid, offset, zczx);
    }
}
void CZDBS::Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, bool zczx)
{
    QFont font;
    ArrowPt.setX(ArrowPtConst.x() * nDiploid + offset.x()*nDiploid);
    ArrowPt.setY(ArrowPtConst.y() * nDiploid + offset.y()*nDiploid);

    rect_GF.setRect(p_GFANConst.x() * nDiploid + offset.x() * nDiploid, p_GFANConst.y() * nDiploid + offset.y() * nDiploid, 14 * nDiploid, 14 * nDiploid);
    rect_ZFZ.setRect(p_ZFZANConst.x() * nDiploid + offset.x() * nDiploid, p_ZFZANConst.y() * nDiploid + offset.y() * nDiploid, 14 * nDiploid, 14 * nDiploid);
    rect_FCFZ.setRect(p_FCFZANConst.x() * nDiploid + offset.x() * nDiploid, p_FCFZANConst.y() * nDiploid + offset.y() * nDiploid, 14 * nDiploid, 14 * nDiploid);
    rect_JCFZ.setRect(p_JCFZANConst.x() * nDiploid + offset.x() * nDiploid, p_JCFZANConst.y() * nDiploid + offset.y() * nDiploid, 14 * nDiploid, 14 * nDiploid);

    p_BSD_SG.setX(p_BSD_SGConst.x()  * nDiploid + offset.x()*nDiploid);
    p_BSD_SG.setY(p_BSD_SGConst.y()  * nDiploid + offset.y()*nDiploid);
    p_BSD_FZ.setX(p_BSD_FZConst.x()  * nDiploid + offset.x()*nDiploid);
    p_BSD_FZ.setY(p_BSD_FZConst.y()  * nDiploid + offset.y()*nDiploid);
    p_BSD_QJ.setX(p_BSD_QJConst.x()  * nDiploid + offset.x()*nDiploid);
    p_BSD_QJ.setY(p_BSD_QJConst.y()  * nDiploid + offset.y()*nDiploid);
    p_BSD_YXFC.setX(p_BSD_YXFCConst.x()  * nDiploid + offset.x()*nDiploid);
    p_BSD_YXFC.setY(p_BSD_YXFCConst.y()  * nDiploid + offset.y()*nDiploid);
    rect_FK.setRect(rect_FKConst.topLeft().x() * nDiploid + offset.x() * nDiploid, rect_FKConst.topLeft().y() * nDiploid + offset.y() * nDiploid, rect_FKConst.width()*nDiploid, rect_FKConst.height()*nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色

    BtnInfo AN_info[4] = {ZFZAN_info, FCFZAN_info, JCFZAN_info, GFAN_info};
    bool isDownAN[4] = {false, false, false, false};
    isDownAN[0] = getIsDown_ZFZ();
    isDownAN[1] = getIsDown_FCFZ();
    isDownAN[2] = getIsDown_JCFZ();
    isDownAN[3] = getIsDown_GF();
    QRect rectAN[4] = {rect_ZFZ, rect_FCFZ, rect_JCFZ, rect_GF};

    QString strNameBSD[4] = {"允许发车", "辅助", "闪光", "区间"};
    int LightState[4] = {BSD_Black, BSD_Black, BSD_Black, BSD_Black};
    LightState[0] = getBSDState_YXFC();
    LightState[1] = getBSDState_FZ();
    LightState[2] = getBSDState_SG();
    LightState[3] = getBSDState_QJ();
    QPoint bsdPt[4] = {p_BSD_YXFC, p_BSD_FZ, p_BSD_SG, p_BSD_QJ};


    //绘制按钮
    for (int i = 0; i < getAnNum(); i++)
    {
        if (i >= 4)
        {
            break;
        }
        if (AN_info[i].nAN_Type == 0)
        {
            if (isDownAN[i] == true)
            {
                draw_Pixmap(painter, ":/Pictures/IDB_AN_DOWN_Yellow.jpg", rectAN[i].topLeft().x(), rectAN[i].topLeft().y(), 14, 14, nDiploid);
            }
            else
            {
                draw_Pixmap(painter, ":/Pictures/IDB_AN_UP_White.jpg", rectAN[i].topLeft().x(), rectAN[i].topLeft().y(), 14, 14, nDiploid);
            }
        }
        else if (AN_info[i].nAN_Type == 1)
        {
            //后续实现扩展
        }
        font.setFamily("宋体");
        font.setPointSize(AN_info[i].nNameSize * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(AN_info[i].cNameColor);//设置画笔颜色
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(AN_info[i].strAN_Name);
        if (AN_info[i].nNamePos_Type == 0)
        {
            painter->drawText(rectAN[i].topLeft().x() + 6 * nDiploid - rec.width() / 2, rectAN[i].topLeft().y() + 30 * nDiploid, AN_info[i].strAN_Name);
        }
        else if (AN_info[i].nNamePos_Type == 1)
        {
            painter->drawText(rectAN[i].topLeft().x() + 6 * nDiploid - rec.width() / 2, rectAN[i].topLeft().y() - 5 * nDiploid, AN_info[i].strAN_Name);
        }
        else if (AN_info[i].nNamePos_Type == 2)
        {
            painter->drawText(rectAN[i].topLeft().x() - 4 * nDiploid - rec.width(), rectAN[i].topLeft().y() + 10 * nDiploid, AN_info[i].strAN_Name);
        }
        else if (AN_info[i].nNamePos_Type == 3)
        {
            painter->drawText(rectAN[i].topRight().x() + 4 * nDiploid, rectAN[i].topLeft().y() + 10 * nDiploid, AN_info[i].strAN_Name);
        }


        if ((i == 1) || (i == 2))
        {
            if (((i == 1) ? (getDownTime_FCFZ()) : (getDownTime_JCFZ())) != 0xFF)
            {
                font.setFamily("宋体");
                font.setPointSize(9 * nDiploid);//字号
                font.setItalic(false);//斜体
                painter->setFont(font);//设置字体
                painter->setPen(Qt::red);//设置画笔颜色
                QString strTime = QString("%1").arg((i == 1) ? (getDownTime_FCFZ()) : (getDownTime_JCFZ()));
                painter->drawText(rectAN[i].topRight().x() + 3 * nDiploid, rectAN[i].topRight().y() + 10 * nDiploid, strTime);
            }
        }
    }
    //绘制表示灯
    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色
    for (int i = 0; i < 4; i++)
    {
        if (LightState[i] == BSD_Red)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", bsdPt[i].x(), bsdPt[i].y(), 13, 13, nDiploid);
        }
        else if (LightState[i] == BSD_Green)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Green.jpg", bsdPt[i].x(), bsdPt[i].y(), 13, 13, nDiploid);
        }
        else if (LightState[i] == BSD_White)
        {
            draw_Pixmap(painter, ":/Pictures/LED_White.jpg", bsdPt[i].x(), bsdPt[i].y(), 13, 13, nDiploid);
        }
        else if (LightState[i] == BSD_Yellow)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Yellow.jpg", bsdPt[i].x(), bsdPt[i].y(), 13, 13, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", bsdPt[i].x(), bsdPt[i].y(), 13, 13, nDiploid);
        }
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(strNameBSD[i]);
        painter->drawText(bsdPt[i].x() + 5 * nDiploid - rec.width() / 2, bsdPt[i].y() + 30 * nDiploid, strNameBSD[i]);
    }
    //绘制箭头
    if (((getSX() == 0) && (zczx == true)) || ((getSX() == 1) && (zczx == false))) //接车箭头在左，发车箭头在右
    {
        if (getArrowStateJC() == Arrow_Black)
        {
            draw_Pixmap(painter, ":/Pictures/ARightGrayA.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Red)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftRed.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Green)
        {
            draw_Pixmap(painter, ":/Pictures/ARightGreen.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Yellow)
        {
            draw_Pixmap(painter, ":/Pictures/ARightYellow.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }

        if (getArrowStateFC() == Arrow_Black)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftGrayA.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Red)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftRed.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Green)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftGreen.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Yellow)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftYellow.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
    }
    else
    {
        if (getArrowStateFC() == Arrow_Black)
        {
            draw_Pixmap(painter, ":/Pictures/ARightGrayA.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Red)
        {
            draw_Pixmap(painter, ":/Pictures/ARightRed.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Green)
        {
            draw_Pixmap(painter, ":/Pictures/ARightGreen.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateFC() == Arrow_Yellow)
        {
            draw_Pixmap(painter, ":/Pictures/ARightYellow.jpg", ArrowPt.x() - 15, ArrowPt.y(), 20, 15, nDiploid);
        }

        if (getArrowStateJC() == Arrow_Black)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftGrayA.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Red)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftRed.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Green)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftGreen.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
        else if (getArrowStateJC() == Arrow_Yellow)
        {
            draw_Pixmap(painter, ":/Pictures/ALeftYellow.jpg", ArrowPt.x() + 15, ArrowPt.y(), 20, 15, nDiploid);
        }
    }
    //绘制方框
    painter->setPen(QPen(Qt::white, 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect_FK);

    QFontMetrics fm(font);
    QRect rec = fm.boundingRect(getName());

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(Qt::black);
    painter->drawRect(rect_FK.topLeft().x() + rect_FK.width() / 2 - rec.width() / 2 - 4 * nDiploid, rect_FK.topLeft().y() - 7 * nDiploid, rec.width() + 8 * nDiploid, rec.height() + 2 * nDiploid);

    painter->setPen(Qt::white);//设置画笔颜色
    painter->drawText(rect_FK.topLeft().x() + rect_FK.width() / 2 - rec.width() / 2, rect_FK.topLeft().y() + 5 * nDiploid, getName());
}
void CZDBS::setVollover(QPoint pt_Base)
{

}

void CZDBS::setZDBS_StateOfLS(int state1, int state2, int state3, int state4, int state5)
{
    if ((state1 & 0xC0) == 0x00)
    {
        setArrowStateFC(Arrow_Black);
    }
    else if ((state1 & 0xC0) == 0x40)
    {
        setArrowStateFC(Arrow_Yellow);
    }
    else if ((state1 & 0xC0) == 0x80)
    {
        setArrowStateFC(Arrow_Green);
    }
    else if ((state1 & 0xC0) == 0xC0)
    {
        setArrowStateFC(Arrow_Red);
    }

    if ((state1 & 0x30) == 0x00)
    {
        setArrowStateJC(Arrow_Black);
    }
    else if ((state1 & 0x30) == 0x10)
    {
        setArrowStateJC(Arrow_Yellow);
    }
    else if ((state1 & 0x30) == 0x20)
    {
        setArrowStateJC(Arrow_Green);
    }
    else if ((state1 & 0x30) == 0x30)
    {
        setArrowStateJC(Arrow_Red);
    }

    if ((state1 & 0x08) == 0x08)
    {
        setIsDown_GF(false);
    }
    else
    {
        setIsDown_GF(true);
    }
    if ((state1 & 0x04) == 0x04)
    {
        setIsDown_ZFZ(false);
    }
    else
    {
        setIsDown_ZFZ(true);
    }
    if ((state1 & 0x02) == 0x02)
    {
        setIsDown_FCFZ(false);
    }
    else
    {
        setIsDown_FCFZ(true);
    }
    if ((state1 & 0x01) == 0x01)
    {
        setIsDown_JCFZ(false);
    }
    else
    {
        setIsDown_JCFZ(true);
    }

    if ((state2 & 0xC0) == 0x80)
    {
        setBSDState_FZ(BSD_White);
    }
    else if ((state2 & 0xC0) == 0xC0)
    {
        setBSDState_FZ(BSD_WhiteFlash);
    }
    else
    {
        setBSDState_FZ(BSD_Black);
    }
    if ((state2 & 0x30) == 0x20)
    {
        setBSDState_QJ(BSD_Red);
    }
    else if ((state2 & 0x30) == 0x30)
    {
        setBSDState_QJ(BSD_RedFlash);
    }
    else
    {
        setBSDState_QJ(BSD_Black);
    }
    if ((state2 & 0x0C) == 0x08)
    {
        setBSDState_YXFC(BSD_Green);
    }
    else if ((state2 & 0x0C) == 0x0C)
    {
        setBSDState_YXFC(BSD_GreenFlash);
    }
    else
    {
        setBSDState_YXFC(BSD_Black);
    }
    if ((state2 & 0x03) == 0x02)
    {
        setBSDState_SG(BSD_Red);
    }
    else if ((state2 & 0x03) == 0x03)
    {
        setBSDState_SG(BSD_RedFlash);
    }
    else
    {
        setBSDState_SG(BSD_Black);
    }
    if ((state3 & 0xC0) == 0x80)
    {
        setBSDState_QG(BSD_Red);
    }
    else if ((state3 & 0xC0) == 0xC0)
    {
        setBSDState_QG(BSD_RedFlash);
    }
    else
    {
        setBSDState_QG(BSD_Black);
    }

    setDownTime_FCFZ(state4);
    setDownTime_JCFZ(state5);
}
void CZDBS::getZDBS_StateForCTC(int *state1, int *state2, int *state3, int *state4, int *state5)
{
    *state1 = getGLXH();
    *state2 = getGLXH() >> 8;
    if (getArrowStateJC() == Arrow_Yellow)
    {
        *state3 = 0x10;
    }
    else if (getArrowStateJC() == Arrow_Red)
    {
        *state3 = 0x20;
    }
    else if (getArrowStateFC() == Arrow_Green)
    {
        *state3 = 0x40;
    }
    else if (getArrowStateFC() == Arrow_Red)
    {
        *state3 = 0x80;
    }
    if (getIsDown_GF() == true)
    {
        *state3 |= 0x08;
    }
    if (getIsDown_JCFZ() == true)
    {
        *state3 |= 0x04;
    }
    if (getIsDown_FCFZ() == true)
    {
        *state3 |= 0x02;
    }
    if (getIsDown_ZFZ() == true)
    {
        *state3 |= 0x01;
    }

    if (getBSDState_FZ() == BSD_Black)
    {
        *state4 = 0x00;
    }
    else if (getBSDState_FZ() == BSD_Red)
    {
        *state4 = 0x10;
    }
    else if (getBSDState_FZ() == BSD_Green)
    {
        *state4 = 0x20;
    }
    else if (getBSDState_FZ() == BSD_White)
    {
        *state4 = 0x30;
    }
    else if (getBSDState_FZ() == BSD_Yellow)
    {
        *state4 = 0x40;
    }
    else if (getBSDState_FZ() == BSD_RedFlash)
    {
        *state4 = 0x50;
    }
    else if (getBSDState_FZ() == BSD_GreenFlash)
    {
        *state4 = 0x60;
    }
    else if (getBSDState_FZ() == BSD_WhiteFlash)
    {
        *state4 = 0x70;
    }
    else if (getBSDState_FZ() == BSD_YellowFlash)
    {
        *state4 = 0x80;
    }

    if (getBSDState_QJ() == BSD_Black)
    {
        *state4 |= 0x00;
    }
    else if (getBSDState_QJ() == BSD_Red)
    {
        *state4 |= 0x01;
    }
    else if (getBSDState_QJ() == BSD_Green)
    {
        *state4 |= 0x02;
    }
    else if (getBSDState_QJ() == BSD_White)
    {
        *state4 |= 0x03;
    }
    else if (getBSDState_QJ() == BSD_Yellow)
    {
        *state4 |= 0x04;
    }
    else if (getBSDState_QJ() == BSD_RedFlash)
    {
        *state4 |= 0x05;
    }
    else if (getBSDState_QJ() == BSD_GreenFlash)
    {
        *state4 |= 0x06;
    }
    else if (getBSDState_QJ() == BSD_WhiteFlash)
    {
        *state4 |= 0x07;
    }
    else if (getBSDState_QJ() == BSD_YellowFlash)
    {
        *state4 |= 0x08;
    }

    if (getBSDState_YXFC() == BSD_Black)
    {
        *state5 = 0x00;
    }
    else if (getBSDState_YXFC() == BSD_Red)
    {
        *state5 = 0x10;
    }
    else if (getBSDState_YXFC() == BSD_Green)
    {
        *state5 = 0x20;
    }
    else if (getBSDState_YXFC() == BSD_White)
    {
        *state5 = 0x30;
    }
    else if (getBSDState_YXFC() == BSD_Yellow)
    {
        *state5 = 0x40;
    }
    else if (getBSDState_YXFC() == BSD_RedFlash)
    {
        *state5 = 0x50;
    }
    else if (getBSDState_YXFC() == BSD_GreenFlash)
    {
        *state5 = 0x60;
    }
    else if (getBSDState_YXFC() == BSD_WhiteFlash)
    {
        *state5 = 0x70;
    }
    else if (getBSDState_YXFC() == BSD_YellowFlash)
    {
        *state5 = 0x80;
    }

    if (getBSDState_QG() == BSD_Black)
    {
        *state5 |= 0x00;
    }
    else if (getBSDState_QG() == BSD_Red)
    {
        *state5 |= 0x01;
    }
    else if (getBSDState_QG() == BSD_Green)
    {
        *state5 |= 0x02;
    }
    else if (getBSDState_QG() == BSD_White)
    {
        *state5 |= 0x03;
    }
    else if (getBSDState_QG() == BSD_Yellow)
    {
        *state5 |= 0x04;
    }
    else if (getBSDState_QG() == BSD_RedFlash)
    {
        *state5 |= 0x05;
    }
    else if (getBSDState_QG() == BSD_GreenFlash)
    {
        *state5 |= 0x06;
    }
    else if (getBSDState_QG() == BSD_WhiteFlash)
    {
        *state5 |= 0x07;
    }
    else if (getBSDState_QG() == BSD_YellowFlash)
    {
        *state5 |= 0x08;
    }
}
unsigned int CZDBS::getDevType()
{
    return Dev_ZDBS;
}

void CZDBS::setGLXH(int x)
{
    m_nGLXH = x;
}
int CZDBS::getGLXH()
{
    return m_nGLXH;
}
void CZDBS::addVectJJQD(int x)
{
    vect_JJQD.append(x);
}
void CZDBS::setVectJJQD(QVector<int> x)
{
    vect_JJQD = x;
}
QVector<int> CZDBS::getVectJJQD()
{
    return vect_JJQD;
}
void CZDBS::setIsNX(bool flag)
{
    isNX = flag;
}
bool CZDBS::getIsNX()
{
    return isNX;
}
void CZDBS::setArrowPt(QPoint pt)
{
    ArrowPt = pt;
    ArrowPtConst = pt;
}
QPoint CZDBS::getArrowPt()
{
    return ArrowPtConst;
}
void CZDBS::setAnNum(int x)
{
    m_nAnNum = x;
}
int CZDBS::getAnNum()
{
    return m_nAnNum;
}
void CZDBS::setGFANPt(QPoint pt)
{
    p_GFAN = pt;
    p_GFANConst = pt;
}
QPoint CZDBS::getGFANPt()
{
    return p_GFANConst;
}
void CZDBS::setZFZANPt(QPoint pt)
{
    p_ZFZAN = pt;
    p_ZFZANConst = pt;
}
QPoint CZDBS::getZFZANPt()
{
    return p_ZFZANConst;
}
void CZDBS::setFCFZANPt(QPoint pt)
{
    p_FCFZAN = pt;
    p_FCFZANConst = pt;
}
QPoint CZDBS::getFCFZANPt()
{
    return p_FCFZANConst;
}
void CZDBS::setJCFZANPt(QPoint pt)
{
    p_JCFZAN = pt;
    p_JCFZANConst = pt;
}
QPoint CZDBS::getJCFZANPt()
{
    return p_JCFZANConst;
}
void CZDBS::setMNPt(QPoint pt)
{
    p_MN = pt;
    p_MNConst = pt;
}
QPoint CZDBS::getMNPt()
{
    return p_MNConst;
}
void CZDBS::setIsDown_GF(bool flag)
{
    isDown_GF = flag;
}
bool CZDBS::getIsDown_GF()
{
    return isDown_GF;
}
void CZDBS::setIsDown_ZFZ(bool flag)
{
    isDown_ZFZ = flag;
}
bool CZDBS::getIsDown_ZFZ()
{
    return isDown_ZFZ;
}
void CZDBS::setIsDown_FCFZ(bool flag)
{
    isDown_FCFZ = flag;
}
bool CZDBS::getIsDown_FCFZ()
{
    return isDown_FCFZ;
}
void CZDBS::setIsDown_JCFZ(bool flag)
{
    isDown_JCFZ = flag;
}
bool CZDBS::getIsDown_JCFZ()
{
    return isDown_JCFZ;
}
void CZDBS::setDownTime_FCFZ(int x)
{
    m_nDownTime_FCFZ = x;
}
int CZDBS::getDownTime_FCFZ()
{
    return m_nDownTime_FCFZ;
}
void CZDBS::setDownTime_JCFZ(int x)
{
    m_nDownTime_JCFZ = x;
}
int CZDBS::getDownTime_JCFZ()
{
    return m_nDownTime_JCFZ;
}
void CZDBS::setBSD_YXFCPt(QPoint pt)
{
    p_BSD_YXFC = pt;
    p_BSD_YXFCConst = pt;
}
QPoint CZDBS::getBSD_YXFCPt()
{
    return p_BSD_YXFCConst;
}
void CZDBS::setBSD_SGPt(QPoint pt)
{
    p_BSD_SG = pt;
    p_BSD_SGConst = pt;
}
QPoint CZDBS::getBSD_SGPt()
{
    return p_BSD_SGConst;
}
void CZDBS::setBSD_FZPt(QPoint pt)
{
    p_BSD_FZ = pt;
    p_BSD_FZConst = pt;
}
QPoint CZDBS::getBSD_FZPt()
{
    return p_BSD_FZConst;
}
void CZDBS::setBSD_QJPt(QPoint pt)
{
    p_BSD_QJ = pt;
    p_BSD_QJConst = pt;
}
QPoint CZDBS::getBSD_QJPt()
{
    return p_BSD_QJConst;
}
void CZDBS::setRect_FK(QRect rect)
{
    rect_FK = rect;
    rect_FKConst = rect;
}
QRect CZDBS::getRect_FK()
{
    return rect_FKConst;
}
void CZDBS::setBSDState_YXFC(int state)
{
    BSDState_YXFC = state;
}
int CZDBS::getBSDState_YXFC()
{
    return BSDState_YXFC;
}
void CZDBS::setBSDState_SG(int state)
{
    BSDState_SG = state;
}
int CZDBS::getBSDState_SG()
{
    return BSDState_SG;
}
void CZDBS::setBSDState_FZ(int state)
{
    BSDState_FZ = state;
}
int CZDBS::getBSDState_FZ()
{
    return BSDState_FZ;
}
void CZDBS::setBSDState_QJ(int state)
{
    BSDState_QJ = state;
}
int CZDBS::getBSDState_QJ()
{
    return BSDState_QJ;
}
void CZDBS::setBSDState_QG(int state)
{
    BSDState_QG = state;
}
int CZDBS::getBSDState_QG()
{
    return BSDState_QG;
}
void CZDBS::setArrowStateJC(int x)
{
    m_nArrowStateJC = x;
}
int CZDBS::getArrowStateJC()
{
    return m_nArrowStateJC;
}
void CZDBS::setArrowStateFC(int x)
{
    m_nArrowStateFC = x;
}
int CZDBS::getArrowStateFC()
{
    return m_nArrowStateFC;
}
void CZDBS::setGFAN_info(QString name, int size, int postype, int antype, QColor color)
{
    if (name != "")
    {
        GFAN_info.strAN_Name = name;
    }
    GFAN_info.nNameSize = size;
    GFAN_info.nNamePos_Type = postype;
    GFAN_info.nAN_Type = antype;
    GFAN_info.cNameColor = color;
}
void CZDBS::setZFZAN_info(QString name, int size, int postype, int antype, QColor color)
{
    if (name != "")
    {
        ZFZAN_info.strAN_Name = name;
    }
    ZFZAN_info.nNameSize = size;
    ZFZAN_info.nNamePos_Type = postype;
    ZFZAN_info.nAN_Type = antype;
    ZFZAN_info.cNameColor = color;
}
void CZDBS::setFCFZAN_info(QString name, int size, int postype, int antype, QColor color)
{
    if (name != "")
    {
        FCFZAN_info.strAN_Name = name;
    }
    FCFZAN_info.nNameSize = size;
    FCFZAN_info.nNamePos_Type = postype;
    FCFZAN_info.nAN_Type = antype;
    FCFZAN_info.cNameColor = color;
}
void CZDBS::setJCFZAN_info(QString name, int size, int postype, int antype, QColor color)
{
    if (name != "")
    {
        JCFZAN_info.strAN_Name = name;
    }
    JCFZAN_info.nNameSize = size;
    JCFZAN_info.nNamePos_Type = postype;
    JCFZAN_info.nAN_Type = antype;
    JCFZAN_info.cNameColor = color;
}
