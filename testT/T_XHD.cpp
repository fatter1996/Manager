#include "T_XHD.h"
#include "TGlobal.h"
#include <QCoreApplication>

T_XHD::T_XHD()
{
    m_nType = 32;
    m_strName = (""); //信号机名称
    m_nCode = 0xffff;
    pCenter.setX(0);            //设备中心点
    pCenter.setY(0);
    pCenterConst.setX(0);       //缩放及偏移使用
    pCenterConst.setY(0);
    m_nXHDType = JZ_XHJ;  //信号机类型
    m_nSX = 0;               //上下行咽喉
    m_textRect.setRect(0, 0, 0, 0);       //信号机文本区域
    m_textRectConst.setRect(0, 0, 0, 0);  //预留站场缩放数据存储结构
    isSignalType = false;   //是否是虚拟信号
    m_nSafeLamp = XHD_HD; //安全灯光
    isHigh = false;       //是否为高柱
    isDisPlayName = true;      //是否显示名称
    m_nXHDState = m_nSafeLamp;
    isLCBt_Down = false;
    isDCBt_Down = false;
    nXHANSizeX = 36;
    nXHANSizeY = 28;
    nXHANSizeXConst = nXHANSizeX;
    nXHANSizeYConst = nXHANSizeY;
    m_rectLC.setRect(0, 0, 0, 0);       //列车按钮区域
    m_rectDC.setRect(0, 0, 0, 0);       //调车按钮区域
    m_rectLCConst.setRect(0, 0, 0, 0);
    m_rectDCConst.setRect(0, 0, 0, 0);
    m_nStationID = 0;
    isYDSD = false;
    signalDCAN = false;
    m_nGLXHNode = 0xffff;
    GLB_XHD = 0;  //公里标
    Dir_XH = "Left";
    XH_LCode = 0xffff;
    XH_RCode = 0xffff;
    isHDDSJC = false;
    XH_WZInfo = XH_CZXH;
    XH_Show = 0;
    XH_LCSD = false;
    XH_LCZD = false;
    XH_DCSD = false;
    XH_DCZD = false;
    XH_YXSD = false;
    XH_YXZD = false;
    XH_CYFX = false;
    XH_BGAN = false;
    XH_PDJZ = false;
    XH_LCZH = false;
}

