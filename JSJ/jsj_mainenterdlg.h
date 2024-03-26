#ifndef JSJ_MAINENTERDLG_H
#define JSJ_MAINENTERDLG_H

#include <QMainWindow>

namespace Ui {
class jsj_MainEnterdlg;
}

class jsj_MainEnterdlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit jsj_MainEnterdlg(QWidget *parent = nullptr);
    ~jsj_MainEnterdlg();

private:
    Ui::jsj_MainEnterdlg *ui;
};

#endif // JSJ_MAINENTERDLG_H
