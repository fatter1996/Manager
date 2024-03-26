#include "gd.h"

CGD::CGD()
{
    p1.setX(1);
    p1.setY(1);
    p2.setX(1);
    p2.setY(1);
    p3.setX(1);
    p3.setY(1);
    p4.setX(1);
    p4.setY(1);
    p12.setX(1);
    p12.setY(1);
    p34.setX(1);
    p34.setY(1);
    pz12.setX(1);
    pz12.setY(1);
    pz34.setX(1);
    pz34.setY(1);
    m_nGDType = QD;            //轨道区段类型
    m_nZ = 0;                   //折点个数
//    isLostTrainNum = false;     //是否列车丢失--暂时跟着车次走，先预留
//    isLock = false;             //是否封锁
    isGDFLBL = false;           //是否分录不良
    //GLB_QDleft=0;
    //GLB_QDright=0;
    //isSXDC = false;
    isZXGD = false;
    isCXGD = false;
    isJJGD = false;
    //GD_LCode= 0xffff;
    //GD_RCode= 0xffff;
    //CarrierFrequency="2300-2";
    //Dir_DMH=LEFT;
    //isNoShow=false;
    m_strMark1 = "";
    m_strMark1 = "";

    isHaveGDTW = false;   //是否有股道停稳
    pt_GDTWBSD = QPoint(-1, -1); //股道停稳表示灯坐标
    pt_GDTWName = QPoint(-1, -1); //股道停稳表示灯名称坐标
    pt_GDTWBSDConst = QPoint(-1, -1); //股道停稳表示灯坐标
    pt_GDTWNameConst = QPoint(-1, -1); //股道停稳表示灯名称坐标
    m_nGDTWTime = 0xFF;   //股道停稳倒计时，0xff表示当前无倒计时
    m_strGDTWName = "";
    vect_ZTArray.clear();
}