//成员功能函数实现
void T_XHD::XHDInit()
{
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
    m_textRect.setCoords(m_textRect.left(), m_textRect.top() + 10, m_textRect.right(), m_textRect.bottom() + 10);
    m_textRectConst.setRect(m_textRect.left(), m_textRect.top(), m_textRect.right(), m_textRect.bottom());

    m_nXHDState = m_nSafeLamp;

    nXHANSizeX = 36;
    nXHANSizeY = 28;
    nXHANSizeXConst = nXHANSizeX;
    nXHANSizeYConst = nXHANSizeY;

    if (getType() == 32)
    {
        m_rectLC.setRect(pCenter.x() - 28, pCenter.y() - 8, 16, 16);
    }
    else if (getType() == 34)
    {
        m_rectLC.setRect(pCenter.x() + 28 - 16, pCenter.y() - 8, 16, 16);
    }
    m_rectDC.setRect(pCenter.x() - 8, pCenter.y() - 8, 16, 16);
    m_rectDCConst = m_rectDC;
    m_rectLCConst = m_rectLC;
    m_rectDCConst = m_rectDC;
    showtype = JKJ;
    XHisPLCZ=false;
    XHFSState=false;
}
void T_XHD::Draw(QPainter *painter, unsigned int nElapsed, double nDiploid, unsigned char type)
{
    //CBitmap m_MemuImage;
    //CBitmap m_MemuImage1;
    showtype = type;
    QPoint p_Line1;  //信号机竖线绘制坐标
    QPoint p_Line2;  //信号机竖线绘制坐标
    QPoint p_Line3;  //信号机高柱横线绘制坐标
    QPoint p_Line4;  //信号机高柱横线绘制坐标
    QPoint p_Xhd;    //信号机靠近信号柱灯位坐标
    QPoint p_Xhd2;   //信号机远离信号柱灯位坐标
    QColor xhd1_Color = Qt::black;  //信号机近端信号灯位显示颜色
    QColor xhd2_Color = Qt::black;  //信号机远端信号灯位显示颜色
    //QPoint p_LCAN;
    //QPoint p_DCAN;
    QFont font;

    //坐标变换 2021.1.11 BJT
    pCenter.setX(pCenterConst.x() * nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid);
    m_textRect.setRect(m_textRectConst.left() * nDiploid, m_textRectConst.top() * nDiploid, m_textRectConst.right() * nDiploid, m_textRectConst.bottom() * nDiploid);

    //nXHANSizeX = nXHANSizeXConst * nDiploid;
    //nXHANSizeY = nXHANSizeYConst * nDiploid;  //绘制时候用nXHANSizeYConst，因为在绘制函数内部会处理缩放，按钮区域用nXHANSizeY

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    m_rectLC.setCoords(m_rectLCConst.left()* nDiploid, m_rectLCConst.top()* nDiploid, m_rectLCConst.right()* nDiploid, m_rectLCConst.bottom()* nDiploid);
    //绘制信号机柱 2021.1.12 BJT
    if (false == getSignalType())   //虚拟按钮不绘制信号机柱
    {
        if ((31 == getType()) || (32 == getType()))
        {
            p_Line1.setX(pCenter.x() - 10 * nDiploid);
            p_Line1.setY(pCenter.y() - 8 * nDiploid);
            p_Line2.setX(pCenter.x() - 10 * nDiploid);
            p_Line2.setY(pCenter.y() + 8 * nDiploid);
        }
        else
        {
            p_Line1.setX(pCenter.x() + 10 * nDiploid);
            p_Line1.setY(pCenter.y() - 8 * nDiploid);
            p_Line2.setX(pCenter.x() + 10 * nDiploid);
            p_Line2.setY(pCenter.y() + 8 * nDiploid);
        }
//        painter->setPen(Qt::white);
        if(XHisPLCZ==true)
        {
            painter->setPen(QPen(Qt::cyan, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::white, 1));
        }
        painter->drawLine(p_Line1, p_Line2); //绘制信号机柱竖线

        if (true == getIsHigh())
        {
            p_Line3.setX(p_Line1.x());
            p_Line3.setY((p_Line1.y() + p_Line2.y()) / 2);
            if ((31 == getType()) || (32 == getType()))
            {
                p_Line4.setX(p_Line1.x() + 6 * nDiploid);
                p_Line4.setY(p_Line3.y());
            }
            else
            {
                p_Line4.setX(p_Line1.x() - 6 * nDiploid);
                p_Line4.setY(p_Line3.y());
            }
//            painter->setPen(Qt::white);
            if(XHisPLCZ==true)
            {
                painter->setPen(QPen(Qt::cyan, 2));
            }
            else
            {
                painter->setPen(QPen(Qt::white, 1));
            }
            painter->drawLine(p_Line3, p_Line4); //绘制信号机柱高柱短横线
        }
    }

    //绘制信号机灯位 2021.1.12 BJT
    if (false == getSignalType())   //虚拟按钮不绘制信号灯位
    {
        if ((31 == getType()) || (32 == getType()))
        {
            if (true == getIsHigh())
            {
                p_Xhd.setX(pCenter.x() + 4 * nDiploid);
                p_Xhd.setY(pCenter.y());
            }
            else
            {
                p_Xhd.setX(pCenter.x() - 1 * nDiploid);
                p_Xhd.setY(pCenter.y());
            }
            p_Xhd2.setX(p_Xhd.x() + 16 * nDiploid);
            p_Xhd2.setY(p_Xhd.y());
        }
        else
        {
            if (true == getIsHigh())
            {
                p_Xhd.setX(pCenter.x() - 4 * nDiploid);
                p_Xhd.setY(pCenter.y());
            }
            else
            {
                p_Xhd.setX(pCenter.x() + 1 * nDiploid);
                p_Xhd.setY(pCenter.y());
            }
            p_Xhd2.setX(p_Xhd.x() - 16 * nDiploid);
            p_Xhd2.setY(p_Xhd.y());
        }
        setXHD_Color(&xhd1_Color, &xhd2_Color, nElapsed);   //根据信号机状态实时设置信号机灯位颜色
//        painter->setPen(QPen(Qt::white, 1));
        if(XHisPLCZ==true)
        {
            painter->setPen(QPen(Qt::cyan, 2));
        }
        else
        {
            painter->setPen(QPen(Qt::white, 1));
        }
        painter->setBrush(xhd1_Color);
        painter->drawEllipse(p_Xhd, int(8 * nDiploid), int(8 * nDiploid));
        if ((34 == getType()) || (32 == getType()))
        {
            painter->setBrush(xhd2_Color);
            painter->drawEllipse(p_Xhd2, int(8 * nDiploid), int(8 * nDiploid));
        }
        else if (((XHD_UU == getXHDState())) || (XHD_LU == getXHDState()) || (XHD_LL == getXHDState()) || (XHD_YD == getXHDState()) || (XHD_USU == getXHDState()))
        {
            painter->setBrush(xhd2_Color);
            painter->drawEllipse(p_Xhd2, int(8 * nDiploid), int(8 * nDiploid));
        }
    }

#if 1   //线路模拟机不需要绘制信号机按钮
    //绘制信号机按钮 2021.1.12 BJT
    if (showtype != JCJ)
    {
        if ((true == getSignalType()) || (showtype == CTC && getXHDType() == JZ_XHJ))
        {
            if (false == getIsLCDown())
            {
                if (32 == getType())
                    painter->drawPixmap(m_rectLC, QPixmap(":/img/LCAN.bmp"));
            }
            else
            {
//                if (nElapsed % 2 == 0)
//                {
//                    m_MemuImage.LoadBitmap(IDB_BTN_LCAN);
//                    drawJM.DrawBitmap(pDC, &m_MemuImage, pCenter, nXHANSizeXConst, nXHANSizeYConst, nDiploid);//绘制时候用nXHANSizeYConst，因为在绘制函数内部会处理缩放，按钮区域用nXHANSizeY
//                }
            }
        }
        else
        {
            if ((32 == getType()) || (34 == getType()))
            {
//                if (32 == getType())
//                {
//                    p_LCAN.SetPoint(pCenter.x - 60 * nDiploid - nXHANSizeX, pCenter.y - 32 * nDiploid + nXHANSizeY);
//                }
//                else if (34 == getType())
//                {
//                    p_LCAN.SetPoint(pCenter.x + 20 * nDiploid + nXHANSizeX, pCenter.y + 5 * nDiploid - nXHANSizeY);
//                }
//                m_rectLC.SetRect(p_LCAN.x, p_LCAN.y, p_LCAN.x + nXHANSizeX, p_LCAN.y + nXHANSizeY);
                QString path = QString::fromLocal8Bit("%1/data/bmp/LCAN.bmp").arg(QCoreApplication::applicationDirPath());
                QString path1 = QString::fromLocal8Bit("%1/data/bmp/LCANS.bmp").arg(QCoreApplication::applicationDirPath());
                if (showtype == CTC)
                {
                    path = QString::fromLocal8Bit("%1/data/bmp/DCANCTC.png").arg(QCoreApplication::applicationDirPath());
                    path1 = QString::fromLocal8Bit("%1/data/bmp/DCANCTCS.png").arg(QCoreApplication::applicationDirPath());
                }
                if (false == getIsLCDown())
                {
                    painter->drawPixmap(m_rectLC, QPixmap(path));
                }
                else
                {
                    if (showtype == CTC)
                    {
                        painter->drawPixmap(m_rectLC, QPixmap(path1));
                    }
                    else if (nElapsed % 2 == 0)
                    {
                        painter->drawPixmap(m_rectLC, QPixmap(path1));
                    }
                    else
                    {
                        painter->drawPixmap(m_rectLC, QPixmap(path));
                    }
                }
            }
//            if ((31 == getType()) || (33 == getType()) || (((32 == getType()) || (34 == getType())) && (JZ_XHJ != getXHDType())))
//            {
//                if ((31 == getType()) || (32 == getType()))
//                {
//                    p_DCAN.SetPoint(pCenter.x - 15 * nDiploid - nXHANSizeX, pCenter.y + 5 * nDiploid - nXHANSizeY);
//                }
//                else if ((33 == getType()) || (34 == getType()))
//                {
//                    p_DCAN.SetPoint(pCenter.x + 15 * nDiploid, pCenter.y - 5 * nDiploid);
//                }
//                m_rectDC.SetRect(p_DCAN.x, p_DCAN.y, p_DCAN.x + nXHANSizeX, p_DCAN.y + nXHANSizeY);
//                if (FALSE == getIsDCDown())
//                {
//                    m_MemuImage1.LoadBitmap(IDB_BTN_DCAN);
//                    drawJM.DrawBitmap(pDC, &m_MemuImage1, p_DCAN, nXHANSizeXConst, nXHANSizeYConst, nDiploid);//绘制时候用nXHANSizeYConst，因为在绘制函数内部会处理缩放，按钮区域用nXHANSizeY
//                }
//                else
//                {
//                    if (nElapsed % 2 == 0)
//                    {
//                        m_MemuImage1.LoadBitmap(IDB_BTN_DCAN);
//                        drawJM.DrawBitmap(pDC, &m_MemuImage1, p_DCAN, nXHANSizeXConst, nXHANSizeYConst, nDiploid);//绘制时候用nXHANSizeYConst，因为在绘制函数内部会处理缩放，按钮区域用nXHANSizeY
//                    }
//                }
//            }
        }
    }
#endif
    //绘制信号机名称 2021.1.11 BJT
    if (true == getDisplayName())
    {
        font.setFamily("Times New Roman");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        if (getIsDCDown())
        {
            if (nElapsed % 2 == 0)
            {
                painter->setPen(Qt::white);//设置画笔颜色
            }
            else
            {
                painter->setPen(Qt::red);//设置画笔颜色
            }

        }
        else
            painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top(), getXHDName());
        painter->setPen(Qt::NoPen);
    }
}
void T_XHD::setXHD_Color(QColor *xhd1_Color, QColor *xhd2_Color, unsigned int nElapsed)
{
    *xhd1_Color = Qt::black;
    *xhd2_Color = Qt::black;
    if (XHD_HD == getXHDState())
    {
        *xhd1_Color = Qt::red;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_DS == getXHDState())
    {
        if (0 == nElapsed % 2)
        {
            *xhd1_Color = Qt::red;
        }
        else
        {
            *xhd1_Color = Qt::black;
        }
        *xhd2_Color = Qt::black;
    }
    else if (XHD_AD == getXHDState())
    {
        *xhd1_Color = Qt::blue;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_BD == getXHDState())
    {
        *xhd1_Color = Qt::white;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_LD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_UD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::yellow;
    }
    else if (XHD_UU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::yellow;
    }
    else if (XHD_LL == getXHDState())
    {
        *xhd1_Color = Qt::green;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_YD == getXHDState())
    {
        *xhd1_Color = Qt::white;
        *xhd2_Color = Qt::red;
    }
    else if (XHD_LU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::green;
    }
    else if (XHD_2U == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        *xhd2_Color = Qt::black;
    }
    else if (XHD_BS == getXHDState())
    {
        if (0 == nElapsed % 2)
        {
            *xhd1_Color = Qt::white;
        }
        else
        {
            *xhd1_Color = Qt::black;
        }
        *xhd2_Color = Qt::black;
    }
    else if (XHD_US == getXHDState())
    {
        *xhd1_Color = Qt::black;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::yellow;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_LS == getXHDState())
    {
        *xhd1_Color = Qt::black;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::green;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_USU == getXHDState())
    {
        *xhd1_Color = Qt::yellow;
        if (0 == nElapsed % 2)
        {
            *xhd2_Color = Qt::yellow;
        }
        else
        {
            *xhd2_Color = Qt::black;
        }
    }
    else if (XHD_MD == getXHDState())
    {
        *xhd1_Color = Qt::black;
        *xhd2_Color = Qt::black;
    }
}

