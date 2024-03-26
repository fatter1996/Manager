#ifndef COMFIRM_BTN_H
#define COMFIRM_BTN_H

#include <QObject>
#include<QPushButton>
#include"JSJ/jsj_services.h"
class Comfirm_Btn : public QPushButton
{
    Q_OBJECT
public:
    Comfirm_Btn( QString text, QWidget *parent = nullptr);
public:
    bool is_Select = false;
    QString StationName;
public slots:
    void Get_SetInfo();
public:
    void SetCurTimeInfo(PalenInfo&info);
};

#endif // COMFIRM_BTN_H
