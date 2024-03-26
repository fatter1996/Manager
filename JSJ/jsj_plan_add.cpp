#include "jsj_plan_add.h"
#include "ui_jsj_plan_add.h"

jsj_plan_Add::jsj_plan_Add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_plan_Add)
{
    ui->setupUi(this);
}

jsj_plan_Add::~jsj_plan_Add()
{
    delete ui;
}

void jsj_plan_Add::AddStaion(QString StationName, unsigned int Position, unsigned int index)
{
//    QHBoxLayout * laYout = new QHBoxLayout();
//    QLabel * label = new QLabel(ui->detailPlan);
//    label->setMinimumSize(91,31);
//    label->setText(StationName);
//    laYout->addWidget(label);

//    laYout->addStretch();
//    QLabel * label_ = new QLabel(ui->detailPlan);
//    label_->setMinimumSize(91,31);
//    label_->setText("计划类型");
//    laYout->addWidget(label_);


//    QComboBox * PlanCombo = new QComboBox(ui->detailPlan);
//    PlanCombo->addItem("接发车",JFC);
//    PlanCombo->addItem("始发",SF);
//    PlanCombo->addItem("终到",ZD);
//    PlanCombo->addItem("通过",TG);
//    PlanCombo->setMinimumSize(182,31);
//    laYout->addWidget(PlanCombo);
//    laYout->addStretch(20);

//    laYout->addStretch();
//    QLabel * labelGD = new QLabel("股道",ui->detailPlan);
//    labelGD->setMinimumSize(91,31);
//    laYout->addWidget(labelGD);

//    QComboBox * GDCombo = new QComboBox(ui->detailPlan);
//    GDCombo->addItem("接发车",JFC);
//    GDCombo->addItem("始发",SF);
//    GDCombo->addItem("终到",ZD);
//    GDCombo->addItem("通过",TG);
//    GDCombo->setMinimumSize(182,31);
//    laYout->addWidget(GDCombo);
//    laYout->addStretch(10);

//    QLabel * labelArr = new QLabel("到达时间",ui->detailPlan);
//    labelArr->setMinimumSize(91,31);
//    laYout->addWidget(labelArr);

//    QTimeEdit * ArrEdit = new QTimeEdit(ui->detailPlan);
//    ArrEdit->setMinimumSize(182,31);
//    laYout->addWidget(ArrEdit);
//    laYout->addStretch(10);

//    QLabel * labelCF = new QLabel("出发时间",ui->detailPlan);
//    labelCF->setMinimumSize(91,31);
//    laYout->addWidget(labelCF);

//    QTimeEdit * CFEdit = new QTimeEdit(ui->detailPlan);
//    CFEdit->setMinimumSize(182,31);
//    laYout->addWidget(CFEdit);
//    laYout->addStretch(10);

//    QPushButton * Button = new QPushButton("确定",ui->detailPlan);
//    group->addButton(Button,index);
//    Button->setMinimumSize(91,31);
//    laYout->addWidget(Button);
//    laYout->addStretch(100);
//    layout->addLayout(laYout,index);

//    Button->installEventFilter(this);
//    GetNodeAppend tempAppend;

//    if(Position == UpOne)
//    {
//        tempAppend.Btn = Button;
//        tempAppend.isfirst_or_LastOne = UpOne;
//        tempAppend.GetNextlead = new QLabel(ui->detailPlan);
//        tempAppend.GetNextlead->setMinimumSize(71,131);
//        tempAppend.GetNextlead->setMaximumSize(71,131);
//        tempAppend.GetNextlead->installEventFilter(this);

//        QHBoxLayout * Midlay = new QHBoxLayout();
//        Midlay->addStretch();
//        Midlay->addWidget(tempAppend.GetNextlead);
//        Midlay->addStretch();

//        tempAppend.LeadIndex = index++;
//        tempAppend.NextHboxLayout = laYout;
//        layout->addLayout(Midlay,tempAppend.LeadIndex);

//    }else if (Position == LastOne)
//    {
//        tempAppend.Btn = Button;
//        tempAppend.isfirst_or_LastOne = LastOne;
//        tempAppend.NextHboxLayout = laYout;

//    }else
//    {
//        tempAppend.Btn = Button;
//        tempAppend.isfirst_or_LastOne = MiddleOne;
//        tempAppend.GetNextlead = new QLabel(ui->detailPlan);
//        tempAppend.GetNextlead->setMinimumSize(71,131);
//        tempAppend.GetNextlead->setMaximumSize(71,131);
//        tempAppend.GetNextlead->installEventFilter(this);

//        tempAppend.LeadIndex = index+1;
//        tempAppend.NextHboxLayout = laYout;
//        QHBoxLayout * Midlay = new QHBoxLayout();
//        Midlay->addStretch();
//        Midlay->addWidget(tempAppend.GetNextlead);
//        Midlay->addStretch();

//        tempAppend.LeadIndex = index++;
//        tempAppend.NextHboxLayout = laYout;
//        layout->addLayout(Midlay,tempAppend.LeadIndex);
//    }
//    NodeAppendList.push_back(tempAppend);
}

void jsj_plan_Add::Get_YindaoLine_Show(QPushButton *Btn)
{

}

unsigned int jsj_plan_Add::GetAhead_twards(int index)
{

}

bool jsj_plan_Add::eventFilter(QObject *target, QEvent *event)
{

}
