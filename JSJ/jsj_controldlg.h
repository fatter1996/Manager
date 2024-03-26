#ifndef JSJ_CONTROLDLG_H
#define JSJ_CONTROLDLG_H

#include <QWidget>
#include"jsj_services.h"
#include"jsj_txservice.h"
#include<QHBoxLayout>
#include<QCheckBox>

#define AGREE  0XAA
#define UNGREE 0XAB
namespace Ui {
class jsj_Controldlg;
}

class jsj_Controldlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_Controldlg(ControlInfo &info, QWidget *parent = nullptr);
    ~jsj_Controldlg();
    void InitTableInfo();
    void GetCheckedInfo_Init(QString StationName,unsigned char After);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::jsj_Controldlg *ui;
    int indexRow = -1;
    unsigned char beforecmd = 0x00;
    unsigned char Aftercmd = 0x00;
};

#endif // JSJ_CONTROLDLG_H
