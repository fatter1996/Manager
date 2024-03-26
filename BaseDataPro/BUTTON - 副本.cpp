#include "BUTTON.h"

CBUTTON::CBUTTON(void)
{
    m_nState = 0;
}

CBUTTON::~CBUTTON(void)
{
}

void CBUTTON::BUTTONInit()
{
    if (m_nType == BLOCK_BUTTON)
    {
        m_strName = "闭塞";
    }
    else if (m_nType == RECOVER_BUTTON)
    {
        m_strName = "复原";
    }
    else
    {
        m_strName = "事故";
    }
    m_rect.setRect(pCenter.x(), pCenter.y(), 15, 15);
}

void CBUTTON::setState(BYTE nState)
{
    m_nState = nState;
}

bool CBUTTON::getState(int nState)
{
    if ((int)(m_nState & nState) > 0)  //判断组件状态是否包含所给状态
        return true;
    else
        return false;
}
void CBUTTON::DRAW(QPainter *painter, float nDiploid, int nElapsed)
{
    QFont font;
    QString image = "";
    if (m_nType == BLOCK_BUTTON)
    {
        if (getState(ALL)) //绘制被按下的按钮
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //绘制抬起的按钮
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    else if (m_nType == RECOVER_BUTTON)
    {
        if (getState(ALL)) //绘制被按下的按钮
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //绘制抬起的按钮
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    else if (m_nType == FAULT_BUTTON)
    {
        if (getState(ALL)) //绘制被按下的按钮
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //绘制抬起的按钮
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", m_rect.topLeft().x(), m_rect.topLeft().x(), 15, 15, nDiploid);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色
    painter->drawText(m_textRect, m_strName);
}

int CBUTTON::moveCursor(QPoint p)
{
    if (m_rect.contains(p))
        return 2;
    if (m_textRect.contains(p))
        return 3;
    return 0;
}

