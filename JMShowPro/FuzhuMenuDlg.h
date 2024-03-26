#ifndef FUZHUMENUDLG_H
#define FUZHUMENUDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CFuzhuMenuDlg;
}

class CFuzhuMenuDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CFuzhuMenuDlg(QWidget *parent = nullptr);
    ~CFuzhuMenuDlg();

private:
    Ui::CFuzhuMenuDlg *ui;
    QSignalMapper *myMapper;
    QPushButton *button_Input[14];

public:
    void init(bool isYJFlag,bool isBJFlag,bool isQFTJFlag,bool isJTGD,bool isDCWZ,bool isWCQDName,bool isDCQDName,bool isXHDName,bool isDCName);

signals:
    void signal_FuzhuMenuDlg_AN(int index);

public slots:
    void slot_AnPro(int i);

private slots:
    void on_pushButton_Cancel_clicked();
};

#endif // FUZHUMENUDLG_H
