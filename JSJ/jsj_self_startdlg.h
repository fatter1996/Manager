#ifndef JSJ_SELF_STARTDLG_H
#define JSJ_SELF_STARTDLG_H

#include <QWidget>
#include"jsj_services.h"
#include"jsj_txservice.h"
namespace Ui {
class jsj_Self_Startdlg;
}

class jsj_Self_Startdlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_Self_Startdlg(QWidget *parent = nullptr);
    ~jsj_Self_Startdlg();
    void InitTable_SelfStart_Info();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_AllSelectBtn_clicked();
private:
    Ui::jsj_Self_Startdlg *ui;
    QWidget *pWidget =nullptr;
};

#endif // JSJ_SELF_STARTDLG_H
