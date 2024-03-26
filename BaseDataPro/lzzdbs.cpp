#include "lzzdbs.h"

CLZZDBS::CLZZDBS()
{
    ZFZAN = DOWN;
    FCFZAN = DOWN;
    JCFZAN = DOWN;
    XHZCJState = DOWN;
    FCANTime = 13;
    FJ = DOWN;
    m_nZDBSANtime = 13;
    FaCheArrow = 0xcc;
}

void CLZZDBS::InitCoordinate()
{
    unsigned int nANdistance = 80;
    ButtonStr_ button;
    InitState();
    pTimer = new QTimer(this);
    pTimer->setInterval(1000);
    bool ll = connect(pTimer, SIGNAL(timeout()), this, SLOT(ontimer()));
    pTimer->start();

    button.name = "邻站发车进路";
    button.AnRect.setRect(BasePt.x(), BasePt.y(), 15, 15);
    v_buttonArray.push_back(button);

    button.name = "邻站总辅助";
    button.AnRect.setRect(BasePt.x() + nANdistance, BasePt.y(), 15, 15);
    v_buttonArray.push_back(button);

    button.name = "邻站发车辅助";
    button.AnRect.setRect(BasePt.x() + 2 * nANdistance, BasePt.y(), 15, 15);
    v_buttonArray.push_back(button);

    button.name = "邻站接车辅助";
    button.AnRect.setRect(BasePt.x() + 3 * nANdistance, BasePt.y(), 15, 15);
    v_buttonArray.push_back(button);

    if (m_nZDBSisNX)
    {
        button.name = "邻站允许改方";
        button.AnRect.setRect(BasePt.x() + 4 * nANdistance, BasePt.y(), 15, 15);
        v_buttonArray.push_back(button);
    }

    ArriowPoint.setX(BasePt.x() - nANdistance / 2);
    ArriowPoint.setY(BasePt.y());
}

