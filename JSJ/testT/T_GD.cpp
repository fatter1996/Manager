#include "T_GD.h"
#include "TGlobal.h"

T_GD::T_GD()
{
    p1.setX(-1);
    p1.setY(-1);
    p2.setX(-1);
    p2.setY(-1);
    p3.setX(-1);
    p3.setY(-1);
    p4.setX(-1);
    p4.setY(-1);
    p12.setX(-1);
    p12.setY(-1);
    p34.setX(-1);
    p34.setY(-1);
    pz12.setX(-1);
    pz12.setY(-1);
    pz34.setX(-1);
    pz34.setY(-1);
    pCenter.setX(-1);
    pCenter.setY(-1);            //设备中心点
    m_nType = 1;              //设备类型
    m_nGDType = QD;            //轨道区段类型
    m_strName = "";
    m_nZ = 0;                   //折点个数
    m_nSX = 0;                  //上下行咽喉
    m_nCode = 0xffff;           //设备编号
    isDisPlayName = true;       //是否显示名称
    gdColor = Qt::red;              //轨道区段颜色
    isLostTrainNum = false;     //是否列车丢失--暂时跟着车次走，先预留
    isLock = false;             //是否封锁
    isGDFLBL = false;           //是否分录不良
    m_textRect.setRect(0,0,0,0);
    m_nStationID=0;
    GLB_QDleft=0;
    GLB_QDright=0;
    isSXDC = false;
    isZXGD = false;
    isCXGD = false;
    isJJGD = false;
    GD_LCode= 0xffff;
    GD_RCode= 0xffff;
    CarrierFrequency="2300-2";
    Dir_DMH="LEFT";
}

//成员功能函数实现
void T_GD::GDInit()
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
    m_textRect.setRect(m_textRect.left(),m_textRect.top()+10,m_textRect.right(),m_textRect.bottom()+10);
    m_textRectConst.setRect(m_textRect.left(),m_textRect.top(),m_textRect.right(),m_textRect.bottom());
    gdColor = BLUE;
    GDisPLCZ=false;
    GDPowerSupply=1;
    GDFSState=false;
}
void T_GD::Draw(QPainter *painter, unsigned int nElapsed, double nDiploid)
{
    QPointF p_jyj1;
    QPointF p_jyj2;
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
    p12.setX(p12Const.x() * nDiploid);
    p12.setY(p12Const.y() * nDiploid);
    p34.setX(p34Const.x() * nDiploid);
    p34.setY(p34Const.y() * nDiploid);
    pz12.setX(pz12Const.x() * nDiploid);
    pz12.setY(pz12Const.y() * nDiploid);
    pz34.setX(pz34Const.x() * nDiploid);
    pz34.setY(pz34Const.y() * nDiploid);
    pCenter.setX(pCenterConst.x() * nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid);
    m_textRect.setRect(m_textRectConst.left() * nDiploid, m_textRectConst.top() * nDiploid, m_textRectConst.right() * nDiploid, m_textRectConst.bottom() * nDiploid);

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //绘制绝缘节 2021.1.11 BJT
    painter->setPen(QPen(Qt::white,1));
    p_jyj1.setX(p12.x());
    p_jyj1.setY(p12.y() - 5);
    p_jyj2.setX(p12.x());
    p_jyj2.setY(p12.y() + 5);
    painter->drawLine(p_jyj1, p_jyj2); //绘制轨道区段左侧绝缘节

    p_jyj1.setX(p34.x());
    p_jyj1.setY(p34.y() - 5);
    p_jyj2.setX(p34.x());
    p_jyj2.setY(p34.y() + 5);
    painter->drawLine(p_jyj1, p_jyj2); //绘制轨道区段右侧绝缘节

    //轨道区段光带绘制 2021.1.11 BJT
//    painter->setPen(QPen(getQDColor(),GDWIDTH));
    if(GDisPLCZ==true)
    {
        painter->setPen(QPen(Qt::cyan,GDWIDTH));
    }
    else
    {
        painter->setPen(QPen(getQDColor(),GDWIDTH));
    }
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

    //绘制轨道区段名称 2021.1.11 BJT
    if (getDisplayName())
    {
        font.setFamily("Times New Roman");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top(), getGDName());
    }
}