//成员功能函数实现
void CGD::GDInit(int type)
{
    p1Const.setX(p1.x());
    p1Const.setY(p1.y());
    p2Const.setX(p2.x());
    p2Const.setY(p2.y());
    p3Const.setX(p3.x());
    p3Const.setY(p3.y());
    p4Const.setX(p4.x());
    p4Const.setY(p4.y());
    p12Const.setX(p12.x());
    p12Const.setY(p12.y());
    p34Const.setX(p34.x());
    p34Const.setY(p34.y());
    pz12Const.setX(pz12.x());
    pz12Const.setY(pz12.y());
    pz34Const.setX(pz34.x());
    pz34Const.setY(pz34.y());
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
    m_textRectConst.setLeft(m_textRect.left());
    m_textRectConst.setTop(m_textRect.top());
    m_textRectConst.setRight(m_textRect.right());
    m_textRectConst.setBottom(m_textRect.bottom());
}
void CGD::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx)
{
    if (type == 0x55)
    {
        Draw_Th_kb(painter, nElapsed, nDiploid, offset);
    }
    Draw_ZT(painter, nElapsed, nDiploid, offset);
}
void CGD::Draw_Th_kb(QPainter *painter, long nElapsed, double nDiploid, QPoint offset)
{
    QFont font;
    //坐标变换 2021.1.11 BJT
//    if(getisNoShow()==true) //根据是否不显示标识进行绘制 ---主要调试用
//    {
//#if 1
//        offset.setX(offset.x());
//        offset.setY(offset.y()+250);
//#else
//        return;
//#endif
//    }
    p1.setX(p1Const.x() * nDiploid + offset.x()*nDiploid);
    p1.setY(p1Const.y() * nDiploid + offset.y()*nDiploid);
    p2.setX(p2Const.x() * nDiploid + offset.x()*nDiploid);
    p2.setY(p2Const.y() * nDiploid + offset.y()*nDiploid);
    p3.setX(p3Const.x() * nDiploid + offset.x()*nDiploid);
    p3.setY(p3Const.y() * nDiploid + offset.y()*nDiploid);
    p4.setX(p4Const.x() * nDiploid + offset.x()*nDiploid);
    p4.setY(p4Const.y() * nDiploid + offset.y()*nDiploid);
    p12.setX(p12Const.x() * nDiploid + offset.x()*nDiploid);
    p12.setY(p12Const.y() * nDiploid + offset.y()*nDiploid);
    p34.setX(p34Const.x() * nDiploid + offset.x()*nDiploid);
    p34.setY(p34Const.y() * nDiploid + offset.y()*nDiploid);
    pz12.setX(pz12Const.x() * nDiploid + offset.x()*nDiploid);
    pz12.setY(pz12Const.y() * nDiploid + offset.y()*nDiploid);
    pz34.setX(pz34Const.x() * nDiploid + offset.x()*nDiploid);
    pz34.setY(pz34Const.y() * nDiploid + offset.y()*nDiploid);
    pCenter.setX(pCenterConst.x() * nDiploid + offset.x()*nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid + offset.y()*nDiploid);
    m_textRect.setLeft(m_textRectConst.left() * nDiploid + offset.x()*nDiploid);
    m_textRect.setTop(m_textRectConst.top() * nDiploid + offset.y()*nDiploid);
    m_textRect.setRight(m_textRectConst.right() * nDiploid + offset.x()*nDiploid);
    m_textRect.setBottom(m_textRectConst.bottom() * nDiploid + offset.y()*nDiploid);
    pt_GDTWBSD.setX(pt_GDTWBSDConst.x() * nDiploid + offset.x() * nDiploid);
    pt_GDTWBSD.setY(pt_GDTWBSDConst.y() * nDiploid + offset.y() * nDiploid);
    pt_GDTWName.setX(pt_GDTWNameConst.x() * nDiploid + offset.x() * nDiploid);
    pt_GDTWName.setY(pt_GDTWNameConst.y() * nDiploid + offset.y() * nDiploid);
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //绘制标记
    if ((getMarkStr1() != "") || (getMarkStr2() != ""))
    {
        QString markstr = getMarkStr1() + getMarkStr2();
        QBrush brush(Qt::yellow); //画刷
        painter->setBrush(brush); //添加画刷
        font.setFamily("宋体");
        font.setPointSize(10 * nDiploid);//字号
        QFontMetrics fm(font);
        QRect markRect = fm.boundingRect(markstr);
        painter->drawRect(pCenter.x() - (markRect.width() / 2) - 1 * nDiploid, pCenter.y() - markRect.height(), markRect.width() + 2 * nDiploid, markRect.height()); //绘制矩形
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::blue);//设置画笔颜色
        //绘制文本
        painter->drawText(pCenter.x() - (markRect.width() / 2), pCenter.y() - (markRect.height() / 2) + 2 * nDiploid, markstr);

    }
    //轨道区段光带绘制 2021.1.11 BJT
