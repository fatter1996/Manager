#ifndef QRWCZYDLG_H
#define QRWCZYDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CQjlogicDlg;
}

class CQjlogicDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CQjlogicDlg(QWidget *parent = nullptr);
    ~CQjlogicDlg();

private:
    Ui::CQjlogicDlg *ui;

public:
    void init(int type);  //0x11-确认无车占用   0x22-区间总解锁  0x33-开启逻辑检查   0x44-关闭逻辑检查
};

#endif // QRWCZYDLG_H
