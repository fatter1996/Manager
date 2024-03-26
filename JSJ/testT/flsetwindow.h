#ifndef FLSETWINDOW_H
#define FLSETWINDOW_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>

class TrackView_Lable;

namespace Ui {
class FLSetWindow;
}

class FLSetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FLSetWindow(TrackView_Lable *p,QWidget *parent = nullptr);
    ~FLSetWindow();

    TrackView_Lable *trackview_Lable;
    int LoginState;//0为默认，1为值班员，2为信号员
    int JieFaCheState;//0为接预，1为到点，2为发预，3为发点,4上水，5吸污，6机车，7列检，8交令，9交票，10列尾，11货检，12乘降，13摘挂，14换乘，15装卸，16道口，17车号，18站务，19综控
    int TrackNum;
    int Track_widget_line_Flag;
    int trafficlog_OrderLineNum;
    QString str_QP_L;
    QString str_QP_R;

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::FLSetWindow *ui;
};

#endif // FLSETWINDOW_H
