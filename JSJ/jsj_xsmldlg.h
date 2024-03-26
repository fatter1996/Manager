#ifndef JSJ_XSMLDLG_H
#define JSJ_XSMLDLG_H

#include <QWidget>
#include"jsj_services.h"
#include"testT/stationshowfrom.h"
namespace Ui {
class jsj_xsmldlg;
}

class jsj_xsmldlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_xsmldlg(QWidget *parent = nullptr);
    ~jsj_xsmldlg();
    void ClearInfo();
private:
    Ui::jsj_xsmldlg *ui;
    stationshowfrom * m_formdlg;
public:
    void InitStation();
    void InitStationInfo();
private:

signals:
    void SetSelectStation(QString StationName);
private slots:
    void XSMLMsgUpdate(DDML ML,unsigned int type,bool flag);
    void GetMsgSave_XSML(unsigned int kind,unsigned int type);
    void GetGDByStationName(QString StatioName);
    void on_comboBox_activated(const QString &arg1);
};

#endif // JSJ_XSMLDLG_H
