#ifndef QFDLG_H
#define QFDLG_H

#include <QDialog>
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CQfDlg;
}

class CQfDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CQfDlg(QWidget *parent = nullptr);
    ~CQfDlg();

private:
    Ui::CQfDlg *ui;
    QString strPassWork;

private:
    QString getStrInitOfCode(int funCode,QString passWork,int type);
public:
    void init(int funCode,QString passWork,int type);
    virtual void keyPressEvent(QKeyEvent *event);
private slots:
    void on_pushButton_0_clicked();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_DelOne_clicked();
    void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();
};

#endif // QFDLG_H
