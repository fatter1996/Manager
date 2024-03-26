#ifndef FLBLDLG_H
#define FLBLDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CFLBLDlg;
}

class CFLBLDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CFLBLDlg(QWidget *parent = nullptr);
    ~CFLBLDlg();

private:
    Ui::CFLBLDlg *ui;
    int nType;
    int nZCIndex;
    int nDevIndex;
    virtual void keyPressEvent(QKeyEvent *event);

public:
    void init(int zcCount,int devCount,int type,bool flag1,bool flag2,bool flag3);  //0x11-轨道道岔  0x22-无岔区段
private slots:
    void on_checkBox_FLBL_stateChanged(int arg1);
    void on_pushButton_FLBL_clicked();
    void on_pushButton_CQFLBL_clicked();
    void on_checkBox_CQFLBL_stateChanged(int arg1);
    void on_pushButton_DWFLBL_clicked();
    void on_checkBox_DWFLBL_stateChanged(int arg1);
    void on_pushButton_FWFLBL_clicked();
    void on_checkBox_FWFLBL_stateChanged(int arg1);
    void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();
signals:
    void signal_FlblDlgOK(int zcCount,int devCount,bool flag1,bool flag2,bool flag3);
};

#endif // FLBLDLG_H
