#ifndef JSJ_LOGINDLG_H
#define JSJ_LOGINDLG_H

#include <QDialog>

namespace Ui {
class jsj_logindlg;
}

class jsj_logindlg : public QDialog
{
    Q_OBJECT

public:
    explicit jsj_logindlg(QWidget *parent = nullptr);
    ~jsj_logindlg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::jsj_logindlg *ui;
};

#endif // JSJ_LOGINDLG_H
