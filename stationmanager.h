#ifndef STATIONMANAGER_H
#define STATIONMANAGER_H

#include <QMainWindow>
#include "data.h"
#include "JMShowPro/trainmonitordlg.h"
#include "VoiceCall/VoiceDialog/VoiceDialog.h"
#include "Repetition/RepetitionWidget.h"
#include"JSJ/jsj_txservice.h"
#include<JSJ/jsj_controldlg.h>
#include"JSJ/jsj_self_startdlg.h"
namespace Ui
{
class CStationManager;
}

class CStationManager : public QMainWindow
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
public:
    explicit CStationManager(QWidget *parent = nullptr);
    ~CStationManager();
    CData *m_Data;
    CTh_kb *pJM;
    CTrainMonitorDlg *pMonitorDlg;
private:
    Ui::CStationManager *ui;
    QTimer *pShowT; //界面绘制刷新显示定时器申明
    long nElapsed;  //定时器计数器
    QStandardItemModel *TreeModel;
    bool isSHow = true;
    int Height;
    int LastHeight;
    void SetTreeData();
    QPushButton *pCloseButton;
    QPushButton *StationRestBtn;
    QPushButton *All_StationRest_Btn;
    VoiceDialog* m_pVoice = nullptr;
    jsj_Controldlg * controldlg = nullptr;
    jsj_Self_Startdlg * self_Startdlg = nullptr;
public:
    void SetCurrentStation(int index);
    void UpdateTreeData();
    void InitButtonLayout();
    void UpdateButtonLayout(QPushButton *pBtn);
    void ClearAndUpdate(CStationJSJ* pStation);
    void Connect(CStationJSJ* pStation);

    void InitVoiceCall();
    int getBtnItemRowByName(QString name);
private:
    void InitMenu();
    int test_taskBar();
public slots:
    void StationRest();
    void All_StationRest();
    void slotOntimeout();
    void slotPushButton(int index);
    void closeWindow();
    void UserManagerShowInfo();
    void UpdateControl_Info(ControlInfo&info);
private slots:
    void on_pushButton_clicked();
    void on_MenuBtn_clicked();
};

#endif // STATIONMANAGER_H
