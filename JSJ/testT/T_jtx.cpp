#include "T_jtx.h"

T_jtx::T_jtx()
{
    p1.setX(-1);
    p1.setY(-1);
    p2.setX(-1);
    p2.setY(-1);
    p3.setX(-1);
    p3.setY(-1);
    p4.setX(-1);
    p4.setY(-1);
    pCenter.setX(-1);
    pCenter.setY(-1);
    m_nType=42;
    m_nStationID=0;

}

//成员功能函数实现
void T_jtx::JTXInit()
{
    p1Const.setX(p1.x());
    p1Const.setY(p1.y());
    p2Const.setX(p2.x());
    p2Const.setY(p2.y());
    p3Const.setX(p3.x());
    p3Const.setY(p3.y());
    p4Const.setX(p4.x());
    p4Const.setY(p4.y());
    pCenterConst.setX(pCenter.x());
    pCenterConst.setY(pCenter.y());
}
void T_jtx::Draw(QPainter *painter,int nElapsed,float nDiploid)
{
    //坐标变换 2021.1.11 BJT
    p1.setX(p1Const.x() * nDiploid);
    p1.setY(p1Const.y() * nDiploid);
    p2.setX(p2Const.x() * nDiploid);
    p2.setY(p2Const.y() * nDiploid);
    p3.setX(p3Const.x() * nDiploid);
    p3.setY(p3Const.y() * nDiploid);
    p4.setX(p4Const.x() * nDiploid);
    p4.setY(p4Const.y() * nDiploid);
    pCenter.setX(pCenterConst.x() * nDiploid);
    pCenter.setY(pCenterConst.y() * nDiploid);

    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::white,1));

    if (getType() == 41)
    {
        if(getJTXType() == 1)
        {
            painter->drawLine(p1, QPointF(p1.x()-5*nDiploid,p1.y()));
            painter->drawLine(p2, QPointF(p2.x()-5*nDiploid,p2.y()));
            painter->drawLine(p1, p2);
        }
        painter->drawLine(pCenter, QPointF(p1.x(),pCenter.y()));
        painter->drawLine(p4, QPointF(pCenter.x(),p4.y()));
    }
    else
    {
        if(getJTXType() == 1)
        {
            painter->drawLine(p1, QPointF(p1.x()+5*nDiploid,p1.y()));
            painter->drawLine(p2, QPointF(p2.x()+5*nDiploid,p2.y()));
            painter->drawLine(p1, p2);
        }
        painter->drawLine(pCenter, QPointF(p1.x(),pCenter.y()));
        painter->drawLine(p4, QPointF(pCenter.x(),p4.y()));
    }

}

//成员变量封装函数实现
void T_jtx::setType(unsigned int nType)
{
    m_nType = nType;
}
unsigned int T_jtx::getType()
{
    return m_nType;
}
void T_jtx::setJTXType(unsigned int nType)
{
    m_nJTXType = nType;
}
unsigned int T_jtx::getJTXType()
{
    return m_nJTXType;
}
void T_jtx::setJTXName(QString name)
{
    m_strName = name;
}
QString T_jtx::getJTXName()
{
    return m_strName;
}
void T_jtx::setStationID(int id)
{
    m_nStationID=id;
}
int T_jtx::getStationID()
{
    return m_nStationID;
}
void T_jtx::setp1(QPoint pt)
{
    p1.setX(pt.x());
    p1.setY(pt.y());
}
void T_jtx::setp2(QPoint pt)
{
    p2.setX(pt.x());
    p2.setY(pt.y());
}
void T_jtx::setp3(QPoint pt)
{
    p3.setX(pt.x());
    p3.setY(pt.y());
}
void T_jtx::setp4(QPoint pt)
{
    p4.setX(pt.x());
    p4.setY(pt.y());
}
void T_jtx::setpCenter(QPoint pt)
{
    pCenter.setX(pt.x());
    pCenter.setY(pt.y());
}
