#include "Arrow.h"
CArrow::CArrow(void)
{
}
CArrow::~CArrow(void)
{
}

void CArrow::SetState(BYTE nState)
{
    m_nState = nState;
}

BOOL CArrow::GetState(BYTE nState)
{
    if (m_nState == nState)
        return TRUE;
    else
        return FALSE;
}

void CArrow::DRAW(QPainter *painter, float nDiploid, int nElapsed)
{
    QString image = "";
    if (m_nType == ARROW_LEFT)//����ļ�ͷ
    {

    }
    else if (m_nType == ARROW_RIGHT) //���ҵļ�ͷ
    {

    }
    draw_Pixmap(painter, image, m_textRect.topLeft().x(), m_textRect.topLeft().x(), 15, 15, nDiploid);
}

