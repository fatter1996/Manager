#include "cqd.h"

CQD::CQD()
{
    m_nChildNum=0;
    for(int i=0;i<4;i++)
    {
        m_nChild[i]=0xffff;
    }
    QD_LCode=0xffff;
    QD_RCode=0xffff;
    FQJ=DOWN;
    DMH_State=DMH_HU;  //0xffff
    DMH_ShowType=1;
    isDisDPZPFlag=false;
}

void CQD::Draw(QPainter *painter, long nElapsed, double nDiploid,QPoint offset)
{
    if(getIsDisDPZPFlag() == false)
    {
        return;
    }
    //坐标计算
    QFont font;
    pt.setX(ptConst.x()*nDiploid+offset.x()*nDiploid);
    pt.setY(ptConst.y()*nDiploid+offset.y()*nDiploid);
    m_DrawRect.setLeft(m_DrawRectConst.left()+offset.x()*nDiploid);
    m_DrawRect.setTop(m_DrawRectConst.top()+offset.y()*nDiploid);
    m_DrawRect.setRight(m_DrawRectConst.right()+offset.x()*nDiploid);
    m_DrawRect.setBottom(m_DrawRectConst.bottom()+offset.y()*nDiploid);
    QString str_Dmh="";
    QColor color1=Qt::black;
    QColor color2=Qt::black;
    QRect rect1;
    QRect rect2;
    rect1.setLeft(m_DrawRect.left());
    rect1.setTop(m_DrawRect.top());
    rect1.setRight(m_DrawRect.right());
    rect1.setBottom(m_DrawRect.bottom()-5);
    rect2.setLeft(m_DrawRect.left());
    rect2.setTop(m_DrawRect.top()+5);
    rect2.setRight(m_DrawRect.right());
    rect2.setBottom(m_DrawRect.bottom());
    if(0xffff == DMH_State)
    {
        return;
    }
    if(DMH_ShowType == 1)  //字母显示
    {
        font.setFamily("Times New Roman");
        font.setPointSize(8 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        if(getDMHState() == DMH_H)
        {
            painter->setPen(Qt::red);//设置画笔颜色
            str_Dmh="H";
        }
        else if(getDMHState() == DMH_HB)
        {
            painter->setPen(Qt::red);//设置画笔颜色
            str_Dmh="HB";
        }
        else if(getDMHState() == DMH_HU)
        {
            painter->setPen(Qt::red);//设置画笔颜色
            str_Dmh="HU";
        }
        else if(getDMHState() == DMH_UU)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="UU";
        }
        else if(getDMHState() == DMH_UUS)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="UUS";
        }
        else if(getDMHState() == DMH_U2)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="U2";
        }
        else if(getDMHState() == DMH_U2S)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="U2S";
        }
        else if(getDMHState() == DMH_U)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="U";
        }
        else if(getDMHState() == DMH_LU)
        {
            painter->setPen(Qt::yellow);//设置画笔颜色
            str_Dmh="LU";
        }
        else if(getDMHState() == DMH_L)
        {
            painter->setPen(Qt::green);//设置画笔颜色
            str_Dmh="L";
        }
        else if(getDMHState() == DMH_L2)
        {
            painter->setPen(Qt::green);//设置画笔颜色
            str_Dmh="L2";
        }
        else if(getDMHState() == DMH_L3)
        {
            painter->setPen(Qt::green);//设置画笔颜色
            str_Dmh="L3";
        }
        else if(getDMHState() == DMH_L4)
        {
            painter->setPen(Qt::green);//设置画笔颜色
            str_Dmh="L4";
        }
        else if(getDMHState() == DMH_L5)
        {
            painter->setPen(Qt::green);//设置画笔颜色
            str_Dmh="L5";
        }
        else if(getDMHState() == DMH_JC)
        {
            painter->setPen(Qt::white);//设置画笔颜色
            str_Dmh="JC";
        }
        else if(getDMHState() == DMH_SP)
        {
            painter->setPen(Qt::white);//设置画笔颜色
            str_Dmh="SP";
        }
        painter->drawText(pt.x(), pt.y(), str_Dmh); //绘制文本
    }
    else if(DMH_ShowType == 0)  //条状方框显示
    {
        font.setFamily("Times New Roman");
        font.setPointSize(8 * nDiploid);//字号
        font.setItalic(false);//斜体
        font.setBold(true);//加粗
        painter->setFont(font);//设置字体
        if(getDMHState() == DMH_H)
        {
            color1=Qt::red;
            color2=Qt::red;
            str_Dmh="H";
        }
        else if(getDMHState() == DMH_HB)
        {
            color1=Qt::red;
            color2=Qt::white;
            str_Dmh="HB";
        }
        else if(getDMHState() == DMH_HU)
        {
            color1=Qt::red;
            color2=Qt::yellow;
            str_Dmh="HU";
        }
        else if(getDMHState() == DMH_UU)
        {
            color1=Qt::yellow;
            color2=Qt::yellow;
            str_Dmh="UU";
        }
        else if(getDMHState() == DMH_UUS)
        {
            color1=Qt::yellow;
            if((nElapsed%2)==0)
            {
                color2=Qt::black;
            }
            else
            {
               color2=Qt::yellow;
            }
            str_Dmh="UUS";
        }
        else if(getDMHState() == DMH_U2)
        {
            color1=Qt::yellow;
            color2=Qt::yellow;
            str_Dmh="U2";
        }
        else if(getDMHState() == DMH_U2S)
        {
            color1=Qt::black;
            if((nElapsed%2)==0)
            {
                color2=Qt::black;
            }
            else
            {
               color2=Qt::yellow;
            }
            str_Dmh="U2S";
        }
        else if(getDMHState() == DMH_U)
        {
            color1=Qt::yellow;
            color2=Qt::yellow;
            str_Dmh="U";
        }
        else if(getDMHState() == DMH_LU)
        {
            color1=Qt::green;
            color2=Qt::yellow;
            str_Dmh="LU";
        }
        else if(getDMHState() == DMH_L)
        {
            color1=Qt::green;
            color2=Qt::green;
            str_Dmh="L";
        }
        else if(getDMHState() == DMH_L2)
        {
            color1=QColor(0, 233, 0);//颜色
            color2=QColor(0, 233, 0);
            str_Dmh="L2";
        }
        else if(getDMHState() == DMH_L3)
        {
            color1=QColor(0, 190, 0);//颜色
            color2=QColor(0, 190, 0);
            str_Dmh="L3";
        }
        else if(getDMHState() == DMH_L4)
        {
            color1=QColor(0, 159, 0);//颜色
            color2=QColor(0, 159, 0);
            str_Dmh="L4";
        }
        else if(getDMHState() == DMH_L5)
        {
            color1=QColor(0, 127, 0);//颜色
            color2=QColor(0, 127, 0);
            str_Dmh="L5";
        }
        else if(getDMHState() == DMH_JC)
        {
            color1=Qt::black;
            color2=Qt::black;
            str_Dmh="JC";
        }
        else if(getDMHState() == DMH_SP)
        {
            color1=Qt::black;
            color2=Qt::black;
            str_Dmh="SP";
        }
        painter->setPen(QPen(Qt::gray, 0.3));
        painter->setBrush(color1);
        painter->drawRect(rect1);
        painter->setBrush(color2);
        painter->drawRect(rect2);
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(str_Dmh);
        painter->setPen(Qt::black);//设置画笔颜色
        painter->drawText((rect1.left()+rect1.right())/2-rec.width()/2, rect2.bottom(), str_Dmh); //绘制文本
    }
}

//成员变量封装函数
void CQD::setFQJ(int state)
{
    FQJ=state;
}
int CQD::getFQJ()
{
    return FQJ;
}
void CQD::setDMHState(int state)
{
    DMH_State=state;
}
int CQD::getDMHState()
{
    return DMH_State;
}
void CQD::setPt(QPoint p)
{
    pt=p;
    ptConst=p;
}
void CQD::setDMHShowType(int type)
{
    DMH_ShowType=type;
}
int CQD::getDMHShowType()
{
    return DMH_ShowType;
}
void CQD::setDrawRect(QRect rect)
{
    m_DrawRect=rect;
    m_DrawRectConst=rect;
}
void CQD::setIsDisDPZPFlag(bool flag)
{
    isDisDPZPFlag=flag;
}
bool CQD::getIsDisDPZPFlag()
{
    return isDisDPZPFlag;
}