void CLZZDBS::InitState()
{
    if (1 == m_nZDBSisNX)
    {
        FaCheEnabled = DOWN;
        LZFaCheEnabled = UP;
        FaCheArrow = 0xaa;
        LZFaCheArrow = 0xcc;
    }
    else
    {
        FaCheEnabled = UP;
        LZFaCheEnabled = DOWN;
        FaCheArrow = 0xcc;
        LZFaCheArrow = 0xaa;
    }
    LZQJKXState = UP;
    ZFZAN = DOWN;
    FCFZAN = DOWN;
    JCFZAN = DOWN;
    GFANState = DOWN;
    XHZCJState = UP;
    LZFCFZANflag = DOWN;
    LZJCFZANflag = DOWN;
    LZGFANState = DOWN;
    LZXHZCJState = UP;
    for (int i = 0; i < v_buttonArray.size(); i++)
    {
        v_buttonArray[i].state = DOWN;
    }
}
void CLZZDBS::Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx)
{
    QFont font;
    QRect FrameRect;
    float nANdistance = 100 * nDiploid;
    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    painter->setPen(Qt::white);//设置画笔颜色
    QFontMetrics fm = painter->fontMetrics();
    for (QVector<ButtonStr_>::Iterator it = v_buttonArray.begin(); it < v_buttonArray.end(); it++)
    {
        if (it->state == UP)
        {
            painter->setBrush(QBrush(Qt::yellow)); //添加画刷
        }
        else
            painter->setBrush(QBrush(Qt::gray)); //添加画刷
        painter->drawRect(it->AnRect);

        painter->drawText(it->AnRect.left() + 7 * nDiploid - fm.width(it->name) / 2, it->AnRect.bottom() + 12 * nDiploid, it->name);
    }
    QPointF pointsR[7] =
    {
        QPointF(ArriowPoint.x(), ArriowPoint.y() + 4),
        QPointF(ArriowPoint.x(), ArriowPoint.y() + 11),
        QPointF(ArriowPoint.x() - 15, ArriowPoint.y() + 11),
        QPointF(ArriowPoint.x() - 15, ArriowPoint.y() + 15),
        QPointF(ArriowPoint.x() - 22.5, ArriowPoint.y() + 7.5),
        QPointF(ArriowPoint.x() - 15, ArriowPoint.y()),
        QPointF(ArriowPoint.x() - 15, ArriowPoint.y() + 4),
    };
    QPoint pt(ArriowPoint.x() - nANdistance / 2, ArriowPoint.y());
    QPointF pointsL[7] =
    {
        QPointF(pt.x(), pt.y() + 4),
        QPointF(pt.x(), pt.y() + 11),
        QPointF(pt.x() + 15, pt.y() + 11),
        QPointF(pt.x() + 15, pt.y() + 15),
        QPointF(pt.x() + 22.5, pt.y() + 7.5),
        QPointF(pt.x() + 15, pt.y()),
        QPointF(pt.x() + 15, pt.y() + 4),
    };
    if (m_nZDBSsx)
    {
        if (FaCheArrow == 0xaa)
            painter->setBrush(QBrush(Qt::yellow)); //添加画刷
        else
            painter->setBrush(QBrush(Qt::gray)); //添加画刷
        QPointF points[7];
        for (int i = 0; i < 7; i++)
            points[i] = pointsR[i];
        painter->drawPolygon(points, 7);

        if (FaCheArrow == 0xaa)
            painter->setBrush(QBrush(Qt::gray)); //添加画刷
        else
            painter->setBrush(QBrush(Qt::green)); //添加画刷

        for (int i = 0; i < 7; i++)
            points[i] = pointsL[i];
        painter->drawPolygon(points, 7);
    }
    else
    {
        if (FaCheArrow == 0xaa)
            painter->setBrush(QBrush(Qt::yellow)); //添加画刷
        else
            painter->setBrush(QBrush(Qt::gray)); //添加画刷
        QPointF points[7];
        for (int i = 0; i < 7; i++)
            points[i] = pointsL[i];
        painter->drawPolygon(points, 7);

        if (FaCheArrow == 0xaa)
            painter->setBrush(QBrush(Qt::gray)); //添加画刷
        else
            painter->setBrush(QBrush(Qt::green)); //添加画刷

        for (int i = 0; i < 7; i++)
            points[i] = pointsR[i];
        painter->drawPolygon(points, 7);
    }
    painter->setBrush(QBrush(Qt::transparent)); //添加画刷
    FrameRect.setRect(ArriowPoint.x() - nANdistance / 2, BasePt.y() - 10, (v_buttonArray.size()) * nANdistance, 50);
    painter->drawRect(FrameRect);
    painter->setBrush(QBrush(Qt::black));
    painter->fillRect(BasePt.x(), BasePt.y() - 16, fm.width(Name), 12, QBrush(Qt::black));
    painter->drawText(BasePt.x(), BasePt.y() - 8, Name);
}
unsigned int CLZZDBS::getDevType()
{
    return Dev_LZZDBS;
}

