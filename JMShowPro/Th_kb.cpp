#include <qmenu.h>
//#include <QLabel>
#include "Th_kb.h"
CTh_kb::CTh_kb()
{
    nCurrentCode = 0;
    BJDlgPt.setX(0);
    BJDlgPt.setY(0);
    YJDlgPt.setX(0);
    YJDlgPt.setY(0);
    yjDlgIsShowType = 0;
    nFunbtnDownTime = 0;
    nQFDlgTitleType = 0;
    strQFPassWork = "123";
    fczkPt = QPoint(0, 0);
    fczkType = 0;
    rect_FCZK = QRect(0, 0, 0, 0);
    ModeType = 0x22;
    isAllowZL = true;
    vect_LSDArray.clear();
    vect_LineArray.clear();
    vect_TextArray.clear();
    pCom_menu = nullptr;
    pCom = nullptr;
    pClear = nullptr;
    isHaveRBtnMenu = false;
}
CTh_kb::~CTh_kb()
{
    delete pCom_menu;
    pCom_menu = nullptr;
    delete pCom;
    pCom = nullptr;
    delete pClear;
    pClear = nullptr;
}
void CTh_kb::initData(QWidget *w)
{
    readFunBtnData();
    readInterfaceData();

    alarmBJ_Dlg = new CAlarmDlg(w);
    alarmBJ_Dlg->init(0x11, BJDlgPt);
    alarmBJ_Dlg->setModal(false);
    //alarmBJ_Dlg->show();

    alarmDlg = new CAlarmDlg(w);
    alarmDlg->init(0x22, YJDlgPt);
    alarmDlg->setModal(false);
    //alarmDlg->show();

    alarmState_Dlg = new CAlarmDlg(w);
    alarmState_Dlg->init(0x33, YJDlgPt);
    alarmState_Dlg->setModal(false);
    //alarmState_Dlg->show();

    initQFTJInfo();

    qftj_dlg = new CQftjDlg(w);
    qftj_dlg->init();
    alarmState_Dlg->setModal(false);
    qftj_dlg->hide();
    connect(this, SIGNAL(signal_upDataQFTJList(QVector<QfListItem *>)), qftj_dlg, SLOT(slot_upDataList(QVector<QfListItem *>)));

    setAlarmTextADD(0x11, "测试字符串1");
    setAlarmTextADD(0x22, "测试字符串2");

    flag_jtgd = false;
    flag_dcwz = false;
    flag_wcqdName = false;
    flag_dcqdName = false;
    flag_xhdName = false;
    flag_dcName = false;
    initMenu();
}
void CTh_kb::initMenu()
{
    if (isHaveRBtnMenu == false)
    {
        return;
    }
    pCom_menu = new QMenu;
    pCom = new QAction;  //添加将完成响应的控件
    pClear = new QAction;
    pCom = pCom_menu->addAction("命令");
    pClear = pCom_menu->addAction("清除");
    pCom->setCheckable(false);
    pClear->setCheckable(false);
    pCom->setEnabled(false);
    pClear->setEnabled(true);

    //设置信号与槽
    connect(pClear, SIGNAL(triggered()), this, SLOT(onClearAction()));
}
void CTh_kb::initQFTJInfo()
{
    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        if (FunBtnArray[i]->isHaveQF == true)
        {
            QfListItem *item = new QfListItem();
            item->init();
            item->strName = FunBtnArray[i]->name1 + FunBtnArray[i]->name2;
            item->nQfType = 0x11;
            item->nIndex = i;
            vect_QFTJArray.push_back(item);
        }
    }
}
void CTh_kb::AddQFTJInfo(int qftype, int index)
{
    for (int i = 0; i < vect_QFTJArray.size(); i++)
    {
        if ((qftype == vect_QFTJArray[i]->nQfType) && (index == vect_QFTJArray[i]->nIndex))
        {
            vect_QFTJArray[i]->nQfCount++;
            break;
        }
    }
}
void CTh_kb::Draw(QPainter *painter, long nElapsed, double nDiploid)
{
    //反走样,防止出现锯齿状线条
    painter->setRenderHint(QPainter::Antialiasing, true);
    draw_FunBtn(painter, nElapsed, nDiploid);
    Draw_DLB(painter, nElapsed, nDiploid);
    draw_FCZK(painter, nElapsed, nDiploid);
    draw_LSD(painter, nElapsed, nDiploid);
    draw_Line(painter, nElapsed, nDiploid);
    draw_Text(painter, nElapsed, nDiploid);//文本需要最后绘制
}
void CTh_kb::draw_FCZK(QPainter *painter, long nElapsed, double nDiploid)
{
    //反走样
    painter->setRenderHint(QPainter::Antialiasing, true);
    QFont font;
    font.setFamily("宋体");
    font.setPointSize(10 * nDiploid);//字号
    font.setItalic(false);//斜体
    painter->setFont(font);//设置字体
    rect_FCZK.setRect(fczkPt.x(), fczkPt.y(), 14 * nDiploid, 14 * nDiploid);
    if (fczkType == 0)
    {
        //非常站控按钮
        painter->setPen(Qt::gray);
        painter->setBrush(Qt::white);
        painter->drawRect(rect_FCZK);
        painter->setPen(Qt::yellow);//设置画笔颜色
        painter->drawText(fczkPt.x() - 20 * nDiploid, fczkPt.y() + 30 * nDiploid, "非常站控");

        painter->setPen(Qt::white);//设置画笔颜色

        if (ModeType == 0x22)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", fczkPt.x() - 60 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", fczkPt.x() - 60 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        painter->drawText(fczkPt.x() - 80 * nDiploid, fczkPt.y() + 30 * nDiploid, "非常站控");


        if (ModeType == 0x11)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Green.jpg", fczkPt.x() - 120 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", fczkPt.x() - 120 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        painter->drawText(fczkPt.x() - 140 * nDiploid, fczkPt.y() + 30 * nDiploid, "自律控制");

        if (isAllowZL == true)
        {
            draw_Pixmap(painter, ":/Pictures/LED_Black.jpg", fczkPt.x() - 180 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", fczkPt.x() - 180 * nDiploid, fczkPt.y(), 13, 13, nDiploid);
        }
        painter->drawText(fczkPt.x() - 200 * nDiploid, fczkPt.y() + 30 * nDiploid, "允许自律");

        painter->setPen(QPen(Qt::white, 1));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(fczkPt.x() - 205 * nDiploid, fczkPt.y() - 8 * nDiploid, 240 * nDiploid, 45 * nDiploid);
    }
}
void CTh_kb::draw_FunBtn(QPainter *painter, long nElapsed, double nDiploid)
{
    QFont font;
    QPoint namePt;
    int width = 60 * nDiploid;
    int heigh = 40 * nDiploid;
    FunBtnPt.setX(FunBtnPtConst.x()*nDiploid);
    FunBtnPt.setY(FunBtnPtConst.y()*nDiploid);
    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        FunBtnArray[i]->rect_Btn.setRect(FunBtnPt.x() + width * i, FunBtnPt.y(), width, heigh);
        if (FunBtnArray[i]->isDown == false) //未被按下
        {
            draw_Pixmap(painter, ":/Pictures/FunBtnUP_Thkb.jpg", FunBtnPt.x() + width * i, FunBtnPt.y(), width, heigh, nDiploid);
            namePt.setX((width * i) + (width / 2));
            namePt.setY(FunBtnPt.y() + (heigh / 2) + 5 * nDiploid);
        }
        else
        {
            draw_Pixmap(painter, ":/Pictures/FunBtnDOWN_Thkb.jpg", FunBtnPt.x() + width * i, FunBtnPt.y(), width, heigh, nDiploid);
            namePt.setX((width * i) + (width / 2));
            namePt.setY(FunBtnPt.y() + (heigh / 2) + 5 * nDiploid);
        }
        font.setFamily("Times New Roman");
        font.setPointSize(9 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        if (FunBtnArray[i]->isHaveQF == true)
        {
            painter->setPen(Qt::red);//设置画笔颜色
        }
        else
        {
            painter->setPen(Qt::black);//设置画笔颜色
        }
        //绘制文本
        QFontMetrics fm(font);
        if (FunBtnArray[i]->name2 == "")
        {
            QRect rec = fm.boundingRect(FunBtnArray[i]->name1);
            painter->drawText(namePt.x() - rec.width() / 2, namePt.y() - 4 * nDiploid, FunBtnArray[i]->name1);
        }
        else
        {
            QRect rec = fm.boundingRect(FunBtnArray[i]->name1);
            painter->drawText(namePt.x() - rec.width() / 2, namePt.y() - 10 * nDiploid, FunBtnArray[i]->name1);
            rec = fm.boundingRect(FunBtnArray[i]->name2);
            painter->drawText(namePt.x() - rec.width() / 2, namePt.y() + 6 * nDiploid, FunBtnArray[i]->name2);
        }
    }
}
void CTh_kb::readFunBtnData()
{
    QString File = QString(GetWorkDirRoot() + "Data/operationMenue.ini");
    QSettings *configIniRead = new QSettings(File, QSettings::IniFormat);
    configIniRead->setIniCodec("UTF-8");
    int num = 0;
    nFunbtnDownTime = configIniRead->value("FUNBTNData/FUNBTNDOWNTIME").toInt();
    FunBtnPt = StringToPoint(configIniRead->value("FUNBTNData/FUNBTNPOINT").toString());
    FunBtnPtConst = FunBtnPt;
    num = configIniRead->value("FUNBTNData/FUNBTNNUM").toInt();
    for (int i = 0; i < num; i++)
    {
        QString getString =  configIniRead->value(QString("FUNBTNData/FUNBUTTON%1").arg(i)).toString();
        QStringList list = getString.split("|");
        if (list.size() == 4)
        {
            FunBtnStr *funBtn = new FunBtnStr();
            funBtn->init();
            funBtn->name1 = list[0];
            funBtn->name2 = list[1];
            funBtn->isHaveQF = list[2].toInt();
            if (list[3] != "")
            {
                funBtn->stationID = StringToHex(list[3]);
            }
            FunBtnArray.append(funBtn);
        }
        else
        {
            QMessageBox::information(NULL, VERSION, QString("标准界面功能按钮数据配置格式错误，请检查!    %1").arg(i));
        }
    }
    File.clear();

    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        FunBtnArray[i]->nFunCode = getFunCodeOfName(FunBtnArray[i]->name1, FunBtnArray[i]->name2);
        //qDebug()<<FunBtnArray[i]->name1<<FunBtnArray[i]->name2<<FunBtnArray[i]->nFunCode;
    }
}
int CTh_kb::getFunCodeOfName(QString name1, QString name2)
{
    if ((name1.contains("S", Qt::CaseSensitive) && (name2.contains("引导总锁", Qt::CaseSensitive))) || (name1.contains("S引导总锁", Qt::CaseSensitive)) || (name2.contains("S引导总锁", Qt::CaseSensitive)))
    {
        return Fun_SYDZS;
    }
    if ((name1.contains("X", Qt::CaseSensitive) && (name2.contains("引导总锁", Qt::CaseSensitive))) || (name1.contains("X引导总锁", Qt::CaseSensitive)) || (name2.contains("X引导总锁", Qt::CaseSensitive)))
    {
        return Fun_XYDZS;
    }
    if ((name1.contains("坡道", Qt::CaseSensitive) && (name2.contains("解锁", Qt::CaseSensitive))) || (name1.contains("坡道解锁", Qt::CaseSensitive)) || (name2.contains("坡道解锁", Qt::CaseSensitive)))
    {
        return Fun_PDJS;
    }
    if ((name1.contains("取消", Qt::CaseSensitive) || (name2.contains("取消", Qt::CaseSensitive))))
    {
        return Fun_ZQX;
    }
    if ((name1.contains("人解", Qt::CaseSensitive) || (name2.contains("人解", Qt::CaseSensitive))))
    {
        return Fun_ZRJ;
    }
    if ((name1.contains("区故", Qt::CaseSensitive) || (name2.contains("区故", Qt::CaseSensitive))))
    {
        return Fun_QGJ;
    }
    if ((name1.contains("总定", Qt::CaseSensitive) || (name2.contains("总定", Qt::CaseSensitive))))
    {
        return Fun_DCZD;
    }
    if ((name1.contains("总反", Qt::CaseSensitive) || (name2.contains("总反", Qt::CaseSensitive))))
    {
        return Fun_DCZF;
    }
    if ((name1.contains("清除", Qt::CaseSensitive) || (name2.contains("清除", Qt::CaseSensitive))))
    {
        return Fun_CLEAR;
    }
    if ((name1.contains("单锁", Qt::CaseSensitive) || (name2.contains("单锁", Qt::CaseSensitive))))
    {
        return Fun_DCDS;
    }
    if ((name1.contains("单解", Qt::CaseSensitive) || (name2.contains("单解", Qt::CaseSensitive))))
    {
        return Fun_DCDJ;
    }
    if (((name1.contains("按钮", Qt::CaseSensitive) && (name2.contains("封锁", Qt::CaseSensitive)))) || (name1.contains("按钮封锁", Qt::CaseSensitive)) || (name2.contains("按钮封锁", Qt::CaseSensitive)))
    {
        return Fun_ANFS;
    }
    if (((name1.contains("按钮", Qt::CaseSensitive) && (name2.contains("解封", Qt::CaseSensitive)))) || (name1.contains("按钮解封", Qt::CaseSensitive)) || (name2.contains("按钮解封", Qt::CaseSensitive)))
    {
        return Fun_ANJF;
    }
    if (((name1.contains("道岔", Qt::CaseSensitive) && (name2.contains("封锁", Qt::CaseSensitive)))) || (name1.contains("道岔封锁", Qt::CaseSensitive)) || (name2.contains("道岔封锁", Qt::CaseSensitive)))
    {
        return Fun_DCFS;
    }
    if (((name1.contains("道岔", Qt::CaseSensitive) && (name2.contains("解封", Qt::CaseSensitive)))) || (name1.contains("道岔解封", Qt::CaseSensitive)) || (name2.contains("道岔解封", Qt::CaseSensitive)))
    {
        return Fun_DCJF;
    }
    if ((name1.contains("点灯", Qt::CaseSensitive) || (name2.contains("点灯", Qt::CaseSensitive))))
    {
        return Fun_DD;
    }
    if ((name1.contains("灭灯", Qt::CaseSensitive) || (name2.contains("灭灯", Qt::CaseSensitive))))
    {
        return Fun_MD;
    }
    if (((name1.contains("上电", Qt::CaseSensitive) && (name2.contains("解锁", Qt::CaseSensitive)))) || (name1.contains("上电解锁", Qt::CaseSensitive)) || (name2.contains("上电解锁", Qt::CaseSensitive)))
    {
        return Fun_SDJS;
    }
    if (((name1.contains("辅助", Qt::CaseSensitive) && (name2.contains("菜单", Qt::CaseSensitive)))) || (name1.contains("辅助菜单", Qt::CaseSensitive)) || (name2.contains("辅助菜单", Qt::CaseSensitive)))
    {
        return Fun_FZMENU;
    }
    if (((name1.contains("分路", Qt::CaseSensitive) && (name2.contains("不良", Qt::CaseSensitive)))) || (name1.contains("分路不良", Qt::CaseSensitive)) || (name2.contains("分路不良", Qt::CaseSensitive)))
    {
        return Fun_FLBL;
    }
    if ((name1.contains("标记", Qt::CaseSensitive) || (name2.contains("标记", Qt::CaseSensitive))))
    {
        return Fun_MARK;
    }
    if (((name1.contains("故障", Qt::CaseSensitive) && (name2.contains("通知", Qt::CaseSensitive)))) || (name1.contains("故障通知", Qt::CaseSensitive)) || (name2.contains("故障通知", Qt::CaseSensitive)))
    {
        return Fun_GZTZ;
    }
    if (((name1.contains("确认", Qt::CaseSensitive) && (name2.contains("占用", Qt::CaseSensitive)))) || (name1.contains("确认无车占用", Qt::CaseSensitive)) || (name2.contains("确认无车占用", Qt::CaseSensitive)))
    {
        return Fun_QRWCZY;
    }
    if (((name1.contains("区间", Qt::CaseSensitive) && (name2.contains("解锁", Qt::CaseSensitive)))) || (name1.contains("区间总解锁", Qt::CaseSensitive)) || (name2.contains("区间总解锁", Qt::CaseSensitive)))
    {
        return Fun_QJZJS;
    }
    if (((name1.contains("开启", Qt::CaseSensitive) && (name2.contains("检查", Qt::CaseSensitive)))) || (name1.contains("开启逻辑检查", Qt::CaseSensitive)) || (name2.contains("开启逻辑检查", Qt::CaseSensitive)))
    {
        return Fun_OPENLJJC;
    }
    if (((name1.contains("关闭", Qt::CaseSensitive) && (name2.contains("检查", Qt::CaseSensitive)))) || (name1.contains("关闭逻辑检查", Qt::CaseSensitive)) || (name2.contains("关闭逻辑检查", Qt::CaseSensitive)))
    {
        return Fun_CLOSELJJC;
    }
    return 0;
}
void  CTh_kb::readInterfaceData()
{
    QString File = QString(GetWorkDirRoot() + "Data/InterfaceData_TH_KB.ini");
    QSettings *configIniRead = new QSettings(File, QSettings::IniFormat);
    configIniRead->setIniCodec("UTF-8");
    int num_DLB = 0;
    int count_DLB = 0;
    int num_LSBSD = 0;
    int count_LSBSD = 0;
    int num_Line = 0;
    int count_Line = 0;
    int num_Text = 0;
    int count_Text = 0;
    BJDlgPt = StringToPoint(configIniRead->value("JMBasedata/BJCK_Point").toString());
    YJDlgPt = StringToPoint(configIniRead->value("JMBasedata/YJCK_Point").toString());
    yjDlgIsShowType = configIniRead->value("JMBasedata/YJCKYCTYPE").toInt();
    nQFDlgTitleType = configIniRead->value("JMBasedata/QFDLGTITLETYPE").toInt();
    strQFPassWork = configIniRead->value("JMBasedata/QFPASSWORK").toString();
    isHaveRBtnMenu = configIniRead->value("JMBasedata/IsHaveRBtmMenu").toBool();

    num_DLB =  configIniRead->value("DLBData/DLBNUM").toInt();
    for (int i = 0; i < num_DLB; i++)
    {
        QString getString =  configIniRead->value(QString("DLBData/DLB%1").arg(i)).toString();
        QStringList list = getString.split("|");
        if (list.size() == 3)
        {
            if ((list[0] != "") && (list[1] != "") && (list[2] != ""))
            {
                DLB_Data *pDLB = new DLB_Data();
                pDLB->init();
                pDLB->strName = list[0];
                pDLB->nStationID = StringToHex(list[1]);
                pDLB->pt = StringToPoint(list[2]);
                vect_DLBArray.append(pDLB);
                count_DLB++;
            }
            else
            {
                QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 电流表数据格式错误，请检查(1)!    %1").arg(i));
            }
        }
        else
        {
            QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 电流表数据格式错误，请检查(2)!    %1").arg(i));
        }
    }

    fczkPt = StringToPoint(configIniRead->value("FCZKData/FCZKPt").toString());
    fczkType = configIniRead->value("FCZKData/FCZJType").toInt();

    num_LSBSD = configIniRead->value("LSBSDData/LSBSDNUM").toInt();
    for (int i = 0; i < num_LSBSD; i++)
    {
        QString getString =  configIniRead->value(QString("LSBSDData/LSBSD%1").arg(i)).toString();
        QStringList list = getString.split("|");
        if (list.size() == 6)
        {
            if ((list[0] != "") && (list[1] != "") && (list[2] != "") && (list[3] != "") && (list[4] != "") && (list[5] != ""))
            {
                LSD_Data lsd;
                lsd.init();
                lsd.strName = list[0];
                lsd.nNameSize = list[1].toInt();
                lsd.nNameShowType = list[2].toInt();
                lsd.cName_Color = StringToColor(list[3]);
                lsd.pt = StringToPoint(list[4]);
                lsd.ptConst = StringToPoint(list[4]);
                lsd.cBSD_Color = StringToColor(list[5]);
                vect_LSDArray.append(lsd);
                count_LSBSD++;
            }
            else
            {
                QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 零散表示灯数据格式错误，请检查(1)!    %1").arg(i));
            }
        }
        else
        {
            QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 零散表示灯数据格式错误，请检查(2)!    %1").arg(i));
        }
    }
    num_Line = configIniRead->value("LineData/LineNum").toInt();
    for (int i = 0; i < num_Line; i++)
    {
        QString getString =  configIniRead->value(QString("LineData/Line%1").arg(i)).toString();
        QStringList list = getString.split("|");
        if (list.size() == 5)
        {
            if ((list[0] != "") && (list[1] != "") && (list[2] != "") && (list[3] != "") && (list[4] != ""))
            {
                Line_Data line;
                line.init();
                line.pt1 = StringToPoint(list[0]);
                line.pt1Const = StringToPoint(list[0]);
                line.pt2 = StringToPoint(list[1]);
                line.pt2Const = StringToPoint(list[1]);
                line.nLineSize = list[2].toInt();
                line.nLineType = list[3].toInt();
                line.cLineColor = StringToColor(list[4]);
                vect_LineArray.append(line);
                count_Line++;
            }
            else
            {
                QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 线段数据格式错误，请检查(1)!    %1").arg(i));
            }
        }
        else
        {
            QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 线段数据格式错误，请检查(2)!    %1").arg(i));
        }
    }
    num_Text = configIniRead->value("TextData/TextNum").toInt();
    for (int i = 0; i < num_Text; i++)
    {
        QString getString =  configIniRead->value(QString("TextData/Text%1").arg(i)).toString();
        QStringList list = getString.split("|");
        if (list.size() == 5)
        {
            if ((list[0] != "") && (list[1] != "") && (list[2] != "") && (list[3] != "") && (list[4] != ""))
            {
                Text_Data text;
                text.init();
                text.str_Name = list[0];
                text.str_FontName = list[1];
                text.nFontSize = list[2].toInt();
                text.pt = StringToPoint(list[3]);
                text.ptConst = StringToPoint(list[3]);
                text.cFontColor = StringToColor(list[4]);
                vect_TextArray.append(text);
                count_Text++;
            }
            else
            {
                QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 界面显示文本数据格式错误，请检查(1)!    %1").arg(i));
            }
        }
        else
        {
            QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB 界面显示文本数据格式错误，请检查(2)!    %1").arg(i));
        }
    }
    if (num_DLB != count_DLB)
    {
        QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB  界面电流表数据配置个数与读取成功个数不匹配，请检查数据配置合法性!    读取成功个数：%1    配置个数：%2").arg(count_DLB).arg(num_DLB));
    }
    if (num_LSBSD != count_LSBSD)
    {
        QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB  无状态表示灯数据配置个数与读取成功个数不匹配，请检查数据配置合法性!    读取成功个数：%1    配置个数：%2").arg(count_LSBSD).arg(num_LSBSD));
    }
    if (num_Line != count_Line)
    {
        QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB  线段数据配置个数与读取成功个数不匹配，请检查数据配置合法性!    读取成功个数：%1    配置个数：%2").arg(count_LSBSD).arg(num_Line));
    }
    if (num_Text != count_Text)
    {
        QMessageBox::information(NULL, VERSION, QString("InterfaceData_TH_KB  界面显示文本数据配置个数与读取成功个数不匹配，请检查数据配置合法性!    读取成功个数：%1    配置个数：%2").arg(count_Text).arg(num_Text));
    }
    File.clear();
}
int CTh_kb::moveCursor(QPoint p)
{
    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        if (FunBtnArray[i]->rect_Btn.contains(p))
        {
            return i + 1;
        }
    }
    if (rect_FCZK.contains(p))
    {
        return FunBtnArray.size() + 1;
    }
    return 0;
}
bool CTh_kb::OnLButtonDown(QPoint p)
{
    int index = moveCursor(p);
    if (index == FunBtnArray.size() + 1)
    {
        if (isQFDlgOK(Fun_FCZK, strQFPassWork, nQFDlgTitleType))
        {
            if (ModeType == 0x11)
            {
                ModeType = 0x22;
            }
            else
            {
                ModeType = 0x11;
            }
            qDebug() << "非常站控按钮按下";
            return true;
        }
    }
    if (ModeType == 0x11) //分散自律模式屏蔽本站操作
    {
        return true;
    }
    if ((index > 0) && (index <= FunBtnArray.size()))
    {
        index--;
        if (FunBtnArray[index]->nFunCode == Fun_FZMENU)
        {
            nCurrentCode = FunBtnArray[index]->nFunCode;
            FunBtnArray[index]->nDownTime = 0;
            CFuzhuMenuDlg *fuzhuMenuDlg = new CFuzhuMenuDlg();
            fuzhuMenuDlg->init(alarmDlg->isHidden(), alarmBJ_Dlg->isHidden(), qftj_dlg->isHidden(), flag_jtgd, flag_dcwz, flag_wcqdName, flag_dcqdName, flag_xhdName, flag_dcName);
            connect(fuzhuMenuDlg, SIGNAL(signal_FuzhuMenuDlg_AN(int)), this, SLOT(slot_FuzhuMenuDlg_AN(int)));
            fuzhuMenuDlg->exec();
            nCurrentCode = 0;
        }
        else if (FunBtnArray[index]->isHaveQF == true)
        {
            if (isQFDlgOK(FunBtnArray[index]->nFunCode, strQFPassWork, nQFDlgTitleType))
            {
                AddQFTJInfo(0x11, index);
                if (FunBtnArray[index]->nFunCode == Fun_GZTZ)
                {
                    FunBtnArray[index]->isDown = !FunBtnArray[index]->isDown;
                }
                else
                {
                    nCurrentCode = FunBtnArray[index]->nFunCode;
                    FunBtnArray[index]->nDownTime = 0;

                    if ((nCurrentCode == Fun_SDJS) || (nCurrentCode == Fun_SYDZS) || (nCurrentCode == Fun_XYDZS))
                    {
                        emit signal_SendFunBtn(nCurrentCode, FunBtnArray[index]->stationID);
                    }
                }
            }
        }
        else
        {
            if (FunBtnArray[index]->nFunCode == Fun_GZTZ)
            {
                FunBtnArray[index]->isDown = !FunBtnArray[index]->isDown;
            }
            else if (FunBtnArray[index]->nFunCode == Fun_CLEAR)
            {
                nCurrentCode = FunBtnArray[index]->nFunCode;
                FunBtnArray[index]->nDownTime = 0;
                emit signal_SendFunBtn(FunBtnArray[index]->nFunCode, 0xFF);
            }
            else
            {
                nCurrentCode = FunBtnArray[index]->nFunCode;
                FunBtnArray[index]->nDownTime = 0;
            }
        }
        //setSpeechText("功能按钮按下："+FunBtnArray[index]->name1+FunBtnArray[index]->name2,2);
        qDebug() << "功能按钮按下：" << FunBtnArray[index]->name1 << FunBtnArray[index]->name2 << FunBtnArray[index]->stationID;
        return true;
    }
    return false;
}
bool CTh_kb::OnRButtonDown(QPoint p)
{
    if ((pCom_menu != nullptr) && (isHaveRBtnMenu == true))
    {
        pCom_menu->exec(QCursor::pos());
        return true;
    }
    return false;
}
void CTh_kb::slot_FuzhuMenuDlg_AN(int index)
{
    switch (index)
    {
    case 0:
    {
        qDebug() << "辅助按钮" << "溜放显示";
        break;
    }
    case 1:
    {
        qDebug() << "辅助按钮" << "系统维护";
        break;
    }
    case 2:
    {
        setYJprBJDlgShowChange(0x22);
        qDebug() << "辅助按钮" << "预警窗口隐藏";
        break;
    }
    case 3:
    {
        setYJprBJDlgShowChange(0x11);
        qDebug() << "辅助按钮" << "报警窗口隐藏";
        break;
    }
    case 4:
    {
        qDebug() << "辅助按钮" << "切屏操作";
        break;
    }
    case 5:
    {
        qDebug() << "辅助按钮" << "缩屏操作";
        break;
    }
    case 6:
    {
        if (qftj_dlg->isHidden() == true)
        {
            qftj_dlg->show();
        }
        else
        {
            qftj_dlg->hide();
        }
        qDebug() << "辅助按钮" << "破封统计显示";
        break;
    }
    case 7:
    {
        qDebug() << "辅助按钮" << "语音暂停";
        break;
    }
    case 8:
    {
        emit signal_ShowJM(0x88);
        qDebug() << "辅助按钮" << "接通光带显示";
        break;
    }
    case 9:
    {
        emit signal_ShowJM(0x77);
        qDebug() << "辅助按钮" << "道岔位置显示";
        break;
    }
    case 10:
    {
        emit signal_ShowJM(0x44);
        qDebug() << "辅助按钮" << "无岔区段名称隐藏";
        break;
    }
    case 11:
    {
        emit signal_ShowJM(0x66);
        qDebug() << "辅助按钮" << "道岔区段名称隐藏";
        break;
    }
    case 12:
    {
        emit signal_ShowJM(0x22);
        qDebug() << "辅助按钮" << "信号机名称隐藏";
        break;
    }
    case 13:
    {
        emit signal_ShowJM(0x33);
        qDebug() << "辅助按钮" << "道岔名称隐藏";
        break;
    }
    default:
    {
        break;
    }
    }
}
void CTh_kb::slot_YHZSBState(unsigned char yhzs_State, int stationID)
{
    bool isDown_S = false;
    bool isDown_X = false;
    if ((yhzs_State & 0x80) == 0x80)
    {
        isDown_S = true;
    }
    else
    {
        isDown_S = false;
    }
    if ((yhzs_State & 0x40) == 0x40)
    {
        isDown_X = true;
    }
    else
    {
        isDown_X = false;
    }

    for (auto ement : FunBtnArray)
    {
        if ((ement->stationID == stationID) && (ement->nFunCode == Fun_SYDZS))
        {
            ement->isDown = isDown_S;
        }
        else if ((ement->stationID == stationID) && (ement->nFunCode == Fun_XYDZS))
        {
            ement->isDown = isDown_X;
        }
    }
}
void CTh_kb::timePro(bool flag_JTGD, bool flag_DCWZ, bool flag_WCQDName, bool flag_DCQDName, bool flag_XHDName, bool flag_DCName)
{
    nElapsed++;

    flag_jtgd = flag_JTGD;
    flag_dcwz = flag_DCWZ;
    flag_wcqdName = flag_WCQDName;
    flag_dcqdName = flag_DCQDName;
    flag_xhdName = flag_XHDName;
    flag_dcName = flag_DCName;
    //功能按钮按下状态设置
    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        if ((FunBtnArray[i]->nFunCode == Fun_SYDZS) || (FunBtnArray[i]->nFunCode == Fun_XYDZS) || (FunBtnArray[i]->nFunCode == Fun_GZTZ))
        {
            continue;
        }
        if (nCurrentCode == FunBtnArray[i]->nFunCode)
        {
            FunBtnArray[i]->isDown = true;
        }
        else
        {
            FunBtnArray[i]->isDown = false;
        }
    }
    //功能按钮按下延时自复倒计时
    if (nElapsed % 2 == 0)
    {
        for (int i = 0; i < FunBtnArray.size(); i++)
        {
            int timeCount = nFunbtnDownTime;
            if ((FunBtnArray[i]->nFunCode == Fun_SYDZS) || (FunBtnArray[i]->nFunCode == Fun_XYDZS)  || (FunBtnArray[i]->nFunCode == Fun_GZTZ))
            {
                continue;
            }
            if ((FunBtnArray[i]->nFunCode == Fun_CLEAR) || (FunBtnArray[i]->nFunCode == Fun_SDJS))
            {
                timeCount = 1;
            }
            if (FunBtnArray[i]->isDown == true)
            {
                if (FunBtnArray[i]->nDownTime < timeCount)
                {
                    FunBtnArray[i]->nDownTime++;
                }
                else
                {
                    FunBtnArray[i]->isDown = false;
                    nCurrentCode = 0;
                }
            }
        }
    }
    DLBTimer();
    emit signal_upDataQFTJList(vect_QFTJArray);
    speechTextPro();
}
void CTh_kb::SetDLBStart(int id)
{
    for (int i = 0; i < vect_DLBArray.size(); i++)
    {
        if (vect_DLBArray[i]->nStationID == id)
        {
            vect_DLBArray[i]->isRunFlag = true;
            vect_DLBArray[i]->runTime = 0;
        }
    }
}
void CTh_kb::DLBTimer()
{
    for (int i = 0; i < vect_DLBArray.size(); i++)
    {
        if (vect_DLBArray[i]->isRunFlag == true)
        {
            if (vect_DLBArray[i]->runTime >= 4)
            {
                vect_DLBArray[i]->isRunFlag = false;
                vect_DLBArray[i]->runTime = 0;
            }
            else
            {
                vect_DLBArray[i]->runTime++;
            }
        }
    }
}
void CTh_kb::ANCmdOfCTC(int anFunType, int devCode1, int devCode2)
{
    if (anFunType == Fun_GZTZ)
    {
        for (int index = 0; index < FunBtnArray.size(); index++)
        {
            if (FunBtnArray[index]->nFunCode == Fun_GZTZ)
            {
                FunBtnArray[index]->isDown = !FunBtnArray[index]->isDown;
                break;
            }
        }
    }
}

