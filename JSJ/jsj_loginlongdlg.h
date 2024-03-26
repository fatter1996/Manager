#ifndef JSJ_LOGINLONGDLG_H
#define JSJ_LOGINLONGDLG_H

#include <QWidget>

namespace Ui {
class jsj_loginlongdlg;
}

class jsj_loginlongdlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_loginlongdlg(QWidget *parent = nullptr);
    ~jsj_loginlongdlg();
public:
    QString UserKeyId ="";
    QString UserName = "";
public:
     int PageIndex = 0;
    void InitInfo();
    void InsertInfo();
    void ShowUpInfo();
    void showDonInfo(bool isDel);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_NextBtn_clicked();

    void on_UpBtn_clicked();

private:
    int IndexMax = 0;
    int IndexMin = 0;
    int index = 0;
    Ui::jsj_loginlongdlg *ui;
};

#endif // JSJ_LOGINLONGDLG_H
