#include <QtDebug>
#include "T_YDQ.h"
#include "TGlobal.h"

T_YDQ::T_YDQ()
{
    m_nStationID=0;
    m_nType=0;
    m_strName="";
    m_nCode=0xffff;
    m_nNumDQ=0;
    m_nNumFQ=0;
    m_nNumCZ=0;
    m_nNumDY=0;
    m_nNumZN=0;
    m_strName_Record="";
    m_nFuncType=0;
    m_nGLQDNode=0xffff;
    m_nPYL=0;
    m_nYDQType=0;
    GLB_YDQ=0;
    p.setX(0);
    p.setY(0);
    isDisPlayName=true;        //是否显示名称
}

//成员功能函数实现
void T_YDQ::Draw(QPainter *painter, unsigned int nElapsed, double nDiploid)
{
    QFont font;

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);

    //绘制应答器  2021.3.17-BJT
    if(getYDQType() == YDQ_YY)
    {
        painter->setPen(QPen(Qt::white));
        painter->setBrush(QBrush(Qt::yellow));

        QPainterPath path;
        path.moveTo(p.x(), p.y());
        path.lineTo(p.x()-5, p.y()+10);
        path.lineTo(p.x()+5, p.y()+10);
        path.lineTo(p.x(), p.y());
        painter->drawPath(path);
        painter->fillPath(path, Qt::yellow);
    }
    else
    {
        painter->setPen(QPen(Qt::white));
        painter->setPen(Qt::white);
        static const QPointF points[3] = {QPointF(p.x(), p.y()), QPointF(p.x()-5, p.y()+10), QPointF(p.x()+5, p.y()+10)};
        painter->drawPolygon(points, 3);
    }

    //绘制应答器名称 2021.3.17 BJT
    if (true == getDisplayName())
    {
        font.setFamily("Times New Roman");
        font.setPointSize(8 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(getYDQName());
        painter->drawText(p.x()-rec.width()/2, p.y()+25, getYDQName());
    }
}

//成员变量封装函数实现
void T_YDQ::setStationID(int id)
{
    m_nStationID=id;
}
int T_YDQ::getStationID()
{
    return m_nStationID;
}
void T_YDQ::setType(int nType)
{
    m_nType = nType;
}
int T_YDQ::getType()
{
    return m_nType;
}
void T_YDQ::setYDQName(QString name)
{
    m_strName=name;
}
QString T_YDQ::getYDQName()
{
    return m_strName;
}
void T_YDQ::setCode(int code)
{
    m_nCode=code;
}
int T_YDQ::getCode()
{
    return m_nCode;
}
void T_YDQ::setNumDQ(int num)
{
    m_nNumDQ=num;
}
int T_YDQ::getNumDQ()
{
    return m_nNumDQ;
}
void T_YDQ::setNumFQ(int num)
{
    m_nNumFQ=num;
}
int T_YDQ::getNumFQ()
{
    return m_nNumFQ;
}
void T_YDQ::setNumCZ(int num)
{
    m_nNumCZ=num;
}
int T_YDQ::getNumCZ()
{
    return m_nNumCZ;
}
void T_YDQ::setNumDY(int num)
{
    m_nNumDY=num;
}
int T_YDQ::getNumDY()
{
    return m_nNumDY;
}
void T_YDQ::setNumZN(int num)
{
    m_nNumZN=num;
}
int T_YDQ::getNumZN()
{
    return m_nNumZN;
}
void T_YDQ::setName_Record(QString recordname)
{
    m_strName_Record=recordname;
}
QString T_YDQ::getName_Record()
{
    return m_strName_Record;
}
void T_YDQ::setFuncType(QString functype)
{
    if(functype == "Q")
    {
        m_nFuncType=YDQ_Q;
    }
    else if(functype == "FQ")
    {
        m_nFuncType=YDQ_FQ;
    }
    else if(functype == "DW")
    {
        m_nFuncType=YDQ_DW;
    }
    else if(functype == "JZ")
    {
        m_nFuncType=YDQ_JZ;
    }
    else if(functype == "CZ")
    {
        m_nFuncType=YDQ_CZ;
    }
    else if(functype == "FCZ")
    {
        m_nFuncType=YDQ_FCZ;
    }
    else if(functype == "FJZ")
    {
        m_nFuncType=YDQ_FJZ;
    }
    else if(functype == "DC")
    {
        m_nFuncType=YDQ_DC;
    }
    else if(functype == "DD")
    {
        m_nFuncType=YDQ_DD;
    }
    else if(functype == "ZJ")
    {
        m_nFuncType=YDQ_ZJ;
    }
    else if(functype == "RL")
    {
        m_nFuncType=YDQ_RL;
    }
    else if(functype == "ZX_R")
    {
        m_nFuncType=YDQ_ZX_R;
    }
    else if(functype == "YG_R")
    {
        m_nFuncType=YDQ_YG_R;
    }
    else if(functype == "ZC_3/2")
    {
        m_nFuncType=YDQ_ZC_3_2;
    }
    else if(functype == "YG_3/2")
    {
        m_nFuncType=YDQ_YG_3_2;
    }
}
int T_YDQ::getFuncType()
{
    return m_nFuncType;
}
void T_YDQ::setGLQDNode(int node)
{
    m_nGLQDNode=node;
}
int T_YDQ::getGLQDNode()
{
    return m_nGLQDNode;
}
void T_YDQ::setPYL(int pyl)
{
    m_nPYL=pyl;
}
int T_YDQ::getPYL()
{
    return m_nPYL;
}
void T_YDQ::setYDQType(QString type)
{
    if(type == "YDQ_WY")
    {
        m_nYDQType=YDQ_WY;
    }
    else if(type == "YDQ_YY")
    {
        m_nYDQType=YDQ_YY;
    }
}
int T_YDQ::getYDQType()
{
    return m_nYDQType;
}
void T_YDQ::setGLB_YDQ(int glb)
{
    GLB_YDQ=glb;
}
int T_YDQ::getGLB_YDQ()
{
    return GLB_YDQ;
}
void T_YDQ::setYDQp(QPoint pt)
{
    p=pt;
}
void T_YDQ::setDisplayName(bool nFlag)
{
    isDisPlayName = nFlag;
}
bool T_YDQ::getDisplayName()
{
    return isDisPlayName;
}
