#include "BaseData.h"

CBaseData::CBaseData(QObject *parent) : QObject(parent)
{
    pCenter.setX(1);
    pCenter.setY(1);         //设备中心点
    pCenterConst.setX(1);
    pCenterConst.setY(1);    //设备中心点
    m_nType = 1;              //设备类型
    m_strName = "";           //设备名称
    m_nSX = 0;                //上下行咽喉
    m_nCode = 0xffff;         //设备编号
    m_textRect.setRect(1, 1, 1, 1);     //设备名称文本区域
    m_textRectConst.setRect(1, 1, 1, 1); //设备名称文本区域
    m_nStationID = 0;         //所属站ID
    m_nBelongToTCC = 0;       //所属TCC ID
    m_nBelongToRBC = 0;       //所属RBC ID
    isDisPlayName = true;     //是否显示名称
    m_bToolTip = false;       //是否显示鼠标提示信息
    m_strToolTip = "";        //鼠标提示信息
}

void CBaseData::setGZType(int gzAttr)
{
    GZType |= gzAttr;
}

bool CBaseData::getGZType(int gzAttr)
{
    bool result = false;
    ((GZType & gzAttr) == gzAttr) ? result = true : result = false;
    return result;
}

void CBaseData::clearGZType(int gzAttr)
{
    GZType &= (~gzAttr);
}

void CBaseData::clearAllGZ()
{
    GZType = 0;
}

void CBaseData::Draw_ToolTip(QPainter *painter, double nDiploid)
{
}
void CBaseData::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx)
{
}
void CBaseData::setDevStateToSafe()
{

}
void CBaseData::setVollover(QPoint pt_Base)
{

}
unsigned int CBaseData::getDevType()
{
    return 0;
}
int CBaseData::moveCursor(QPoint p)
{
    return 0;
}
void CBaseData::draw_Pixmap(QPainter *painter, QString picName, int x, int y, int width, int heigh, int nDiploid)
{
    QPixmap pix;
    pix.load(picName);
    painter->drawPixmap(x, y, width, heigh, pix);
}
void CBaseData::setCenterPt(QPointF pt)
{
    pCenter = pt;
    pCenterConst = pt;
}
QPointF CBaseData::getCenterPt()
{
    return pCenterConst;
}
void CBaseData::setType(unsigned int nType)
{
    m_nType = nType;
}
unsigned int CBaseData::getType()
{
    return m_nType;
}
void CBaseData::setName(QString name)
{
    m_strName = name;
}
QString CBaseData::getName()
{
    return m_strName;
}
void CBaseData::setSX(unsigned int nSX)
{
    m_nSX = nSX;
}
unsigned int CBaseData::getSX()
{
    return m_nSX;
}
void CBaseData::setCode(unsigned int code)
{
    m_nCode = code;
}
unsigned int CBaseData::getCode()
{
    return m_nCode;
}
void CBaseData::setTextRect(QRectF rect)
{
    m_textRect = rect;
    m_textRectConst = rect;
}
QRectF CBaseData::getTextRect()
{
    return m_textRect;
}
void CBaseData::setStationID(unsigned int id)
{
    m_nStationID = id;
}
unsigned int CBaseData::getStationID()
{
    return m_nStationID;
}
void CBaseData::setBelongToTCC(unsigned int id)
{
    m_nBelongToTCC = id;
}
unsigned int CBaseData::getBelongToTCC()
{
    return m_nBelongToTCC;
}
void CBaseData::setBelongToRBC(unsigned int id)
{
    m_nBelongToRBC = id;
}
unsigned int CBaseData::getBelongToRBC()
{
    return m_nBelongToRBC;
}
void CBaseData::setDisplayName(bool nFlag)
{
    isDisPlayName = nFlag;
}
bool CBaseData::getDisplayName()
{
    return isDisPlayName;
}
void CBaseData::setToolTipFlag(bool flag)
{
    m_bToolTip = flag;
}
bool CBaseData::getToolTipFlag()
{
    return m_bToolTip;
}
void CBaseData::setToolTipStr(QString str)
{
    m_strToolTip = str;
}
QString CBaseData::getToolTipStr()
{
    return m_strToolTip;
}