int CLZZDBS::moveCursor(QPoint p)
{
    for (int i = 0; i < v_buttonArray.size(); i++)
    {
        if (v_buttonArray[i].AnRect.contains(p))
            return i + 1;
    }
    return 0;
}
void CLZZDBS::DealwithClicked(int index)
{
    if (index < v_buttonArray.size())
    {
        if (index == 0 || index == 1)
        {
            if (v_buttonArray[index].state == UP)
                v_buttonArray[index].state = DOWN;
            else if (v_buttonArray[index].state == DOWN)
                v_buttonArray[index].state = UP;
        }
        else if (index == 4)
        {
            if (v_buttonArray[index].state == UP)
                v_buttonArray[index].state = DOWN;
            else if ((v_buttonArray[index].state == DOWN) && (LZXHZCJState == UP))
                v_buttonArray[index].state = UP;
        }
        else if (index == 2 || index == 3)
        {
            if (v_buttonArray[1].state == UP)
            {
                v_buttonArray[index].state = UP;
                v_buttonArray[index].anTime = m_nZDBSANtime;
            }
        }
    }
}
void CLZZDBS::ontimer()
{
    if (v_buttonArray.size() >= 4)
    {
        if (v_buttonArray[1].state == DOWN)
        {
            v_buttonArray[2].state = DOWN;
            v_buttonArray[3].state = DOWN;
        }
        if (v_buttonArray[2].state == UP)
        {
            v_buttonArray[2].anTime--;
            if (v_buttonArray[2].anTime == 0)
                v_buttonArray[2].state = DOWN;
        }
        if (v_buttonArray[3].state == UP)
        {
            v_buttonArray[3].anTime--;
            if (v_buttonArray[3].anTime == 0)
                v_buttonArray[3].state = DOWN;
        }
        ZFZAN = v_buttonArray[1].state;
        FCFZAN = v_buttonArray[2].state;
        JCFZAN = v_buttonArray[3].state;
        XHZCJState = v_buttonArray[0].state == UP ? DOWN : UP;
        if (v_buttonArray.size() > 4)
            GFANState = v_buttonArray[4].state;
//        if (FJ == UP)
//            FaCheArrow = 0xaa;
//        else
//            FaCheArrow = 0xcc;
    }
    if (1 == m_nZDBSisNX)
    {
        if ((XHZCJState == DOWN) && (FaCheEnabled == UP) && (GFANState == UP))
        {
            FaCheArrow = 0xcc;
        }
        else if ((XHZCJState == DOWN) && (FaCheEnabled == DOWN) && (GFANState == UP) && (LZXHZCJState == UP))
        {
            FaCheArrow = 0xcc;
            FaCheEnabled = UP;
        }
        else if (FaCheEnabled == DOWN && (LZFaCheArrow == 0xcc || LZFaCheArrow == 0xdd))
        {
            FaCheArrow = 0xaa;
        }
        else if (FaCheArrow == 0xcc)
        {
            FaCheArrow = 0xcc;
        }
    }
    if (0 == m_nZDBSisNX)
    {
        if ((XHZCJState == DOWN) && (FaCheArrow == 0xcc || ((FaCheArrow == 0xaa)  && (LZXHZCJState == UP) && (LZGFANState == DOWN) && (LZQJKXState == UP))))
        {
            FaCheArrow = 0xcc;
        }
        else if (LZFaCheArrow == 0xcc || LZFaCheArrow == 0xdd)
        {
            FaCheArrow = 0xaa;
        }
        else if ((LZFaCheArrow == 0xcc || LZFaCheArrow == 0xdd) && JCFZAN == UP && LZFCFZANflag == UP)
        {
            FaCheArrow = 0xaa;
        }
        else if (FaCheArrow == 0xcc)
        {
            FaCheArrow = 0xcc;
        }
        else if (FaCheArrow == 0xdd)
            FaCheArrow = 0xdd;
        else if (FaCheArrow == 0xaa)
            FaCheArrow = 0xaa;
    }
    if (0 == m_nZDBSisNX)
    {
        if ((FaCheArrow == 0xdd) || (FaCheArrow == 0xcc))
            FaCheEnabled = UP;
        else
            FaCheEnabled = DOWN;
    }
    if (1 == m_nZDBSisNX)
    {
        if ((LZFaCheArrow == 0xdd || LZFaCheArrow == 0xcc) && (FaCheArrow == 0xcc) && (XHZCJState == UP))
        {
            FaCheEnabled = DOWN;
            GFANState = DOWN;
        }
        if ((LZXHZCJState == UP && (LZQJKXState == DOWN)) && (JCFZAN == UP && LZFCFZANflag == UP))
        {
            FaCheEnabled = DOWN;
            FaCheArrow = 0xaa;
            GFANState = DOWN;
        }
    }
    if (FCFZAN == UP && LZJCFZANflag == UP)
    {
        FaCheArrow = 0xcc;
        FaCheEnabled = UP;
    }
}
