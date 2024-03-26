#ifndef JSJ_STATIONHEART_H
#define JSJ_STATIONHEART_H

#include <QObject>
#include<QTimer>
#include<QDateTime>
#define TIMER_TIMEOUT   (3*1000)
#define HEARTBET_TIMEOUT   (2*100)
class jsj_Stationheart : public QObject
{
    Q_OBJECT
public:
    explicit jsj_Stationheart( int StationId, QObject *parent = nullptr);

signals:
public:
    int multiNum = 0;
private:
    QTimer * CurTimer;
    QDateTime m_lastTime;
    int TimerId = -1;
    int StationId_;
private slots:
    void checkHeartbeat();
};

#endif // JSJ_STATIONHEART_H