//    if(true==getGDFLBL())   //分路不良绘制虚线
//    {
//        painter->setPen(QPen(getQDColor(),GDWIDTH * nDiploid,Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
//    }
//    else
//    {
//        painter->setPen(QPen(getQDColor(),GDWIDTH * nDiploid,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    }
    painter->setPen(QPen(getQDColor(), GDWIDTH * nDiploid, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (0 == getZ())
    {
        painter->drawLine(p12, p34);//绘制轨道电路光带
    }
    else if (1 == getZ())
    {
        painter->drawLine(p12, pCenter);//绘制轨道电路光带
        painter->drawLine(pCenter, p34);//绘制轨道电路光带
    }
    else
    {
        painter->drawLine(p12, pz12);//绘制轨道电路光带
        painter->drawLine(pz12, pz34);//绘制轨道电路光带
        painter->drawLine(pz34, p34);//绘制轨道电路光带
    }
    //分路不良绘制粉红色外包线
    if (true == getGDFLBL())
    {
        if (0 == getZ())
        {
            Draw_FLBL_WBX(painter, p12, p34, nDiploid);
        }
        else if (1 == getZ())
        {
            Draw_FLBL_WBX(painter, p12, pCenter, nDiploid);
            Draw_FLBL_WBX(painter, pCenter, p34, nDiploid);
        }
        else
        {
            Draw_FLBL_WBX(painter, p12, pz12, nDiploid);
            Draw_FLBL_WBX(painter, pz12, pz34, nDiploid);
            Draw_FLBL_WBX(painter, pz34, p34, nDiploid);
        }
    }
    //绘制股道停稳表示灯
    if (getIsHaveGDTW() == true)
    {
        if ((getGDTWTime() != 0xFF) && (nElapsed % 2 == 0)) //正在倒计时
        {
            draw_Pixmap(painter, ":/Pictures/LED_White.jpg", getPt_GDTWBSD().x(), getPt_GDTWBSD().y(), 13, 13, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", getPt_GDTWBSD().x(), getPt_GDTWBSD().y(), 13, 13, nDiploid);
        }
        if ((getGDTWName() != "") && (getPt_GDTWName().x() >= 0) && (getPt_GDTWName().y() >= 0))
        {
            font.setFamily("宋体");
            font.setPointSize(10 * nDiploid);//字号
            font.setItalic(false);//斜体
            painter->setFont(font);//设置字体
            painter->setPen(Qt::white);//设置画笔颜色
            //绘制文本
            painter->drawText(getPt_GDTWName().x(), getPt_GDTWName().y(), getGDTWName());
        }
        if (getGDTWTime() != 0xFF)
        {
            font.setFamily("宋体");
            font.setPointSize(9 * nDiploid);//字号
            font.setItalic(false);//斜体
            painter->setFont(font);//设置字体
            painter->setPen(Qt::red);//设置画笔颜色
            //绘制文本
            if ((getGDTWName() != "") && (getPt_GDTWName().x() >= 0) && (getPt_GDTWName().y() >= 0)) //有名称时倒计时位于名称右边
            {
                QFontMetrics fm(font);
                QRect rec = fm.boundingRect(getGDTWName());
                painter->drawText(getPt_GDTWName().x() + rec.width() + 2 * nDiploid, getPt_GDTWName().y(), QString("%1").arg(getGDTWTime()));
            }
            else   //没有名称时
            {
                QFontMetrics fm(font);
                QRect rec = fm.boundingRect(QString("%1").arg(getGDTWTime()));
                //倒计时在灯的上方
                //painter->drawText(getPt_GDTWBSD().x() - rec.width()/2 + 7*nDiploid, getPt_GDTWBSD().y()-rec.height() -2*nDiploid, QString("%1").arg(getGDTWTime()));
                //倒计时在灯的右边
                painter->drawText(getPt_GDTWBSD().x() + 15 * nDiploid, getPt_GDTWBSD().y() + 11 * nDiploid, QString("%1").arg(getGDTWTime()));
            }
        }
    }

    //绘制绝缘节 2021.1.11 BJT
    painter->setPen(QPen(SkyBlue, 1));
    painter->drawLine(p1, p2); //绘制轨道区段左侧绝缘节
    painter->drawLine(p3, p4); //绘制轨道区段右侧绝缘节

    //绘制轨道区段名称 2021.1.11 BJT
    if (getDisplayName())
    {
        font.setFamily("宋体");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.bottom() - 10 * nDiploid, getName());
    }
    //无岔区段绘制载频
//    if(true == getIsDisDPZPFlag())
//    {
//        font.setFamily("宋体");
//        font.setPointSize(8 * nDiploid);//字号
//        font.setItalic(false);//斜体
//        painter->setFont(font);//设置字体
//        QFontMetrics fm(font);
//        QRect rec = fm.boundingRect(getCarrierFrequency());
//        painter->setPen(QPen(Qt::white, 1));
//        painter->setBrush(Qt::black);
//        painter->drawRect(p12.x()+rec.width()/2+10* nDiploid, p12.y()-rec.height()-12* nDiploid, rec.width()+4* nDiploid, rec.height());
//        painter->drawText(p12.x()+rec.width()/2+11* nDiploid, p12.y()-rec.height()-2* nDiploid, getCarrierFrequency()); //绘制文本
//    }
    DrawTrain(painter, nElapsed, nDiploid, offset);
}
void CGD::Draw_FLBL_WBX(QPainter *painter, QPointF pt1, QPointF pt2, double nDiploid)
{
    painter->setPen(QPen(Qt::magenta, 1 * nDiploid, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (pt1.y() == pt2.y())
    {
        painter->drawLine(QPoint(pt1.x(), pt1.y() - 2 * nDiploid), QPoint(pt2.x(), pt2.y() - 2 * nDiploid)); //绘制分路不良外包线
        painter->drawLine(QPoint(pt1.x(), pt1.y() + 2 * nDiploid), QPoint(pt2.x(), pt2.y() + 2 * nDiploid)); //绘制分路不良外包线
    }
    else
    {
        painter->drawLine(QPoint(pt1.x(), pt1.y() - 4 * nDiploid), QPoint(pt2.x(), pt2.y() - 4 * nDiploid)); //绘制分路不良外包线
        painter->drawLine(QPoint(pt1.x(), pt1.y() + 4 * nDiploid), QPoint(pt2.x(), pt2.y() + 4 * nDiploid)); //绘制分路不良外包线
    }
}
void CGD::Draw_ToolTip(QPainter *painter, double nDiploid)
{
//    if(getisNoShow()==true)
//    {
//        return;
//    }
    QFont font;
    QPoint pt;
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    //绘制鼠标进入文字提示信息
    if ((true == getToolTipFlag()) && ("" != getToolTipStr()))
    {
        pt.setX(m_textRect.left() + 5 * nDiploid);
        pt.setY(m_textRect.top() - 10 * nDiploid);
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
void CGD::Draw_ZT(QPainter *painter, long nElapsed, double nDiploid, QPoint offset)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (auto ement : vect_ZTArray)
    {
        ement.pt_Center.setX(ement.pt_CenterConst.x() * nDiploid);
        ement.pt_Center.setY(ement.pt_CenterConst.y() * nDiploid);
        painter->setPen(QPen(ement.cLineColor, ement.nLineSize));
        //绘制长横线 2022.11.30 BJT
        painter->drawLine(QPoint(ement.pt_Center.x() - ement.nWidth / 2, ement.pt_Center.y()), QPoint(ement.pt_Center.x() + ement.nWidth / 2, ement.pt_Center.y()));
        //绘制短竖线  2022.11.30-BJT
        if (ement.isUP == true)
        {
            painter->drawLine(QPoint(ement.pt_Center.x() - ement.nWidth / 2, ement.pt_Center.y()), QPoint(ement.pt_Center.x() - ement.nWidth / 2, ement.pt_Center.y() - ement.nHeigh));
            painter->drawLine(QPoint(ement.pt_Center.x() + ement.nWidth / 2, ement.pt_Center.y()), QPoint(ement.pt_Center.x() + ement.nWidth / 2, ement.pt_Center.y() - ement.nHeigh));
        }
        else
        {
            painter->drawLine(QPoint(ement.pt_Center.x() - ement.nWidth / 2, ement.pt_Center.y()), QPoint(ement.pt_Center.x() - ement.nWidth / 2, ement.pt_Center.y() + ement.nHeigh));
            painter->drawLine(QPoint(ement.pt_Center.x() + ement.nWidth / 2, ement.pt_Center.y()), QPoint(ement.pt_Center.x() + ement.nWidth / 2, ement.pt_Center.y() + ement.nHeigh));
        }
    }
}
void CGD::setDevStateToSafe()
{
    gdColor = Qt::red;        //轨道区段颜色
}

void CGD::DrawTrain(QPainter *painter, long nElapsed, double nDiploid, QPoint offset)
{
    QFont font;
    if (pTrain)
    {
        QRect m_rectTrain, m_rectTrainTxt;
        m_rectTrain.setRect(pCenter.x() - 28, pCenter.y() - 10, pTrain->TrainNumber.length() * 13 * nDiploid, 22);
        m_rectTrainTxt.setRect(m_rectTrain.left() + 5, m_rectTrain.top(), pTrain->TrainNumber.length() * 13 * nDiploid - 10, 22);
        if (pTrain->Train_Type == "列车")
            painter->setBrush(QBrush(Qt::green)); //添加画刷
        else  if (pTrain->Train_Type == "调车")
            painter->setBrush(QBrush(Qt::white)); //添加画刷
        painter->setPen(Qt::white);//设置画笔颜色
        painter->drawRect(m_rectTrain);
        if (pTrain->Train_SX == "右行")
        {
            QPointF points[3] =
            {
                QPointF(m_rectTrain.right(), m_rectTrain.top()),
                QPointF(m_rectTrain.right(), m_rectTrain.bottom() + 2 * nDiploid),
                QPointF(m_rectTrain.right() + 15 * nDiploid, m_rectTrain.top() + 10 * nDiploid)
            };
            painter->setBrush(QBrush(Qt::white)); //添加画刷
            painter->drawPolygon(points, 3);
        }
        else if (pTrain->Train_SX == "左行")
        {
            QPointF points[3] =
            {
                QPointF(m_rectTrain.left(), m_rectTrain.top()),
                QPointF(m_rectTrain.left(), m_rectTrain.bottom()),
                QPointF(m_rectTrain.left() - 15 * nDiploid, m_rectTrain.top() + 10 * nDiploid)
            };
            painter->setBrush(QBrush(Qt::white)); //添加画刷
            painter->drawPolygon(points, 3);
        }

        font.setFamily("宋体");
        font.setPointSize(15 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::red);//设置画笔颜色
        painter->drawText(m_rectTrainTxt, Qt::AlignCenter, pTrain->TrainNumber);
    }
}
unsigned int CGD::getDevType()
{
    return Dev_GD;
}
int CGD::moveCursor(QPoint p)
{
    QPoint pt;
//    if(getisNoShow()==true)
//    {
//        return 0;
//    }
    pt.setX(p.x());
    pt.setY(p.y() - Off_Global);
    if (m_textRect.contains(pt))
    {
        return 1;
    }
    return 0;
}
void CGD::setVollover(QPoint pt_Base)
{
    double x1 = 0;
    double x2 = 0;
    double y1 = 0;
    double y2 = 0;
    double y3 = 0;
    setTextRect(QRectF(pt_Base.x() - (getTextRect().right() - pt_Base.x()), pt_Base.y() - (getTextRect().bottom() - pt_Base.y())/*-20*/, getTextRect().width(), getTextRect().height()));
    x1 = getp1().x();
    x2 = getp2().x();
    y1 = getp1().y();
    y2 = getp2().y();
    y3 = getp12().y();
    setp1(QPointF(pt_Base.x() - (x2 - pt_Base.x()), pt_Base.y() - (y3 - pt_Base.y()) - (((y2 - y1) + 1) / 2)));
    setp2(QPointF(pt_Base.x() - (x1 - pt_Base.x()), pt_Base.y() - (y3 - pt_Base.y()) + (((y2 - y1) + 1) / 2)));
    x1 = getp3().x();
    x2 = getp4().x();
    y1 = getp3().y();
    y2 = getp4().y();
    y3 = getp34().y();
    setp3(QPointF(pt_Base.x() - (x2 - pt_Base.x()), pt_Base.y() - (y3 - pt_Base.y()) - (((y2 - y1) + 1) / 2)));
    setp4(QPointF(pt_Base.x() - (x1 - pt_Base.x()), pt_Base.y() - (y3 - pt_Base.y()) + (((y2 - y1) + 1) / 2)));

    setp12(QPointF(pt_Base.x() - (getp12().x() - pt_Base.x()), pt_Base.y() - (getp12().y() - pt_Base.y())));
    setp34(QPointF(pt_Base.x() - (getp34().x() - pt_Base.x()), pt_Base.y() - (getp34().y() - pt_Base.y())));
    setpz12(QPointF(pt_Base.x() - (getpz12().x() - pt_Base.x()), pt_Base.y() - (getpz12().y() - pt_Base.y())));
    setpz34(QPointF(pt_Base.x() - (getpz34().x() - pt_Base.x()), pt_Base.y() - (getpz34().y() - pt_Base.y())));
    setCenterPt(QPointF(pt_Base.x() - (getCenterPt().x() - pt_Base.x()), pt_Base.y() - (getCenterPt().y() - pt_Base.y())));
}
//void CGD::gd_StatePro()
//{
//    if((true == getIsQDZY()) || true == getIsQDGZ())
//    {
//        setQDColor("RED");
//    }
//    else
//    {
//        setQDColor("BLUE");
//    }
//}
//成员变量封装函数实现
void CGD::setGDType(QString str_type)
{
    if (str_type == "GD_QD")
    {
        m_nGDType = GD_QD;
    }
    else if (str_type == "JJ_QD")
    {
        m_nGDType = JJ_QD;
    }
    else if (str_type == "QD")
    {
        m_nGDType = QD;
    }
}
unsigned int CGD::getGDType()
{
    return m_nGDType;
}
void CGD::setZ(unsigned int nZ)
{
    m_nZ = nZ;
}
unsigned int CGD::getZ()
{
    return m_nZ;
}
//void CGD::setLostTraNum(bool nFlag)
//{
//    isLostTrainNum = nFlag;
//}
//bool CGD::getLostTraNum()
//{
//    return isLostTrainNum;
//}
//void CGD::setLock(bool nFlag)
//{
//    isLock = nFlag;
//}
//bool CGD::getLock()
//{
//    return isLock;
//}
void CGD::setGDFLBL(bool nFlag)
{
    isGDFLBL = nFlag;
}
bool CGD::getGDFLBL()
{
    return isGDFLBL;
}
void CGD::setp1(QPointF pt)
{
    p1 = pt;
    p1Const = pt;
}
QPointF CGD::getp1()
{
    return p1Const;
}
void CGD::setp2(QPointF pt)
{
    p2 = pt;
    p2Const = pt;
}
QPointF CGD::getp2()
{
    return p2Const;
}
void CGD::setp3(QPointF pt)
{
    p3 = pt;
    p3Const = pt;
}
QPointF CGD::getp3()
{
    return p3Const;
}
void CGD::setp4(QPointF pt)
{
    p4 = pt;
    p4Const = pt;
}
QPointF CGD::getp4()
{
    return p4Const;
}
void CGD::setp12(QPointF pt)
{
    p12 = pt;
    p12Const = pt;
}
QPointF CGD::getp12()
{
    return p12Const;
}
void CGD::setp34(QPointF pt)
{
    p34 = pt;
    p34Const = pt;
}
QPointF CGD::getp34()
{
    return p34Const;
}
void CGD::setpz12(QPointF pt)
{
    pz12 = pt;
    pz12Const = pt;
}
QPointF CGD::getpz12()
{
    return pz12Const;
}
void CGD::setpz34(QPointF pt)
{
    pz34 = pt;
    pz34Const = pt;
}
QPointF CGD::getpz34()
{
    return pz34Const;
}
//void CGD::setGLB_QDleft(int glb)
//{
//    GLB_QDleft = glb;
//}
//int CGD::getGLB_QDleft()
//{
//    return GLB_QDleft;
//}
//void CGD::setGLB_QDright(int glb)
//{
//    GLB_QDright=glb;
//}
//int CGD::getGLB_QDright()
//{
//    return GLB_QDright;
//}
//void CGD::setIsSXDC(bool flag)
//{
//    isSXDC=flag;
//}
//bool CGD::getIsSXDC()
//{
//    return isSXDC;
//}
void CGD::setIsZXGD(bool flag)
{
    isZXGD = flag;
}
bool CGD::getIsZXGD()
{
    return isZXGD;
}
void CGD::setIsCXGD(bool flag)
{
    isCXGD = flag;
}
bool CGD::getIsCXGD()
{
    return isZXGD;
}
void CGD::setIsJJGD(bool flag)
{
    isJJGD = flag;
}
bool CGD::getIsJJGD()
{
    return isJJGD;
}
//void CGD::setGD_LCode(int code)
//{
//    GD_LCode=code;
//}
//int CGD::getGD_LCode()
//{
//    return GD_LCode;
//}
//void CGD::setGD_RCode(int code)
//{
//    GD_RCode=code;
//}
//int CGD::getGD_RCode()
//{
//    return GD_RCode;
//}
//void CGD::setCarrierFrequency(QString str)
//{
//    CarrierFrequency=str;
//}
//QString CGD::getCarrierFrequency()
//{
//    return CarrierFrequency;
//}
//void CGD::setDir_DMH(bool flag)
//{
//    if(flag == 1)
//    {
//        Dir_DMH=LEFT;
//    }
//    else
//    {
//        Dir_DMH=RIGHT;
//    }
//}
//int CGD::getDir_DMH()
//{
//    return Dir_DMH;
//}
//void CGD::setCodeFHXH(int fhxh)
//{
//    m_nCodeFHXH=fhxh;
//}
//int CGD::getCodeFHXH()
//{
//    return m_nCodeFHXH;
//}
//void CGD::setisNoShow(bool flag)
//{
//    isNoShow=flag;
//}
//bool CGD::getisNoShow()
//{
//    return isNoShow;
//}
void CGD::setMarkStr1(QString str)
{
    m_strMark1 = str;
}
QString CGD::getMarkStr1()
{
    return m_strMark1;
}
void CGD::setMarkStr2(QString str)
{
    m_strMark2 = str;
}
QString CGD::getMarkStr2()
{
    return m_strMark2;
}
void CGD::setIsHaveGDTW(bool flag)
{
    isHaveGDTW = flag;
}
bool CGD::getIsHaveGDTW()
{
    return isHaveGDTW;
}
void CGD::setGDTWName(QString name)
{
    m_strGDTWName = name;
}
QString CGD::getGDTWName()
{
    return m_strGDTWName;
}
void CGD::setPt_GDTWBSD(QPoint pt)
{
    pt_GDTWBSD = pt;
    pt_GDTWBSDConst = pt;
}
QPoint CGD::getPt_GDTWBSD()
{
    return pt_GDTWBSD;
}
void CGD::setPt_GDTWName(QPoint pt)
{
    pt_GDTWName = pt;
    pt_GDTWNameConst = pt;
}
QPoint CGD::getPt_GDTWName()
{
    return pt_GDTWName;
}
void CGD::setGDTWTime(int x)
{
    m_nGDTWTime = x;
}
int CGD::getGDTWTime()
{
    return m_nGDTWTime;
}
void CGD::setZTData(QVector<ZT_Data> ztarray)
{
    vect_ZTArray.clear();
    vect_ZTArray = ztarray;
}
void CGD::addZTData(ZT_Data ztdata)
{
    vect_ZTArray.push_back(ztdata);
}
void CGD::addZTData(QPoint pt, int wid, int heigh, int linesize, bool isup, QColor color)
{
    ZT_Data data;
    data.init();
    data.pt_Center = pt;
    data.pt_CenterConst = pt;
    data.nWidth = wid;
    data.nHeigh = heigh;
    data.nLineSize = linesize;
    data.isUP = isup;
    data.cLineColor = color;
    addZTData(data);
}
QVector<ZT_Data> CGD::getZTData()
{
    return vect_ZTArray;
}
ZT_Data CGD::getZTData(int x)
{
    ZT_Data data;
    data.init();
    if (x >= vect_ZTArray.size())
    {
        return data;
    }
    data = vect_ZTArray[x];
    return data;
}
