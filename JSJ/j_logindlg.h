#ifndef J_LOGINDLG_H
#define J_LOGINDLG_H

#include <QWidget>

namespace Ui {
class J_logindlg;
}

class J_logindlg : public QWidget
{
    Q_OBJECT

public:
    explicit J_logindlg(QWidget *parent = nullptr);
    ~J_logindlg();

private:
    Ui::J_logindlg *ui;
};

#endif // J_LOGINDLG_H
