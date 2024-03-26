#ifndef JSJ_USERMANAGERDLG_H
#define JSJ_USERMANAGERDLG_H

#include <QWidget>
#include<QTableWidget>
#include<QHeaderView>
#include"jsj_services.h"
#include"jsj_addnewonedlg.h"
#include"jsj_logindlg.h"

namespace Ui {
class jsj_UserManagerdlg;
}

class jsj_UserManagerdlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_UserManagerdlg(QWidget *parent = nullptr);
    ~jsj_UserManagerdlg();
    void InitUserInfo();
    void InitTable(QString StationName,QString UserName);
    void ShowInit();
    void showDonInfo(bool isDel);
private slots:
    void on_ExitBtn_clicked();
    void on_SearchBtn_clicked();
    void on_ReSetBtn_clicked();
    void on_DelBtn_clicked();
    void on_DaochuBtn_clicked();
    void on_DaoruBtn_clicked();
    void on_AddNewBtn_clicked();
    void on_EditBtn_clicked();
private:
    Ui::jsj_UserManagerdlg *ui;
    int IndexMax = 0;
    int IndexMin = 0;
    int index = 0;
    int PageIndex = 0;
private slots:
    void on_RecordBtn_clicked();
    void on_NextBtn_clicked();
    void on_UpBtn_clicked();
    void SetUpdatedTable();
    void on_pushButton_clicked();
};

#endif // JSJ_USERMANAGERDLG_H
