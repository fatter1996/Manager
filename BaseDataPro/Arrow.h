#include "BaseData.h"

class CArrow   : public CBaseData
{
public:
    CArrow(void);
    ~CArrow(void);

    void SetState(BYTE nState);
    BOOL GetState(BYTE nState);
    int m_nState;
    void DRAW(QPainter *painter, float nDiploid, int nElapsed);
//private:
    QPoint p;
    //CBitmap m_btImage;

    int m_nSX;
};
