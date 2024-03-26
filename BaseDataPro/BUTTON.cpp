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
    m_textRect.setRect(pCenter.x() - 10, pCenter.y() - 7 - SIZE - 1, m_strName.length() * 16, 12);
}

void CBUTTON::setState(BYTE nState)
{
    m_nState = nState;
}

bool CBUTTON::getState(int nState)
{
    if ((int)(m_nState & nState) > 0)
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
        if (getState(ALL))
        {
            image = "://bmp/btndownGreen.bmp"; //up
        }
        else
        {
            image = "://bmp/btnupGreen.bmp";
        }
    }
    else if (m_nType == RECOVER_BUTTON)
    {
        if (getState(ALL))
        {
            image = "://bmp/btndownYellow.bmp"; //up
        }
        else
        {
            image = "://bmp/btnupYellow.bmp";
        }
    }
    else if (m_nType == FAULT_BUTTON)
    {
        if (getState(ALL))
        {
            image = "://bmp/btndownRed.bmp"; //up
        }
        else
        {
            image = "://bmp/btnupRed.bmp";
        }
    }
    draw_Pixmap(painter, image, m_rect.topLeft().x(), m_rect.topLeft().y(), 15, 15, nDiploid);

    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);
    font.setItalic(false);
    painter->setFont(font);
    painter->setPen(Qt::white);
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

