#ifndef MONITOR_H
#define MONITOR_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "TStation.h"
#include<QMouseEvent>
#include"testT/T_globaldata.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class Monitor;
}
QT_END_NAMESPACE

class Monitor : public QMainWindow
{
    Q_OBJECT

public:
    Monitor(QWidget *parent = nullptr);
    Monitor(int ExamID, unsigned char show, unsigned char size, QWidget *parent = nullptr);
    ~Monitor();

    //重绘界面
protected:
    void paintEvent(QPaintEvent *e);

public:
    QVector<TStation*>zcArray;   //站场数据数组
    QString StationaName;
private:
    Ui::Monitor *ui;

public:
    TStation::QD_Str pQD;
    T_jtx pJTX;
    T_Text pText;
    T_GDDC dc ;



    QTransform m_Transform;
    TStation *station;  //站场类指针定义（后面考虑改为数组）
    QTimer *UpdateTimer;
    QTimer *JLProcTimer;
    QTimer *ANProcTimer;
    int nElapsed;
    float nDiploid;

    unsigned char showType;//0-联锁界面 1-联锁工程师培训标记  2-CTC界面  3-监测机
    unsigned char sizeType;//大小类型
    bool CZState;//是否操作状态
    int CurrentExamID;    //试题ID
    bool FCZKFlag;   //非常站控
    //读取数据库相关功能

    /*****************ctc3.0 设备选择变量设置*******************/
    bool IsEditFlag;
    QRect SelectRect;
    QPoint StartPt;
    QPoint FtartPt;
    bool  SelectFlag;
    bool  IsstationLoadFlag;
    QStringList DCList_SAVE,XHList_SAVE,QDList_SAVE,QJList_SAVE;
    /*****************END*******************/
public:
    TStation * tempStation = nullptr;
    QString connectionName;
    QSqlDatabase * m_sqliteDB;
    void dataAcquisition();
    int HexStringToInt(QString str, int length); //将十六进制表示的字符串转换为整型数据
    QPoint StringToPoint(QString str);
    QRect StringToRect(QString str);
    int StringGLBToInt(QString str); //公里标字符串转换为米单位整型
    void ClearAll();
    void setScene(int TitleID);//设置场景 根据试题ID
    void InitScene(int TitleID, unsigned char show, unsigned char size);//初始化场景 根据试题ID

    void StationRead(QString FileName);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void OnLButtonDown(QMouseEvent *event);


    void OnRButtonDown(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void ResetRectSelect();
    void SelectDevFun();
    QRect CountRectFun();
    void BaseRectDraw(QPainter *painter, int LineType,QColor LineColor,QColor BlackColor, QRect rect);
    void RectDraw(QPainter *painter,QRect rect);
    void GetSelectDev(QRect Rect,  QStringList *XHdev,QStringList *DCdev,QStringList *QJdev,QStringList *QDdev,QStringList *XHdev_code,QStringList *DCdev_code,QStringList *QJdev_code,QStringList *QDdev_code);


signals:
    void SendCancelCMD();
    void SendFCZKCMD();
    void SendSelectDevsignal(QStringList XHList,QStringList DCList,QStringList QJList,QStringList QDList,QStringList XHdev_code,QStringList DCdev_code,QStringList QJdev_code,QStringList QDdev_code);
private slots:
    void UPdate();
    void JLProc();
    void ANProc();
    void ClearStatesName(QString Type,QString Name);

    //    void on_ZQXButton_clicked();
    void DealWithCTCCmd(QVector<T_XHD*> v_ANArray);
    void SetCancleByInterlock();

    void ClearSelectFun();
    //    void UpdataStationdata(QString StationName,int ID,QString fileName,int sizetype);
    void UpdataStationdata(int ID,QString fileName,bool sizetype);

    void setZCRefresh();
};
#endif // MAINWINDOW_H
