#ifndef JSJ_ADDNEWONEDLG_H
#define JSJ_ADDNEWONEDLG_H

#include <QWidget>
#include"jsj_services.h"
#include<QButtonGroup>
namespace Ui {
class jsj_addnewOnedlg;
}

class jsj_addnewOnedlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_addnewOnedlg(QWidget *parent = nullptr);
    ~jsj_addnewOnedlg();
public:
    void GetCheckLight(QString &Name);
private slots:
    void on_pushButton_2_clicked();
    void slot_RtnGroupChanged(int s1);
    void on_pushButton_3_clicked();
    void on_ChezhanCombo_currentIndexChanged(int index);
    void on_ChezhanCombo_currentTextChanged(const QString &arg1);
    void on_pushButton_clicked();

public:
    bool isFileExist(QString StationName, QString TeamName, QString UserName);
public:
    unsigned int OperateType;
    QString UserKeyId = "";
public:
    void InitInfo();
private:
    Ui::jsj_addnewOnedlg *ui;
    QButtonGroup * bgGroup;
    QString GZNumberStr;
    QString UserName;
    QString LoginName;
    QString LoginPassword;
    QString BeiZhu;
    int StatusId = -1;
    int GangweiId = -1;
    int JueSe = -1;
    int StationId = -1;
};

#endif // JSJ_ADDNEWONEDLG_H
