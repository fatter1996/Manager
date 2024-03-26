#include "BaseData.h"
#define SIZE 4
class CBUTTON   : public CBaseData
{
public:
    CBUTTON(void);
    ~CBUTTON(void);
    void BUTTONInit();
    void DRAW(QPainter *painter, float nDiploid, int nElapsed);
    void setState(BYTE nState) ;

    int moveCursor(QPoint p);
    int m_nState;
    QRect m_rect;


    bool CBUTTON::getState(int nState);
private:
    //CBitmap m_btImage;// ���尴ť

};
