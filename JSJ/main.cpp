#include <QApplication>
#include"jsj_maindlg.h"
#include"jsj_services.h"
#include<QMessageBox>
#include"jsj_txservice.h"
#include"testT/stationshowfrom.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBSERVICES.loadDBConfig();
    DBSERVICES.GetTrainTypeString("TrainNumType");
    qApp->setStyleSheet("QToolTip{border: 0px solid black;background:rgb(255,255,127);}");
    QString str = DBSERVICES.GetConnect_toDB();
    if(str == "")
    {
        TX_Send_SERVICES.bindPort();
        DBSERVICES.GetddInfoList("调度命令计划");
        JsJ_Maindlg * w = new JsJ_Maindlg();
        w->show();
        return a.exec();
    }
    else
    {
        QMessageBox::information(nullptr,"提示",QString("数据库连接失败,原因%1,请联系管理员处理").arg(str));
    }
}
