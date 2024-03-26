#ifndef TRAINMONITORDLG_H
#define TRAINMONITORDLG_H
#include<QStandardItemModel>
#include <QDialog>
#include <qaction.h>
#include <QMenuBar>
class CData;
namespace Ui
{
class CTrainMonitorDlg;
}

class CTrainMonitorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CTrainMonitorDlg(QWidget *parent = nullptr, CData *p = nullptr);
    ~CTrainMonitorDlg();

    CData *pData;
    QTimer *updateTimer;
    int row = -1;
    void clearAllView();
    void MoveWindow();
private:
    Ui::CTrainMonitorDlg *ui;
    QStandardItemModel *standItemModel;
    QAction *DirectionAction;
    QAction *TypeAction;
    QAction *FCAction;
    QAction *SFCAction;
    QAction *DLAction;
    QAction *CXAction[5];

    void InitMenue();
    void ModifyAttMenue(int column);
private  slots:
    void UpdateTrainMsg();
    void on_tableView_clicked(const QModelIndex &index);
    void on_cxButton_clicked();
    void on_delButton_clicked();
    void on_pushButton_clicked();
    void on_loseButton_clicked();
    void on_dlButton_clicked();
    void on_fdlButton_clicked();
    void on_typeButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_lcButton_clicked();
    void on_dcButton_clicked();
    void on_sfcButton_clicked();
    void onSliderValueChanged(int value);
    void on_speedButton_clicked();
    void on_zhaiguaButton_clicked();
    void on_lianguaButton_clicked();
    void slot_tableViewMenu(QPoint pt);
    void on_tableView_pressed(const QModelIndex &index);
    void on_fcButton_clicked();
};

#endif // TRAINMONITORDLG_H
