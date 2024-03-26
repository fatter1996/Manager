#ifndef DDML_DDLG_H
#define DDML_DDLG_H

#include <QWidget>
#include"jsj_services.h"
#include"jsj_txservice.h"

namespace Ui {
class ddml_ddlg;
}

class ddml_ddlg : public QWidget
{
    Q_OBJECT

public:
    explicit ddml_ddlg(QWidget *parent = nullptr);
    ~ddml_ddlg();
    void InitStation_ddml();
private slots:

private:
    Ui::ddml_ddlg *ui;
    QStringList QSR_List;
private:
    void ClearInfo();
    void InitTabelInfo();
    QString GetPlaintextContent();
public:
    QString GetSendTime();
    int GetIdByName(QString Name);
private slots:
    void GetMsgUpdate(DDML ML, unsigned int Kind,bool is);
    void GetMsgSave(unsigned int Kind,unsigned int type);
    void on_ml_type_Combo_currentTextChanged(const QString &arg1);
};

#endif // DDML_DDLG_H
