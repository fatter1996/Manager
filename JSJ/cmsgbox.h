#ifndef CMSGBOX_H
#define CMSGBOX_H

#include <QObject>
#include <QGridLayout>
#include<QBoxLayout>
#include<QDialog>
#include<QLabel>
#include <QMouseEvent>
class CMsgBox : public QDialog
{
    Q_OBJECT
public:
    explicit CMsgBox(QWidget *parent = nullptr);

    static int showMsgBox(QString strText, QWidget *parent = nullptr);

private:
    void init();
public:
    enum EnmButton{
        ENM_OK_BTN = 0,
        ENM_CANCEL_BTN
    };
public:
private:
    QString m_strImgPath = "";
    QString m_strText = "";
    QPushButton *pBtnConfirm;
    QPushButton *pBtnCancle;
    QLabel *m_pLabel = nullptr;
     QPoint last;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};
#endif // CMSGBOX_H
