#ifndef GLOBALSTRUCTURE_H
#define GLOBALSTRUCTURE_H
#include "Global.h"
typedef struct
{
    int stationID;
    QPoint offSetPt;
    QPoint staNamePt;
    int m_nCTCSoftID;
    int m_nLSSoftID;
    int m_nXLMNJSoftID;
    void init()
    {
        stationID=m_nLSSoftID=m_nCTCSoftID=m_nXLMNJSoftID=0;
        offSetPt=QPoint(0,0);
        staNamePt=QPoint(0,0);
    }
}StaInfo;

typedef struct
{
    QString str_Speech;
    int n_Count;
    void init()
    {
        str_Speech="";
        n_Count=0;
    }
}SpeechData;

typedef struct
{
    QString strName;
    int nStationID;
    QPoint pt;
    bool isRunFlag;
    int runTime;
    void init()
    {
        strName="";
        nStationID=runTime=0;
        pt=QPoint(0,0);
        isRunFlag=false;
    }
}DLB_Data;
#endif // GLOBALSTRUCTURE_H
