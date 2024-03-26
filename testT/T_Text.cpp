#include "T_Text.h"
#include "TGlobal.h"
T_Text::T_Text()
{
    m_textRect.setRect(0, 0, 0, 0);          //轨道区段文本区域
    m_textRectConst.setRect(0, 0, 0, 0);    //预留站场缩放数据存储结构
    m_strName = "";        //文本内容
    m_strDCName = "";         //关联道岔名称
    m_nQDNode = 0xffff;
    m_nSize = 11;     //文本大小
    textColor = Qt::white;       //文本显示颜色
    m_nTextType = TEXT;
    m_nStationID=0;

    IsQDTextShowFlag = true;
}

//成员功能函数实现
void T_Text::TextInit()
{
    if(m_nSize == 12)
        m_nSize = 10;
    m_textRect.setRect(m_textRect.left(),m_textRect.top()+15,m_textRect.right(),m_textRect.bottom()+15);
    m_textRectConst.setRect(m_textRect.left(),m_textRect.top(),m_textRect.right(),m_textRect.bottom());
}
void T_Text::Draw(QPainter *painter, unsigned int nElapsed, double nDiploid)
{
    QFont font;
    //坐标变换 2021.1.11 BJT
    m_textRect.setRect(m_textRectConst.left() * nDiploid, m_textRectConst.top() * nDiploid,
                       m_textRectConst.right() * nDiploid, m_textRectConst.bottom() * nDiploid);

    if (STATIONNAMETEXT == getTextType())
    {
        font.setFamily("Times New Roman");
        font.setPointSize(getTextSize() * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::green);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top(), getTextName());
    }
    else if (DCQDNAMETEXT == getTextType())
    {
        if(IsQDTextShowFlag)
        {
            font.setFamily("Times New Roman");
            font.setPointSize(getTextSize() * nDiploid);//字号
            font.setItalic(false);//斜体
            painter->setFont(font);//设置字体
            painter->setPen(Qt::white);//设置画笔颜色
            //绘制文本
            painter->drawText(m_textRect.left(), m_textRect.top(), getTextName());
        }
    }
    else
    {
        font.setFamily("Times New Roman");
        font.setPointSize(getTextSize() * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        painter->drawText(m_textRect.left(), m_textRect.top(), getTextName());
    }
}

//成员变量封装函数实现
void T_Text::setTextName(QString name)
{
    m_strName = name;
}
QString T_Text::getTextName()
{
    return m_strName;
}
void T_Text::setGLDCName(QString name)
{
    m_strDCName = name;
}
QString T_Text::getGLDCName()
{
    return m_strDCName;
}
void T_Text::setGLQDNode(unsigned int node)
{
    m_nQDNode = node;
}
unsigned int T_Text::getGLQDNode()
{
    return m_nQDNode;
}
void T_Text::setTextSize(unsigned int size)
{
    m_nSize = size;
}
unsigned int T_Text::getTextSize()
{
    return m_nSize;
}
void T_Text::setTextColor(QString strColor)
{
    if ("WHITE" == strColor)
    {
        textColor = Qt::white;
    }
    else if ("GREEN" == strColor)
    {
        textColor = Qt::green;
    }
    else if ("BLUE" == strColor)
    {
        textColor = Qt::blue;
    }
    else if ("YELLOW" == strColor)
    {
        textColor = Qt::yellow;
    }
    else if ("BLACK" == strColor)
    {
        textColor = Qt::black;
    }
    else if ("RED" == strColor)
    {
        textColor = Qt::red;
    }
    else if ("GRAY" == strColor)
    {
        textColor = Qt::gray;
    }
    else
    {
        textColor = Qt::white;
    }
}
QColor T_Text::getTextColor()
{
    return textColor;
}
void T_Text::setTextType(QString type)
{
    if(type == "STATIONNAMETEXT")
    {
        m_nTextType = STATIONNAMETEXT;
    }
    else if(type == "DCQDNAMETEXT")
    {
        m_nTextType = DCQDNAMETEXT;
    }
    else if(type == "TEXT")
    {
        m_nTextType = TEXT;
    }
}
unsigned int T_Text::getTextType()
{
    return m_nTextType;
}
void T_Text::setStationID(int id)
{
    m_nStationID=id;
}
int T_Text::getStationID()
{
    return m_nStationID;
}
void T_Text::setTextRect(QRect rect)
{
    m_textRect=rect;
}
