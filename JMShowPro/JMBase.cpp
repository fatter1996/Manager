#include "JMBase.h"

CJMBase::CJMBase(QObject *parent) : QObject(parent)
{
    tts = new QTextToSpeech(this);
    isHaveRBtnMenu = false;
    vect_LSDArray.clear();
    vect_LineArray.clear();
    vect_TextArray.clear();
}
void CJMBase::draw_Pixmap(QPainter *painter, QString picName, int x, int y, int width, int heigh, int nDiploid)
{
    QPixmap pix;
    pix.load(picName);
    painter->drawPixmap(x, y, width, heigh, pix);
}
bool CJMBase::isQFDlgOK(int funCode, QString passWork, int type)
{
    CQfDlg dlg;
    dlg.init(funCode, passWork, type);
    if (dlg.exec() == QDialog::Accepted)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void CJMBase::draw_LSD(QPainter *painter, long nElapsed, double nDiploid)
{
    QFont font;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < vect_LSDArray.size(); i++)
    {
        vect_LSDArray[i].pt = QPoint(vect_LSDArray[i].ptConst.x() * nDiploid, vect_LSDArray[i].ptConst.y() * nDiploid);
        //绘制表示灯
        if (vect_LSDArray[i].cBSD_Color == Qt::white)
        {
            draw_Pixmap(painter, ":/Pictures/LED_White.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        else if (vect_LSDArray[i].cBSD_Color == Qt::green)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Green.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        else if (vect_LSDArray[i].cBSD_Color == Qt::red)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        else if (vect_LSDArray[i].cBSD_Color == Qt::yellow)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Yellow.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        else if (vect_LSDArray[i].cBSD_Color == Qt::black)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", vect_LSDArray[i].pt.x(), vect_LSDArray[i].pt.y(), 11, 11, nDiploid);
        }
        //绘制名称
        if (vect_LSDArray[i].strName != "9999")
        {
            font.setFamily("宋体");
            font.setPointSize(vect_LSDArray[i].nNameSize * nDiploid);//字号
            font.setItalic(false);//斜体
            painter->setFont(font);//设置字体
            painter->setPen(vect_LSDArray[i].cName_Color);//设置画笔颜色
            QFontMetrics fm(font);
            QRect rec = fm.boundingRect(vect_LSDArray[i].strName);
            if (vect_LSDArray[i].nNameShowType == 0)
            {
                painter->drawText(vect_LSDArray[i].pt.x() + 5 * nDiploid - rec.width() / 2, vect_LSDArray[i].pt.y() + 30 * nDiploid, vect_LSDArray[i].strName);
            }
            else if (vect_LSDArray[i].nNameShowType == 1)
            {

            }
            else if (vect_LSDArray[i].nNameShowType == 2)
            {

            }
            else if (vect_LSDArray[i].nNameShowType == 3)
            {

            }
        }
    }
}
void CJMBase::draw_Line(QPainter *painter, long nElapsed, double nDiploid)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < vect_LineArray.size(); i++)
    {
        vect_LineArray[i].pt1 = QPoint(vect_LineArray[i].pt1Const.x() * nDiploid, vect_LineArray[i].pt1Const.y() * nDiploid);
        vect_LineArray[i].pt2 = QPoint(vect_LineArray[i].pt2Const.x() * nDiploid, vect_LineArray[i].pt2Const.y() * nDiploid);

        if (vect_LineArray[i].nLineType == 0)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else if (vect_LineArray[i].nLineType == 1)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::DashLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else if (vect_LineArray[i].nLineType == 2)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::DotLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else if (vect_LineArray[i].nLineType == 4)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::DashDotLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else if (vect_LineArray[i].nLineType == 5)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::DashDotDotLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else if (vect_LineArray[i].nLineType == 6)
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::CustomDashLine, Qt::SquareCap, Qt::BevelJoin));
        }
        else
        {
            painter->setPen(QPen(vect_LineArray[i].cLineColor, vect_LineArray[i].nLineSize * nDiploid, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
        }

        painter->drawLine(QPoint(vect_LineArray[i].pt1.x(), vect_LineArray[i].pt1.y()), QPoint(vect_LineArray[i].pt2.x(), vect_LineArray[i].pt2.y()));
    }
}
void CJMBase::draw_Text(QPainter *painter, long nElapsed, double nDiploid)
{
    QFont font;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < vect_TextArray.size(); i++)
    {
        vect_TextArray[i].pt = QPoint(vect_TextArray[i].ptConst.x() * nDiploid, vect_TextArray[i].ptConst.y() * nDiploid);
        font.setFamily(vect_TextArray[i].str_FontName);
        font.setPointSize(vect_TextArray[i].nFontSize * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(vect_TextArray[i].cFontColor);//设置画笔颜色
        painter->drawText(vect_TextArray[i].pt.x(), vect_TextArray[i].pt.y(), vect_TextArray[i].str_Name);
    }
}
void CJMBase::Draw(QPainter *painter, long nElapsed, double nDiploid)
{

}
void CJMBase::initData(QWidget *w)
{

}
void CJMBase::readInterfaceData()
{

}
int CJMBase::moveCursor(QPoint p)
{
    return 0;
}
bool CJMBase::OnLButtonDown(QPoint p)
{
    return false;
}
bool CJMBase::OnRButtonDown(QPoint p)
{
    return false;
}
void CJMBase::SetDLBStart(int id)
{

}
void CJMBase::DLBTimer()
{

}
void CJMBase::ANCmdOfCTC(int anFunType, int devCode1, int devCode2)
{

}
void CJMBase::initMenu()
{

}
void CJMBase::timePro(bool flag_JTGD, bool flag_DCWZ, bool flag_WCQDName, bool flag_DCQDName, bool flag_XHDName, bool flag_DCName)
{

}
void CJMBase::Draw_DLB(QPainter *painter, long nElapsed, double nDiploid)
{

}
void CJMBase::slot_FuzhuMenuDlg_AN(int index)
{

}
void CJMBase::slot_YHZSBState(unsigned char yhzs_State, int stationID)
{

}
void CJMBase::onClearAction()
{

}

void CJMBase::setSpeechText(QString str, int index)
{
    bool flag = false;
    for (int i = 0; i < vect_SpeechArr.size(); i++)
    {
        if (vect_SpeechArr[i]->str_Speech == str)
        {
            if (vect_SpeechArr[i]->n_Count < index)
            {
                vect_SpeechArr[i]->n_Count = index;
            }
            flag = true;
            break;
        }
    }
    if (flag == false)
    {
        SpeechData *pSpeech = new SpeechData();
        pSpeech->str_Speech = str;
        pSpeech->n_Count = index;
        vect_SpeechArr.append(pSpeech);
    }
}
void CJMBase::speechTextPro()
{
    for (int i = 0; i < vect_SpeechArr.size(); i++)
    {
        if (tts->state() == QTextToSpeech::Ready)
        {
            tts->say(vect_SpeechArr[i]->str_Speech);
            vect_SpeechArr[i]->n_Count--;
            if (vect_SpeechArr[i]->n_Count == 0)
            {
                vect_SpeechArr.erase(vect_SpeechArr.begin() + i);
            }
        }
    }
}