void CTh_kb::timePro()
{
    nElapsed++;
    //功能按钮按下状态设置
    for (int i = 0; i < FunBtnArray.size(); i++)
    {
        if ((FunBtnArray[i]->nFunCode == Fun_SYDZS) || (FunBtnArray[i]->nFunCode == Fun_XYDZS) || (FunBtnArray[i]->nFunCode == Fun_GZTZ))
        {
            continue;
        }
        if (nCurrentCode == FunBtnArray[i]->nFunCode)
        {
            FunBtnArray[i]->isDown = true;
        }
        else
        {
            FunBtnArray[i]->isDown = false;
        }
    }
    //功能按钮按下延时自复倒计时
    if (nElapsed % 2 == 0)
    {
        for (int i = 0; i < FunBtnArray.size(); i++)
        {
            int timeCount = nFunbtnDownTime;
            if ((FunBtnArray[i]->nFunCode == Fun_SYDZS) || (FunBtnArray[i]->nFunCode == Fun_XYDZS)  || (FunBtnArray[i]->nFunCode == Fun_GZTZ))
            {
                continue;
            }
            if ((FunBtnArray[i]->nFunCode == Fun_CLEAR) || (FunBtnArray[i]->nFunCode == Fun_SDJS))
            {
                timeCount = 1;
            }
            if (FunBtnArray[i]->isDown == true)
            {
                if (FunBtnArray[i]->nDownTime < timeCount)
                {
                    FunBtnArray[i]->nDownTime++;
                }
                else
                {
                    FunBtnArray[i]->isDown = false;
                    nCurrentCode = 0;
                }
            }
        }
    }
    DLBTimer();
    emit signal_upDataQFTJList(vect_QFTJArray);
    speechTextPro();
}
void CTh_kb::Draw_DLB(QPainter *painter, long nElapsed, double nDiploid)
{
    QFont font;
    painter->setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < vect_DLBArray.size(); i++)
    {
        //绘制底部
        QString strDL = "";
        font.setFamily("宋体");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::yellow);//设置画笔颜色
        if (vect_DLBArray[i]->isRunFlag == false)
        {
            draw_Pixmap(painter, ":/Pictures/DLB_0.jpg", vect_DLBArray[i]->pt.x(), vect_DLBArray[i]->pt.y(), 151, 151, nDiploid);
            strDL = "0.0";
        }
        else
        {
            if ((vect_DLBArray[i]->runTime == 1) || (vect_DLBArray[i]->runTime == 4))
            {
                draw_Pixmap(painter, ":/Pictures/DLB_1_5.jpg", vect_DLBArray[i]->pt.x(), vect_DLBArray[i]->pt.y(), 151, 151, nDiploid);
                strDL = "1.5";
            }
            else
            {
                draw_Pixmap(painter, ":/Pictures/DLB_3_9.jpg", vect_DLBArray[i]->pt.x(), vect_DLBArray[i]->pt.y(), 151, 151, nDiploid);
                strDL = "3.9";
            }
        }
        painter->drawText(vect_DLBArray[i]->pt.x() + 73 * nDiploid, vect_DLBArray[i]->pt.y() + 105 * nDiploid, "A");
        painter->drawText(vect_DLBArray[i]->pt.x() + 66 * nDiploid, vect_DLBArray[i]->pt.y() + 125 * nDiploid, strDL);

        font.setFamily("宋体");
        font.setPointSize(10 * nDiploid);//字号
        font.setItalic(false);//斜体
        painter->setFont(font);//设置字体
        painter->setPen(Qt::white);//设置画笔颜色
        //绘制文本
        QFontMetrics fm(font);
        QRect rec = fm.boundingRect(vect_DLBArray[i]->strName);
        painter->drawText(vect_DLBArray[i]->pt.x() + 75 * nDiploid - rec.width() / 2, vect_DLBArray[i]->pt.y() + 148 * nDiploid, vect_DLBArray[i]->strName);
    }
}
void CTh_kb::setYJprBJDlgShowChange(int type)
{
    if (type == 0x11)
    {
        if (alarmBJ_Dlg->isHidden() == true)
        {
            alarmBJ_Dlg->show();
        }
        else
        {
            alarmBJ_Dlg->hide();
        }
    }
    else if (type == 0x22)
    {
        if (alarmDlg->isHidden() == true)
        {
            alarmDlg->show();
            if (yjDlgIsShowType == 1)
            {
                alarmState_Dlg->show();
            }
        }
        else
        {
            alarmDlg->hide();
            if (yjDlgIsShowType == 1)
            {
                alarmState_Dlg->hide();
            }
        }
    }
}
void CTh_kb::setAlarmTextADD(int type, QString str)
{
    QColor color = Qt::black;
    if (type == 0x11)
    {
        alarmDlg->add_TSXX(str, color); //预警窗口
    }
    else if (type == 0x22)
    {
        alarmBJ_Dlg->add_TSXX(str, color); //报警窗口
    }
}
void CTh_kb::onClearAction()
{
    nCurrentCode = 0;
    emit signal_SendFunBtn(Fun_CLEAR, 0xFF);
}

void CTh_kb::ClearMenueButton_Slot()
{
    nCurrentCode = 0;
}
