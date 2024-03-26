#include"cmsgbox.h"
#include<QApplication>
#include<QPushButton>
#include"jsj_services.h"
CMsgBox::CMsgBox(QWidget *parent)
    :QDialog(parent)
{
    init();
}

int CMsgBox::showMsgBox(QString strText,  QWidget *parent)
{
    CMsgBox msgBox(parent);

    msgBox.m_pLabel->setText(strText);
    return msgBox.exec();
}

void CMsgBox::init()
{
    this->setFixedSize(533,279);
    this->setStyleSheet("color:rgb(57,193,230);");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); //隐藏标题栏
    this->setWindowModality(Qt::NonModal); //窗口模态

    QWidget *pWidget = new QWidget(this);
    pWidget->resize(this->size());
    pWidget->setStyleSheet("background-image: url(:/new/jsj/rc/新增/BB.png);");

    QGridLayout *pGLayout = new QGridLayout;
    pGLayout->setSpacing(20);


    m_pLabel = new QLabel(this);
    m_pLabel->setFixedSize(500,80);
    m_pLabel->setAlignment(Qt::AlignCenter);
    m_pLabel->setText(m_strText);
    m_pLabel->setStyleSheet("font-size:24px;"
                            "color:white");

    m_pLabel->setWordWrap(true);
    pBtnConfirm = new QPushButton(this);
    pBtnConfirm->setFixedSize(111,46);
    pBtnConfirm->setStyleSheet("border-image: url(:/new/jsj/rc/新增/p.png);");



    connect(pBtnConfirm,&QPushButton::clicked,[=]{
        done(ENM_OK_BTN);
    });

    pBtnCancle = new QPushButton(this);
    pBtnCancle->setFixedSize(111,46);
    pBtnCancle->setStyleSheet("border-image: url(:/new/jsj/rc/新增/d.png);");
    connect(pBtnCancle,&QPushButton::clicked,[=]{
        done(ENM_CANCEL_BTN);
    });
    {
        pGLayout->addWidget(m_pLabel,1,0,1,4,Qt::AlignCenter);
        pGLayout->addWidget(pBtnCancle,2,2,1,1,Qt::AlignRight);
        pGLayout->addWidget(pBtnConfirm,2,1,1,1,Qt::AlignLeft);
    }
    pGLayout->setRowMinimumHeight(2,50);
    this->setLayout(pGLayout);
}
void CMsgBox::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void CMsgBox::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void CMsgBox::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
