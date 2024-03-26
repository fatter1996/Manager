#ifndef JSJ_MAINDLG_H
#define JSJ_MAINDLG_H

#include <QWidget>
#include"jsj_usermanagerdlg.h"
#include"jsj_ddmldlg.h"

#include<QButtonGroup>
namespace Ui {
class JsJ_Maindlg;
}

class JsJ_Maindlg : public QWidget
{
    Q_OBJECT

public:
    explicit JsJ_Maindlg(QWidget *parent = nullptr);
    ~JsJ_Maindlg();

private:
    Ui::JsJ_Maindlg *ui;
    jsj_UserManagerdlg * m_Managerdlg;
    JSJ_ddmldlg * dlg;
    QButtonGroup * m_Group;
private slots:
    void SetSlot(int v);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_DDZXBtn_clicked();
    void SetMainWindow(bool isShow);
};

#endif // JSJ_MAINDLG_H
