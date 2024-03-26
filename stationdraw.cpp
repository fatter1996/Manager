#include "stationdraw.h"
#include "ui_stationdraw.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <qmenu.h>
CStationDraw::CStationDraw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CStationDraw)
{
    ui->setupUi(this);
    UpdateTimer = new QTimer(this);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(UPdate()));
    UpdateTimer->start(500);//开启刷新计时器
    pCurrentStation = nullptr;
    setMouseTracking(true);
}
CStationDraw::~CStationDraw()
{
    delete ui;
}
void CStationDraw::SetCurrentStation(CStation *pStation)
{
    pCurrentStation = (CStationJSJ *)pStation;
}
CStationJSJ *CStationDraw::GetCurrentStation()
{
    return pCurrentStation;
}
void CStationDraw::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen; //画笔
    pen.setColor(QColor(0, 0, 0));
    QBrush brush(QColor(0, 0, 0, 255)); //画刷


    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(0, 0, width(), height()); //绘制矩形

    m_Transform = painter.transform();

    if (pCurrentStation != nullptr)
    {
        pCurrentStation->drawStation(&painter, nElapsed, 1, 0x55);
        pJM->Draw(&painter, nElapsed, 1);
    }
    painter.end();
}
void CStationDraw::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    if (pCurrentStation == nullptr)
        return;
    int size = pCurrentStation->DevArray.size();
    int nMoveCursor = -1;
    if (pJM->moveCursor(p) > 0)
    {
        nMoveCursor = 1;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            nMoveCursor = pCurrentStation->DevArray[i]->moveCursor(p);
            if (nMoveCursor > 0)
                break;
        }
    }
    if (nMoveCursor > 0)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

void CStationDraw::mousePressEvent(QMouseEvent *event)
{
    if (pCurrentStation == nullptr)
        return;
    if (event->button() == Qt::LeftButton)
    {
        OnLButtonDown(event);
    }
    else if (event->button() == Qt::RightButton)
    {
        pCurrentStation->OnRButtonDown(event);
    }
}

void CStationDraw::OnLButtonDown(QMouseEvent *event)
{
    QPoint pt;
    pt = event->pos();
    if (pJM->moveCursor(pt))
    {
        pJM->OnLButtonDown(pt);
    }
    else
    {
        if (pCurrentStation != nullptr)
            pCurrentStation->DevLButtonDown(event->pos(), pJM);
    }
}

void CStationDraw::ManagerOntimer()
{
    pJM->timePro();
}
void CStationDraw::InitData()
{
    connect(pJM, SIGNAL(signal_SendFunBtn(int, int)), this, SLOT(DealSingleCMD(int, int)));
    connect(pJM, SIGNAL(signal_ShowJM(int)), pCurrentStation, SLOT(setShow_Menu(int)));
}
void CStationDraw::UPdate()
{
    this->update();
    nElapsed++;
}
void CStationDraw::DealMenuAction(QAction *action)
{
    Q_UNUSED(action);
}
void CStationDraw::DealSingleCMD(int funcode, int index)
{
    index = -1;
    if (pCurrentStation)
        pCurrentStation->PackOperationMessage(funcode, funcode, 0xfff);
}
