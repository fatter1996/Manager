#ifndef ALARMDLG_H
#define ALARMDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CAlarmDlg;
}

class CAlarmDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CAlarmDlg(QWidget *parent = nullptr);
    ~CAlarmDlg();

private:
    Ui::CAlarmDlg *ui;
    //QVector<QLineEdit *>lineEditArray;
    int count;
    int DlgType;  //0x11-标准界面操作信息提示框，0x22-标准界面报警窗口，0x33-卡斯柯界面操作信息提示框

    QLineEdit *lineEdit_State_DevA;
    QLineEdit *lineEdit_State_DevB;
    QLineEdit *lineEdit_State_OpA;
    QLineEdit *lineEdit_State_OpB;
    QLineEdit *lineEdit_State_WXJ;
public:
    void init(int type,QPoint pt);

    void add_TSXX(QString str,QColor color);
};

#endif // ALARMDLG_H