int T_XHD::MouseMove(QPoint pt)
{
    if (showtype == JCJ)
    {
        return -1;
    }
    if (showtype == CTC)
    {
        if (m_rectLC.contains(pt) && (32 == getType() || 34 == getType()) && (getXHDType() != JZ_XHJ))
        {
            return 2;
        }
        else if (m_rectDC.contains(pt) && (32 == getType() || 34 == getType()))
        {
            return 1;
        }
        else if (m_rectDC.contains(pt))
        {
            return 2;
        }
    }
    else
    {
        if (m_rectLC.contains(pt))
        {
            return 1;
        }
        else if (m_rectDC.contains(pt))
        {
            return 2;
        }
    }
    return -1;
}

//成员变量封装函数
void T_XHD::setType(unsigned int nType)
{
    m_nType = nType;
}
unsigned int T_XHD::getType()
{
    return m_nType;
}
void T_XHD::setXHDName(QString string)
{
    m_strName = string;
}
QString T_XHD::getXHDName()
{
    return m_strName;
}
void T_XHD::setCode(unsigned int code)
{
    m_nCode = code;
}
unsigned int T_XHD::getCode()
{
    return m_nCode;
}
void T_XHD::setXHDType(QString strType)
{
    unsigned int nType = JZ_XHJ;
    if (strType == "JZXHJ")
    {
        nType = JZ_XHJ;
    }
    else if (strType == "DCJL_XHJ")
    {
        nType = DCJL_XHJ;
    }
    else if (strType == "JZFS_XHJ")
    {
        nType = JZFS_XHJ;
    }
    else if (strType == "SXCZ_XHJ")
    {
        nType = SXCZ_XHJ;
    }
    else if (strType == "YG_XHJ")
    {
        nType = YG_XHJ;
    }
    else if (strType == "CZ_XHJ")
    {
        nType = CZ_XHJ;
    }
    else if (strType == "DC_XHJ")
    {
        nType = DC_XHJ;
    }
    else if (strType == "DCFS_XHJ")
    {
        nType = DCFS_XHJ;
    }
    else if (strType == "FCJL_XHJ")
    {
        nType = FCJL_XHJ;
    }
    else if (strType == "JLFS_XHJ")
    {
        nType = JLFS_XHJ;
    }
    m_nXHDType = nType;
}
unsigned int T_XHD::getXHDType()
{
    return m_nXHDType;
}
void T_XHD::setSX(unsigned int nSX)
{
    m_nSX = nSX;
}
unsigned int T_XHD::getSX()
{
    return m_nSX;
}
void T_XHD::setSignalType(bool flag)
{
    isSignalType = flag;
}
bool T_XHD::getSignalType()
{
    return isSignalType;
}
void T_XHD::setSafeLamp(QString strSafelamp)
{
    unsigned int safelamp = XHD_HD;
    if (strSafelamp == "XHD_HD")
    {
        safelamp = XHD_HD;
    }
    else
    {
        safelamp = XHD_AD;
    }
    m_nSafeLamp = safelamp;
}
unsigned int T_XHD::getSafeLamp()
{
    return m_nSafeLamp;
}
void T_XHD::setIsHigh(bool ishigh)
{
    isHigh = ishigh;
}
bool T_XHD::getIsHigh()
{
    return isHigh;
}
void T_XHD::setDisplayName(bool nFlag)
{
    isDisPlayName = nFlag;
}
bool T_XHD::getDisplayName()
{
    return isDisPlayName;
}
void T_XHD::setXHDState(unsigned int state)
{
    m_nXHDState = state;
}
unsigned int T_XHD::getXHDState()
{
    return m_nXHDState;
}
void T_XHD::setIsLCDown(bool nFlag)
{
    isLCBt_Down = nFlag;
}
bool T_XHD::getIsLCDown()
{
    return isLCBt_Down;
}
void T_XHD::setIsDCDown(bool nFlag)
{
    isDCBt_Down = nFlag;
}
bool T_XHD::getIsDCDown()
{
    return isDCBt_Down;
}
void T_XHD::setStationID(int id)
{
    m_nStationID = id;
}
int T_XHD::getStationID()
{
    return m_nStationID;
}
void T_XHD::setTextRect(QRect rect)
{
    m_textRect = rect;
}
void T_XHD::setIsYDSD(bool flag)
{
    isYDSD = flag;
}
bool T_XHD::getIsYDSD()
{
    return isYDSD;
}
void T_XHD::setLCAN_Rect(QRect rect)
{
    m_rectLC = rect;
}
void T_XHD::setDCAN_Rect(QRect rect)
{
    m_rectDC = rect;
}
void T_XHD::setYDAN_Rect(QRect rect)
{
    m_rectYD = rect;
}
void T_XHD::setSignalDCAN(bool flag)
{
    signalDCAN = flag;
}
bool T_XHD::getSignalDCAN()
{
    return signalDCAN;
}
void T_XHD::setGLB_XHD(int glb)
{
    GLB_XHD = glb;
}
int T_XHD::getGLB_XHD()
{
    return GLB_XHD;
}
void T_XHD::setGLXHNode(int node)
{
    m_nGLXHNode = node;
}
int T_XHD::getGLXHNode()
{
    return m_nGLXHNode;
}
void T_XHD::setDir_XH(QString dir)
{
    Dir_XH = dir;
}
QString T_XHD::getDir_XH()
{
    return Dir_XH;
}
void T_XHD::setXH_LCode(int code)
{
    XH_LCode = code;
}
int T_XHD::getXH_LCode()
{
    return XH_LCode;
}
void T_XHD::setXH_RCode(int code)
{
    XH_RCode = code;
}
int T_XHD::getXH_RCode()
{
    return XH_RCode;
}
void T_XHD::setisHDDSJC(bool flag)
{
    isHDDSJC = flag;
}
bool T_XHD::getisHDDSJC()
{
    return isHDDSJC;
}
void T_XHD::setXH_WZInfo(QString info)
{
    if (info == "XH_CZXH")
    {
        XH_WZInfo = XH_CZXH;
    }
    else if (info == "XH_DZXH")
    {
        XH_WZInfo = XH_DZXH;
    }
    else if (info == "XH_BZXH")
    {
        XH_WZInfo = XH_BZXH;
    }
    else if (info == "XH_JTXXH")
    {
        XH_WZInfo = XH_JTXXH;
    }
}
int T_XHD::getXH_WZInfo()
{
    return XH_WZInfo;
}
void T_XHD::setXH_Show(int show)
{
    XH_Show = show;
}
int T_XHD::getXH_Show()
{
    return XH_Show;
}
void T_XHD::setXH_LCSD(bool flag)
{
    XH_LCSD = flag;
}
bool T_XHD::getXH_LCSD()
{
    return XH_LCSD;
}
void T_XHD::setXH_LCZD(bool flag)
{
    XH_LCZD = flag;
}
bool T_XHD::getXH_LCZD()
{
    return XH_LCZD;
}
void T_XHD::setXH_DCSD(bool flag)
{
    XH_DCSD = flag;
}
bool T_XHD::getXH_DCSD()
{
    return XH_DCSD;
}
void T_XHD::setXH_DCZD(bool flag)
{
    XH_DCZD = flag;
}
bool T_XHD::getXH_DCZD()
{
    return XH_DCZD;
}
void T_XHD::setXH_YXSD(bool flag)
{
    XH_YXSD = flag;
}
bool T_XHD::getXH_YXSD()
{
    return XH_YXSD;
}
void T_XHD::setXH_YXZD(bool flag)
{
    XH_YXZD = flag;
}
bool T_XHD::getXH_YXZD()
{
    return XH_YXZD;
}
void T_XHD::setXH_CYFX(bool flag)
{
    XH_CYFX = flag;
}
bool T_XHD::getXH_CYFX()
{
    return XH_CYFX;
}
void T_XHD::setXH_BGAN(bool flag)
{
    XH_BGAN = flag;
}
bool T_XHD::getXH_BGAN()
{
    return XH_BGAN;
}
void T_XHD::setXH_PDJZ(bool flag)
{
    XH_PDJZ = flag;
}
bool T_XHD::getXH_PDJZ()
{
    return XH_PDJZ;
}
void T_XHD::setXH_LCZH(bool flag)
{
    XH_LCZH = flag;
}
bool T_XHD::getXH_LCZH()
{
    return XH_LCZH;
}
void T_XHD::setpCenter(QPoint pt)
{
    pCenter = pt;
}

void T_XHD::clearState()
{
    isLCBt_Down = false;
    isDCBt_Down = false;
    setXHDState(getSafeLamp());
}
