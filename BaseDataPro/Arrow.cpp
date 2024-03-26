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
    if (m_nType == ARROW_LEFT)
    {
        if (GetState(STATE_YELLOW))
            image = "://Pictures/ALeftYellow.jpg";
        else if (GetState(STATE_RED))
            image = "://Pictures/ALeftRed.jpg";
        else if (GetState(STATE_GREEN))
            image = "://Pictures/ALeftGreen.jpg";
        else
            image = "://Pictures/ALeftGray.jpg";
    }
    else if (m_nType == ARROW_RIGHT)
    {
        if (GetState(STATE_YELLOW))
            image = "://Pictures/ARightYellow.jpg";
        else if (GetState(STATE_RED))
            image = "://Pictures/ARightRed.jpg";
        else if (GetState(STATE_GREEN))
            image = "://Pictures/ARightGreen.jpg";
        else
            image = "://Pictures/ARightGray.jpg";
    }
    draw_Pixmap(painter, image, p.x(), p.y(), 20, 15, nDiploid);
}

