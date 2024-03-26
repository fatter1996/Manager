#include "jsj_stationheart.h"
#include<QDebug>
jsj_Stationheart::jsj_Stationheart(int StationId, QObject *parent) : QObject(parent)
{

    StationId_ = StationId;
    CurTimer = new QTimer(this);
    m_lastTime = QDateTime::currentDateTime();
    CurTimer->setInterval(2000);
     CurTimer->start();
    connect(CurTimer, SIGNAL(timeout()), this, SLOT(checkHeartbeat()));
}
void jsj_Stationheart::checkHeartbeat()
{
    multiNum++;
}
