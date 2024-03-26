#include "T_GDDC.h"
#include "TGlobal.h"

T_GDDC::T_GDDC()
{
    m_nType = 23;      //设备类型
    m_strName = "";         //轨道名称
    m_nSX = 0;        //上下行咽喉
    m_nCode = 0xffff;      //设备编号
    m_nQDCode = 0xffff;    //关联区段设备编号
    m_textRect.setRect(0,0,0,0);          //轨道区段文本区域
    m_textRectConst.setRect(0, 0, 0, 0);     //预留站场缩放数据存储结构
    m_nCxjy = 0;      //超限绝缘节
    m_nZ = 32;         //折点
    m_nJyj = 56;       //绝缘节
    p1.setX(0);
    p1.setY(0);
    p2.setX(0);
    p2.setY(0);
    p3.setX(0);
    p3.setY(0);
    p4.setX(0);
    p4.setY(0);
    p5.setX(0);
    p5.setY(0);
    p6.setX(0);
    p6.setY(0);
    p12.setX(0);
    p12.setY(0);
    p34.setX(0);
    p34.setY(0);
    p56.setX(0);
    p56.setY(0);
    pz12.setX(0);
    pz12.setY(0);
    pz34.setX(0);
    pz34.setY(0);
    pz56.setX(0);
    pz56.setY(0);
    pCenter.setX(0);
    pCenter.setY(0);
    p1Const.setX(0);
    p1Const.setY(0);
    p2Const.setX(0);
    p2Const.setY(0);
    p3Const.setX(0);
    p3Const.setY(0);
    p4Const.setX(0);
    p4Const.setY(0);
    p5Const.setX(0);
    p5Const.setY(0);
    p6Const.setX(0);
    p6Const.setY(0);
    p12Const.setX(0);
    p12Const.setY(0);
    p34Const.setX(0);
    p34Const.setY(0);
    p56Const.setX(0);
    p56Const.setY(0);
    pz12Const.setX(0);
    pz12Const.setY(0);
    pz34Const.setX(0);
    pz34Const.setY(0);
    pz56Const.setX(0);
    pz56Const.setY(0);
    pCenterConst.setX(0);
    pCenterConst.setY(0);
    isMainGD = false;
    m_nQ = -1;        //岔前设备
    m_nD = -1;        //定位设备
    m_nF = -1;        //反位设备
    isDisPlayName = true;        //是否显示名称
    m_nDCWZ = DCSK;
    gdColor = Qt::red;
    p34C.setX(0);
    p34C.setY(0);
    p56C.setX(0);
    p56C.setY(0);  //用于存放岔心关键坐标，不从数据读取，而是再init中计算得到
    p34CConst.setX(0);
    p34CConst.setY(0);
    p56CConst.setX(0);
    p56CConst.setY(0);
    m_nSDDCCode = 0xffff;
    m_TrainNum = "";
    m_TrainDown = false;
    m_bDrawGD = true;
    m_nStationID=0;
    isSDDC=false;
    m_nOneToMore=false;
    m_nSameQDDCCode=0xffff;
    isWGDW=false;
    GLB_DCCQ=0;    //公里标
    GLB_DCCX=0;    //公里标
    GLB_DCDW=0;    //公里标
    GLB_DCFW=0;    //公里标
    isTSDC=false;
    CarrierFrequency="2300-2";
    Dir_DMH="LEFT";

}
//成员功能函数
void T_GDDC::GDDCInit()
{
    p1Const.setX(p1.x());
    p1Const.setY(p1.y());
    p2Const.setX(p2.x());
    p2Const.setY(p2.y());
    p3Const.setX(p3.x());
    p3Const.setY(p3.y());
    p4Const.setX(p4.x());
    p4Const.setY(p4.y());
    p5Const.setX(p5.x());
    p5Const.setY(p5.y());
    p6Const.setX(p6.x());
    p6Const.setY(p6.y());
    p12Const.setX(p12.x());
    p12Const.setY(p12.y());
    p34Const.setX(p34.x());
    p34Const.setY(p34.y());
    p56Const.setX(p56.x());
    p56Const.setY(p56.y());
    pz12Const.setX(pz12.x());
    pz12Const.setY(pz12.y());
    pz34Const.setX(pz34.x());
    pz34Const.setY(pz34.y());
    pz56Const.setX(pz56.x());
    pz56Const.setY(pz56.y());
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
    m_textRect.setRect(m_textRect.left(),m_textRect.top()+15,m_textRect.right(),m_textRect.bottom()+15);
    m_textRectConst.setRect(m_textRect.left(),m_textRect.top(),m_textRect.right(),m_textRect.bottom());

    double a, b, c;
    a = pCenter.x() - pz34.x();
    b = pCenter.y() - pz34.y();
    c = sqrt(a*a + b*b);
    p34C.setX(pCenter.x() - 10 * a / c);
    p34C.setY(pCenter.y() - 10 * b / c);

    a = pCenter.x() - pz56.x();
    b = pCenter.y() - pz56.y();
    c = sqrt(a*a + b*b);
    p56C.setX(pCenter.x() - 10 * a / c);
    p56C.setY(pCenter.y() - 10 * b / c);

    p34CConst.setX(p34C.x());
    p34CConst.setY(p34C.y());
    p56CConst.setX(p56C.x());
    p56CConst.setY(p56C.y());
    m_nDCWZ = DCDW;
//    m_nDCWZ = DCSK;
    gdColor = BLUE;
    DCisPLCZ=false;
    DCFSState=false;
}
void T_GDDC::Draw(QPainter *painter, unsigned int nElapsed, double nDiploid)
{
//    QPointF p_CQCenter;
//    QPointF p_DWCenter;
//    QPointF p_FWCenter;
    QColor CQcolor = Qt::red;
    QColor DWcolor = Qt::red;
    QColor FWcolor = Qt::red;
    QFont font;
    //坐标变换 2021.1.11 BJT
    p1.setX(p1Const.x() * nDiploid);
    p1.setY(p1Const.y() * nDiploid);
    p2.setX(p2Const.x() * nDiploid);
    p2.setY(p2Const.y() * nDiploid);
    p3.setX(p3Const.x() * nDiploid);
    p3.setY(p3Const.y() * nDiploid);
    p4.setX(p4Const.x() * nDiploid);
    p4.setY(p4Const.y() * nDiploid);
    p5.setX(p5Const.x() * nDiploid);
    p5.setY(p5Const.y() * nDiploid);
    p6.setX(p6Const.x() * nDiploid);
    p6.setY(p6Const.y() * nDiploid);
    p12.setX(p12Const.x() * nDiploid);
    p12.setY(p12Const.y() * nDiploid);
    p34.setX(p34Const.x() * nDiploid);
    p34.setY(p34Const.y() * nDiploid);
    p56.setX(p56Const.x() * nDiploid);
    p56.setY(p56Const.y() * nDiploid);
    pz12.setX(pz12Const.x() * nDiploid);
    pz12.setY(pz12Const.y() * nDiploid);
    pz34.setX(pz34Const.x() * nDiploid);
    pz34.setY(pz34Const.y() * nDiploid);
    pz56.setX(pz56Const.x() * nDiploid);
    pz56.setY(pz56Const.y() * nDiploid);
    pCenter.setX(pCenterConst.x() * nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid);
    m_textRect.setRect(m_textRectConst.left() * nDiploid, m_textRectConst.top() * nDiploid,
                       m_textRectConst.right() * nDiploid, m_textRectConst.bottom() * nDiploid );
    p34C.setX(p34CConst.x() * nDiploid);
    p34C.setY(p34CConst.y() * nDiploid);
    p56C.setX(p56CConst.x() * nDiploid);
    p56C.setY(p56CConst.y() * nDiploid);

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    //绘制绝缘节 2021.1.14 BJT
    if (true == getJyj(JYJ12))   //绘制岔前绝缘节
    {
        if (true == getCxjy(JYJ12))
        {
            painter->setPen(QPen(Qt::red,1));
            painter->drawEllipse(p12, 3, 3);
        }
//        painter->setPen(QPen(Qt::white,1));
        if(DCisPLCZ==true)
        {
            painter->setPen(QPen(Qt::cyan, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::white, 1));
        }
        painter->drawLine(p1, p2);
    }
    if (true == getJyj(JYJ34))   //绘制定位绝缘节
    {
        if (true == getCxjy(JYJ34))
        {
            painter->setPen(QPen(Qt::red,1));
            painter->drawEllipse(p34, 3, 3);
        }
//        painter->setPen(QPen(Qt::white,1));
        if(DCisPLCZ==true)
        {
            painter->setPen(QPen(Qt::cyan, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::white, 1));
        }
        painter->drawLine(p3, p4);
    }
    if (true == getJyj(JYJ56))   //绘制反位绝缘节
    {
        if (true == getCxjy(JYJ56))
        {
            painter->setPen(QPen(Qt::red,1));
            painter->drawEllipse(p56, 3, 3);
        }
//        painter->setPen(QPen(Qt::white,1));
        if(DCisPLCZ==true)
        {
            painter->setPen(QPen(Qt::cyan, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::white, 1));
        }
        painter->drawLine(p5, p6);
    }
    setDCColor(&CQcolor, &DWcolor, &FWcolor); //设置岔前、定位、反位绘制颜色

    //绘制岔前 2021.1.15 BJT
//    painter->setPen(QPen(CQcolor,GDWIDTH * nDiploid));
    if(DCisPLCZ==true)
    {
        painter->setPen(QPen(Qt::cyan, GDWIDTH * nDiploid));
    }
    else
    {
        painter->setPen(QPen(CQcolor,GDWIDTH * nDiploid));
    }
    if (true == getZ(JYJ12))  //岔前有折点
    {
        painter->drawLine(p12, pz12);
        painter->drawLine(pz12, pCenter);
    }
    else
    {
        painter->drawLine(p12, pCenter);
    }
    //绘制定位 2021.1.15 BJT
//    painter->setPen(QPen(DWcolor,GDWIDTH * nDiploid));
    if(DCisPLCZ==true)
    {
        painter->setPen(QPen(Qt::cyan, GDWIDTH * nDiploid));
    }
    else
    {
        painter->setPen(QPen(DWcolor,GDWIDTH * nDiploid));
    }
    if (true == getZ(JYJ34))  //岔前有折点
    {
        painter->drawLine(p34, pz34);
        painter->drawLine(pz34, pCenter);
    }
    else
    {
        painter->drawLine(p34, pCenter);
    }
    //绘制反位 2021.1.15 BJT
//    painter->setPen(QPen(FWcolor,GDWIDTH * nDiploid));
    if(DCisPLCZ==true)
    {
        painter->setPen(QPen(Qt::cyan, GDWIDTH * nDiploid));
    }
    else
    {
        painter->setPen(QPen(FWcolor,GDWIDTH * nDiploid));
    }
    if (true == getZ(JYJ56))  //岔前有折点
    {
        painter->drawLine(p56, pz56);
        painter->drawLine(pz56, pCenter);
    }
    else
    {
        painter->drawLine(p56, pCenter);
    }
    //绘制岔心 2021.1.15 BJT
    if (DCDW == getDCWZ())
    {
        painter->setPen(QPen(Qt::black,GDWIDTH * nDiploid));
        painter->drawLine(pCenter, p56C);
        painter->setPen(QPen(Qt::green,GDWIDTH * nDiploid));
        painter->drawLine(pCenter, p34C);
    }
    else if (DCFW == getDCWZ())
    {
        painter->setPen(QPen(Qt::black,GDWIDTH * nDiploid));
        painter->drawLine(pCenter, p34C);
        painter->setPen(QPen(Qt::yellow,GDWIDTH * nDiploid));
        painter->drawLine(pCenter, p56C);
    }
    else
    {
        if (0 == nElapsed % 2)
        {
            painter->setPen(QPen(Qt::red,GDWIDTH * nDiploid));
            painter->drawLine(pCenter, p34C);
            painter->setPen(QPen(Qt::red,GDWIDTH * nDiploid));
            painter->drawLine(pCenter, p56C);
        }
        else
        {
            painter->setPen(QPen(Qt::black,GDWIDTH * nDiploid));
            painter->drawLine(pCenter, p34C);
            painter->setPen(QPen(Qt::black,GDWIDTH * nDiploid));
            painter->drawLine(pCenter, p56C);
        }
    }
    //绘制道岔名称 2021.1.14 BJT
    if (getDisplayName()) //单站界面名称显示
    {
        font.setFamily("Times New Roman");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        if (DCDW == getDCWZ())
        {
            painter->setPen(Qt::green);//设置画笔颜色
            painter->drawText(m_textRect.left(), m_textRect.top(), getGDDCName()); //绘制文本
        }
        else if (DCFW == getDCWZ())
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            painter->drawText(m_textRect.left(), m_textRect.top(), getGDDCName()); //绘制文本
        }
        else
        {
            painter->setPen(Qt::white);//设置画笔颜色
            painter->drawText(m_textRect.left(), m_textRect.top(), getGDDCName()); //绘制文本
        }
    }
}
void T_GDDC::setDCColor(QColor *cq_Color, QColor *dw_Color, QColor *fw_Color)
{
    //设置岔前绘制颜色
    if (true == getIsMainGD())
    {
        *cq_Color = getQDColor();
    }
    else
    {
        *cq_Color = Qt::blue;
    }
    //设置定位绘制颜色
    if (true == getIsMainGD())
    {
        if (DCDW == getDCWZ())
        {
            *dw_Color = getQDColor();
        }
        else
        {
            if ((DCSK == getDCWZ()) && (getQDColor() == Qt::red))
            {
                *dw_Color = getQDColor();
            }
            else
            {
                *dw_Color = Qt::blue;
            }
        }
    }
    else
    {
        *dw_Color = Qt::blue;
    }
    //设置反位绘制颜色
    if (true == getIsMainGD())
    {
        if (DCFW == getDCWZ())
        {
            *fw_Color = getQDColor();
        }
        else
        {
            if ((DCSK == getDCWZ()) && (getQDColor() == (Qt::red)))
            {
                *fw_Color = getQDColor();
            }
            else
            {
                *fw_Color = Qt::blue;
            }
        }
    }
    else
    {
        *fw_Color = Qt::blue;
    }
}
//成员变量封装函数
void T_GDDC::setType(unsigned int type)
{
    m_nType = type;
}
unsigned int T_GDDC::getType()
{
    return m_nType;
}
void T_GDDC::setGDDCName(QString name)
{
    m_strName = name;
}
QString T_GDDC::getGDDCName()
{
    return m_strName;
}
void T_GDDC::setSX(unsigned int nSX)
{
    m_nSX = nSX;
}
unsigned int T_GDDC::getSX()
{
    return m_nSX;
}
void T_GDDC::setCode(unsigned int code)
{
    m_nCode = code;
}
unsigned int T_GDDC::getCode()
{
    return m_nCode;
}
void T_GDDC::setQDCode(unsigned int code)
{
    m_nQDCode = code;
}
unsigned int T_GDDC::getQDCode()
{
    return m_nQDCode;
}
void T_GDDC::setCxjy(unsigned int nCxjy)
{
    m_nCxjy = nCxjy;
}
bool T_GDDC::getCxjy(unsigned int nCxjy)
{
    if ((m_nCxjy & nCxjy)> 0)
        return true;
    else
        return false;
}
void T_GDDC::setZ(unsigned int nZ)
{
    m_nZ = nZ;
}
bool T_GDDC::getZ(unsigned int nZ)
{
    if ((m_nZ & nZ)> 0)
        return true;
    else
        return false;
}
void T_GDDC::setJyj(unsigned int nJyj)
{
    m_nJyj = nJyj;
}
bool T_GDDC::getJyj(unsigned int nJyj)
{
    if ((m_nJyj & nJyj)> 0)
        return true;
    else
        return false;
}
void T_GDDC::setCQdev(int dev)
{
    m_nQ = dev;
}
int T_GDDC::getCQdev()
{
    return m_nQ;
}
void T_GDDC::setDWdev(int dev)
{
    m_nD = dev;
}
int T_GDDC::getDWdev()
{
    return m_nD;
}
void T_GDDC::setFWdev(int dev)
{
    m_nF = dev;
}
int T_GDDC::getFWdev()
{
    return m_nF;
}
void T_GDDC::setDisplayName(bool nFlag)
{
    isDisPlayName = nFlag;
}
bool T_GDDC::getDisplayName()
{
    return isDisPlayName;
}
void T_GDDC::setIsMainGD(bool nFlag)
{
    isMainGD = nFlag;
}
bool T_GDDC::getIsMainGD()
{
    if (m_bDrawGD || isMainGD)
    {
        return true;
    }
    return false;
}
void T_GDDC::setDCWZ(unsigned int dcwz)
{
    m_nDCWZ = dcwz;
}
unsigned int  T_GDDC::getDCWZ()
{
    return m_nDCWZ;
}
void T_GDDC::setQDColor(QColor color)
{
    gdColor = color;
}
QColor T_GDDC::getQDColor()
{
    return gdColor;
}
void T_GDDC::setSDDCCode(int sddc_code)
{
    m_nSDDCCode = sddc_code;
}
int T_GDDC::getSDDCCode()
{
    return m_nSDDCCode;
}
int T_GDDC::setDrawGD(bool bDrawGD)
{
    m_bDrawGD = bDrawGD;
    if (getDCWZ() == DCDW)
        return m_nD;
    if (getDCWZ() == DCFW)
        return m_nF;
    return -1;
}
void T_GDDC::setStationID(int id)
{
    m_nStationID=id;
}
int T_GDDC::getStationID()
{
    return m_nStationID;
}
void T_GDDC::setIsSDDC(bool flag)
{
    isSDDC=flag;
}
bool T_GDDC::getIsSDDC()
{
    return isSDDC;
}
void T_GDDC::setOneToMore(int onetomore)
{
    m_nOneToMore=onetomore;
}
int T_GDDC::getOneToMore()
{
    return m_nOneToMore;
}
void T_GDDC::setTextRect(QRect rect)
{
    m_textRect=rect;
}
void T_GDDC::setp1(QPoint pt)
{
    p1.setX(pt.x());
    p1.setY(pt.y());
}
void T_GDDC::setp2(QPoint pt)
{
    p2.setX(pt.x());
    p2.setY(pt.y());
}
void T_GDDC::setp3(QPoint pt)
{
    p3.setX(pt.x());
    p3.setY(pt.y());
}
void T_GDDC::setp4(QPoint pt)
{
    p4.setX(pt.x());
    p4.setY(pt.y());
}
void T_GDDC::setp5(QPoint pt)
{
    p5.setX(pt.x());
    p5.setY(pt.y());
}
void T_GDDC::setp6(QPoint pt)
{
    p6.setX(pt.x());
    p6.setY(pt.y());
}
void T_GDDC::setp12(QPoint pt)
{
    p12.setX(pt.x());
    p12.setY(pt.y());
}
void T_GDDC::setp34(QPoint pt)
{
    p34.setX(pt.x());
    p34.setY(pt.y());
}
void T_GDDC::setp56(QPoint pt)
{
    p56.setX(pt.x());
    p56.setY(pt.y());
}
void T_GDDC::setpz12(QPoint pt)
{
    pz12.setX(pt.x());
    pz12.setY(pt.y());
}
void T_GDDC::setpz34(QPoint pt)
{
    pz34.setX(pt.x());
    pz34.setY(pt.y());
}
void T_GDDC::setpz56(QPoint pt)
{
    pz56.setX(pt.x());
    pz56.setY(pt.y());
}
void T_GDDC::setpCenter(QPoint pt)
{
    pCenter.setX(pt.x());
    pCenter.setY(pt.y());
}
void T_GDDC::setSameQDDCCode(int code)
{
    m_nSameQDDCCode=code;
}
int T_GDDC::getSameQDDCCode()
{
    return m_nSameQDDCCode;
}
void T_GDDC::setIsWGDW(bool flag)
{
    isWGDW=flag;
}
bool T_GDDC::getIsWGDW()
{
    return isWGDW;
}
void T_GDDC::setGLB_DCCQ(int glb)
{
    GLB_DCCQ=glb;
}
int T_GDDC::getGLB_DCCQ()
{
    return GLB_DCCQ;
}
void T_GDDC::setGLB_DCCX(int glb)
{
    GLB_DCCX=glb;
}
int T_GDDC::getGLB_DCCX()
{
    return GLB_DCCX;
}
void T_GDDC::setGLB_DCDW(int glb)
{
    GLB_DCDW=glb;
}
int T_GDDC::getGLB_DCDW()
{
    return GLB_DCDW;
}
void T_GDDC::setGLB_DCFW(int glb)
{
    GLB_DCFW=glb;
}
int T_GDDC::getGLB_DCFW()
{
    return GLB_DCFW;
}
void T_GDDC::setIsTSDC(bool flag)
{
    isTSDC=flag;
}
bool T_GDDC::getIsTSDC()
{
    return isTSDC;
}
void T_GDDC::setCarrierFrequency(QString str)
{
    CarrierFrequency=str;
}
QString T_GDDC::getCarrierFrequency()
{
    return CarrierFrequency;
}
void T_GDDC::setDir_DMH(QString str)
{
    Dir_DMH=str;
}
QString T_GDDC::getDir_DMH()
{
    return Dir_DMH;
}
