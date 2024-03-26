#ifndef T_MyStation_H
#define T_MyStation_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>

namespace Ui {
class T_MyStation;
}

class T_MyStation : public QMainWindow
{
    Q_OBJECT

public:
    explicit T_MyStation(QWidget *parent = nullptr);
    ~T_MyStation();


    int StringSplit(QString source, QString ch,QStringList &strarr);

private:
    Ui::T_MyStation *ui;
};

#endif // T_MyStation_H
