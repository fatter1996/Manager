#ifndef TRAINADDDLG_H
#define TRAINADDDLG_H
#include "qabstractbutton.h"
#include <QDialog>
class CData;
class CStationJSJ;
namespace Ui
{
class CTrainAddDlg;
}

class CTrainAddDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CTrainAddDlg(QWidget *parent = nullptr, CData *data = nullptr, unsigned int code = 0xffff, CStationJSJ *p = nullptr);
    ~CTrainAddDlg();
    bool addFlag;
private slots:
    void on_pushButton_clicked();
    void onSliderValueChanged(int value);

    void on_pushButton_2_clicked();

private:
    Ui::CTrainAddDlg *ui;
    CData *pData;
    CStationJSJ *pStation;
    unsigned int devCode;

};

#endif // TRAINADDDLG_H
