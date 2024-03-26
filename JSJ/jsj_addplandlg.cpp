#include "jsj_addplandlg.h"
#include "ui_jsj_addplandlg.h"

jsj_addplandlg::jsj_addplandlg(unsigned int type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_addplandlg)
{
    ui->setupUi(this);
    layout = new QVBoxLayout(ui->detailPlan);
    layout->addStretch();
    ui->detailPlan->setLayout(layout);

}

jsj_addplandlg::~jsj_addplandlg()
{
    delete ui;
}
#include<QTimeEdit>
#include<QPushButton>
void jsj_addplandlg::AddStaion(QString StationName, unsigned int Position, unsigned int index)
{
    QHBoxLayout * laYout = new QHBoxLayout();
    QLabel * label = new QLabel(ui->detailPlan);
    label->setMinimumSize(91,31);
    label->setText(StationName);
    laYout->addWidget(label);

    laYout->addStretch();
    QLabel * label_ = new QLabel(ui->detailPlan);
    label_->setMinimumSize(91,31);
    label_->setText("计划类型");
    laYout->addWidget(label_);


    QComboBox * PlanCombo = new QComboBox(ui->detailPlan);
    PlanCombo->addItem("接发车",JFC);
    PlanCombo->addItem("始发",SF);
    PlanCombo->addItem("终到",ZD);
    PlanCombo->addItem("通过",TG);
    PlanCombo->setMinimumSize(182,31);
    laYout->addWidget(PlanCombo);
    laYout->addStretch(20);

    laYout->addStretch();
    QLabel * labelGD = new QLabel("股道",ui->detailPlan);
    labelGD->setMinimumSize(91,31);
    laYout->addWidget(labelGD);

    QComboBox * GDCombo = new QComboBox(ui->detailPlan);
    GDCombo->addItem("接发车",JFC);
    GDCombo->addItem("始发",SF);
    GDCombo->addItem("终到",ZD);
    GDCombo->addItem("通过",TG);
    GDCombo->setMinimumSize(182,31);
    laYout->addWidget(GDCombo);
    laYout->addStretch(10);

    QLabel * labelArr = new QLabel("到达时间",ui->detailPlan);
    labelArr->setMinimumSize(91,31);
    laYout->addWidget(labelArr);

    QTimeEdit * ArrEdit = new QTimeEdit(ui->detailPlan);
    ArrEdit->setMinimumSize(182,31);
    laYout->addWidget(ArrEdit);
    laYout->addStretch(10);

    QLabel * labelCF = new QLabel("出发时间",ui->detailPlan);
    labelCF->setMinimumSize(91,31);
    laYout->addWidget(labelCF);

    QTimeEdit * CFEdit = new QTimeEdit(ui->detailPlan);
    CFEdit->setMinimumSize(182,31);
    laYout->addWidget(CFEdit);
    laYout->addStretch(10);

    QPushButton * Button = new QPushButton("确定",ui->detailPlan);
    group->addButton(Button,index);
    Button->setMinimumSize(91,31);
    laYout->addWidget(Button);
    laYout->addStretch(100);
    layout->addLayout(laYout,index);

    Button->installEventFilter(this);
    GetNodeAppend tempAppend;

    if(Position == UpOne)
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = UpOne;
        tempAppend.GetNextlead = new QLabel(ui->detailPlan);
        tempAppend.GetNextlead->setMinimumSize(71,131);
        tempAppend.GetNextlead->setMaximumSize(71,131);
        tempAppend.GetNextlead->installEventFilter(this);

        QHBoxLayout * Midlay = new QHBoxLayout();
        Midlay->addStretch();
        Midlay->addWidget(tempAppend.GetNextlead);
        Midlay->addStretch();

        tempAppend.LeadIndex = index++;
        tempAppend.NextHboxLayout = laYout;
        layout->addLayout(Midlay,tempAppend.LeadIndex);

    }else if (Position == LastOne)
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = LastOne;
        tempAppend.NextHboxLayout = laYout;

    }else
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = MiddleOne;
        tempAppend.GetNextlead = new QLabel(ui->detailPlan);
        tempAppend.GetNextlead->setMinimumSize(71,131);
        tempAppend.GetNextlead->setMaximumSize(71,131);
        tempAppend.GetNextlead->installEventFilter(this);

        tempAppend.LeadIndex = index+1;
        tempAppend.NextHboxLayout = laYout;
        QHBoxLayout * Midlay = new QHBoxLayout();
        Midlay->addStretch();
        Midlay->addWidget(tempAppend.GetNextlead);
        Midlay->addStretch();

        tempAppend.LeadIndex = index++;
        tempAppend.NextHboxLayout = laYout;
        layout->addLayout(Midlay,tempAppend.LeadIndex);
    }
    NodeAppendList.push_back(tempAppend);
}

bool jsj_addplandlg::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        int Size = group->buttons().size();
        for(int i = 0; i < Size;++i)
        {
            if(target == group->buttons()[i])
            {
                Get_YindaoLine_Show(static_cast<QPushButton*>( group->buttons()[i]));
            }
        }
    }
    return false;
}
void jsj_addplandlg::Get_YindaoLine_Show(QPushButton *Btn)
{
    for(int i = 0; i < NodeAppendList.size();++i)
    {
        if(NodeAppendList[i].Btn == Btn)
        {
            if(NodeAppendList[i].isfirst_or_LastOne == UpOne)
            {
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                NodeAppendList[i+1].direct = Down_Ahead;
                CurDirect = Down_Ahead;
                NodeAppendList[i].GetNextlead->show();
                break;
            }
            else if (NodeAppendList[i].isfirst_or_LastOne == LastOne)
            {
                NodeAppendList[i-1].direct = Up_Ahead;
                CurDirect = Up_Ahead;
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                NodeAppendList[i].GetNextlead->show();
                break;
            }else
            {
                unsigned int ahead = NodeAppendList[i].direct;
                if(CurDirect == -1)//说明第一次不是从头或尾开始
                {
                    int tw = GetAhead_twards(i);
                    if(tw == -1)
                    {
                        NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                        NodeAppendList[i].GetNextlead->show();
                        NodeAppendList[i+1].direct = Down_Ahead;


                        NodeAppendList[i-1].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                        NodeAppendList[i-1].GetNextlead->show();
                        NodeAppendList[i-1].direct = Up_Ahead;
                    }


                }else
                {
                    if(CurDirect == Up_Ahead)
                    {
                        NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                        NodeAppendList[i].GetNextlead->show();
                        NodeAppendList[i-1].direct = Up_Ahead;
                    }
                }
            }
        }
    }
}

unsigned int jsj_addplandlg::GetAhead_twards(int index)
{
    unsigned int UpIndex = NodeAppendList[index-1].direct;
    unsigned int DownIndex = NodeAppendList[index+1].direct;
    if(UpIndex == -1 && DownIndex == -1)
        return -1;
    else if (UpIndex!=-1)
        return UpIndex;
    else if (DownIndex!=-1)
        return DownIndex;
}
#include"jsj_plannode.h"
void jsj_addplandlg::SetEditNodeInfo()
{
    for(int i = 0; i < Node->CurMultiPlanInfo.size();++i)
    {
        PalenInfo info = Node->CurMultiPlanInfo[i];
    }
}
