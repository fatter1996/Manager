#ifndef STATIONDRAW_H
#define STATIONDRAW_H

#include <QWidget>
#include <BaseDataPro/stationjsj.h>
#include <JMShowPro/Th_kb.h>
namespace Ui
{
class CStationDraw;
}

class CStationDraw : public QWidget
{
    Q_OBJECT

public:
    explicit CStationDraw(QWidget *parent = nullptr);
    ~CStationDraw();
    void SetCurrentStation(CStation *pStation);
    CStationJSJ  *GetCurrentStation();
    void ManagerOntimer();
    void InitData();
    CTh_kb *pJM;
private:
    Ui::CStationDraw *ui;
    QTimer *UpdateTimer;

    CStationJSJ *pCurrentStation;


    QTransform m_Transform;
    long nElapsed = 0;
protected:

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void OnLButtonDown(QMouseEvent *event);
    void xhdRButtonDown(CXHD *pXHD);
private slots:
    void UPdate();
    void DealMenuAction(QAction* action);
    void DealSingleCMD(int funcode, int index);
};

#endif // STATIONDRAW_H
