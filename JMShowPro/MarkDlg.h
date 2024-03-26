#ifndef MARKDLG_H
#define MARKDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CMarkDlg;
}

class CMarkDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CMarkDlg(QWidget *parent = nullptr);
    ~CMarkDlg();

private:
    Ui::CMarkDlg *ui;
    QSignalMapper *myMapper;
    QPushButton *button_Input[53];
    virtual void keyPressEvent(QKeyEvent *event);
    QTimer *pTime; //界面绘制刷新显示定时器申明
    int nfocus;
    int nZCCount;
    int nDevCount;

public:
    void init(int zcCount,int devCount);

public slots:
    void getText(int i);

signals:
    void signal_MarkDlgOK(int zcCount,int devCount,QString str1,QString str2);

private slots:
    void on_pushButton_TuiGe_clicked();
    void on_pushButton_QingKong_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_OK_clicked();
    void slot_onTimeOut();
};

#endif // MARKDLG_H