//成员变量封装函数实现
void T_GD::setType(unsigned int nType)
{
    m_nType = nType;
}
unsigned int T_GD::getType()
{
    return m_nType;
}
void T_GD::setGDType(QString str_type)
{
    if(str_type == "GD_QD")
    {
        m_nGDType = GD_QD;
    }
    else if(str_type == "JJ_QD")
    {
        m_nGDType = JJ_QD;
    }
    else if(str_type == "QD")
    {
        m_nGDType = QD;
    }
}
unsigned int T_GD::getGDType()
{
    return m_nGDType;
}
void T_GD::setGDName(QString name)
{
    m_strName = name;
}
QString T_GD::getGDName()
{
    return m_strName;
}
void T_GD::setZ(unsigned int nZ)
{
    m_nZ = nZ;
}
unsigned int T_GD::getZ()
{
    return m_nZ;
}
void T_GD::setSX(unsigned int nSX)
{
    m_nSX = nSX;
}
unsigned int T_GD::getSX()
{
    return m_nSX;
}
void T_GD::setCode(unsigned int code)
{
    m_nCode = code;
}
unsigned int T_GD::getCode()
{
    return m_nCode;
}
void T_GD::setDisplayName(bool nFlag)
{
    isDisPlayName = nFlag;
}
bool T_GD::getDisplayName()
{
    return isDisPlayName;
}
void T_GD::setQDColor(QColor color)
{
    gdColor = color;
}
QColor T_GD::getQDColor()
{
    return gdColor;
}
void T_GD::setLostTraNum(bool nFlag)
{
    isLostTrainNum = nFlag;
}
bool T_GD::getLostTraNum()
{
    return isLostTrainNum;
}
void T_GD::setLock(bool nFlag)
{
    isLock = nFlag;
}
bool T_GD::getLock()
{
    return isLock;
}
void T_GD::setGDFLBL(bool nFlag)
{
    isGDFLBL = nFlag;
}
bool T_GD::getGDFLBL()
{
    return isGDFLBL;
}
void T_GD::setStationID(int id)
{
    m_nStationID=id;
}
int T_GD::getStationID()
{
    return m_nStationID;
}
void T_GD::setp1(QPoint pt)
{
    p1.setX(pt.x());
    p1.setY(pt.y());
}
void T_GD::setp2(QPoint pt)
{
    p2.setX(pt.x());
    p2.setY(pt.y());
}
void T_GD::setp3(QPoint pt)
{
    p3.setX(pt.x());
    p3.setY(pt.y());
}
void T_GD::setp4(QPoint pt)
{
    p4.setX(pt.x());
    p4.setY(pt.y());
}
void T_GD::setp12(QPoint pt)
{
    p12.setX(pt.x());
    p12.setY(pt.y());
}
void T_GD::setp34(QPoint pt)
{
    p34.setX(pt.x());
    p34.setY(pt.y());
}
void T_GD::setpz12(QPoint pt)
{
    pz12.setX(pt.x());
    pz12.setY(pt.y());
}
void T_GD::setpz34(QPoint pt)
{
    pz34.setX(pt.x());
    pz34.setY(pt.y());
}
void T_GD::setpCenter(QPoint pt)
{
    pCenter.setX(pt.x());
    pCenter.setY(pt.y());
}
void T_GD::setTextRect(QRect rect)
{
    m_textRect=rect;
}
void T_GD::setGLB_QDleft(int glb)
{
    GLB_QDleft = glb;
}
int T_GD::getGLB_QDleft()
{
    return GLB_QDleft;
}
void T_GD::setGLB_QDright(int glb)
{
    GLB_QDright=glb;
}
int T_GD::getGLB_QDright()
{
    return GLB_QDright;
}
void T_GD::setIsSXDC(bool flag)
{
    isSXDC=flag;
}
bool T_GD::getIsSXDC()
{
    return isSXDC;
}
void T_GD::setIsZXGD(bool flag)
{
    isZXGD=flag;
}
bool T_GD::getIsZXGD()
{
    return isZXGD;
}
void T_GD::setIsCXGD(bool flag)
{
    isCXGD=flag;
}
bool T_GD::getIsCXGD()
{
    return isZXGD;
}
void T_GD::setIsJJGD(bool flag)
{
    isJJGD=flag;
}
bool T_GD::getIsJJGD()
{
    return isJJGD;
}
void T_GD::setGD_LCode(int code)
{
    GD_LCode=code;
}
int T_GD::getGD_LCode()
{
    return GD_LCode;
}
void T_GD::setGD_RCode(int code)
{
    GD_RCode=code;
}
int T_GD::getGD_RCode()
{
    return GD_RCode;
}
void T_GD::setCarrierFrequency(QString str)
{
    CarrierFrequency=str;
}
QString T_GD::getCarrierFrequency()
{
    return CarrierFrequency;
}
void T_GD::setDir_DMH(QString str)
{
    Dir_DMH=str;
}
QString T_GD::getDir_DMH()
{
    return Dir_DMH;
}
