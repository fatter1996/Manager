#include "jsj_adp.h"
#include "ui_jsj_adp.h"

jsj_adp::jsj_adp(QSize &Size, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::jsj_adp)
{
    ui->setupUi(this);
    this->setMinimumSize(Size);
    ui->scrollArea->setMinimumSize(QSize(Size.width()-10,Size.height()-41));
    setWindowTitle("多站阶段计划添加界面");
    ui->cx_combo->addItem("无超限",NO_CX);
    ui->cx_combo->addItem("一级超限",ONE_CX);
    ui->cx_combo->addItem("二级超限",TWO_CX);
    ui->cx_combo->addItem("三级超限",THREE_CX);
    ui->cx_combo->addItem("超级超限",Super_CX);

    ui->qytype_Combo->addItem("电力",Eletric);
    ui->qytype_Combo->addItem("非电力",UN_Eletric);


    ui->CCLX_Combo->addItem("快速旅客列车",KSLK);
    ui->CCLX_Combo->addItem("普通旅客列车",PTLK);
    ui->CCLX_Combo->addItem("货物列车",HWLC);
    ui->CCLX_Combo->addItem("行包列车",XBLC);
    ui->CCLX_Combo->addItem("动车组",DCZ);
    ui->CCLX_Combo->addItem("特快货物列车",TKHWLC);
    ui->CCLX_Combo->addItem("快速货物列车",KSHWLC);
    ui->CCLX_Combo->addItem("特别旅客快车",TBLK);

    layout = new QVBoxLayout(ui->detailPlan);
    ui->detailPlan->setLayout(layout);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowModality(Qt::ApplicationModal);
    //setStyleSheet("QComboBox{color: rgb(0, 0, 0);}");
}

jsj_adp::~jsj_adp()
{
    delete ui;
}
#include<QPushButton>
void jsj_adp::AddStaion(QString StationName, unsigned int Position, unsigned int index)
{
    if(index == 0)
        Position =UpOne;
    DBSERVICES.info.initInfo();
    QString Name =  DBSERVICES.GetStationId(StationName);
    emit DBSERVICES.GetGDInfo(Name);
    QHBoxLayout * laYout = new QHBoxLayout();
    QLabel * label = new QLabel(ui->detailPlan);
    label->setAlignment(Qt::AlignLeft|Qt::AlignCenter);
    label->setObjectName("车站名称");
    label->setMinimumSize(91,31);
    label->setMaximumSize(91,31);
    label->setText(StationName);
    laYout->addWidget(label);

    laYout->addStretch();
    QLabel * label_ = new QLabel(ui->detailPlan);
    label_->setMinimumSize(91,31);
    label_->setMaximumSize(91,31);
    label_->setText("计划类型");
    label_->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    laYout->addWidget(label_);


    QComboBox * PlanCombo = new QComboBox(ui->detailPlan);
    PlanCombo->setObjectName("接发车类型");
    PlanCombo->addItem("接发车",JFC);
    PlanCombo->addItem("始发",SF);
    PlanCombo->addItem("终到",ZD);
    PlanCombo->addItem("通过",TG);
    PlanCombo->setMinimumSize(182,31);
    PlanCombo->setMaximumSize(182,31);
    laYout->addWidget(PlanCombo);
    laYout->addStretch(20);

    connect(PlanCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(getPlanType(int)));

    QLabel * labelGD = new QLabel("股道",ui->detailPlan);
    labelGD->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    labelGD->setMinimumSize(91,31);
    labelGD->setMaximumSize(91,31);
    laYout->addWidget(labelGD);

    QComboBox * GDCombo = new QComboBox(ui->detailPlan);
    GDCombo->setObjectName("股道名称");


    GDCombo->setMinimumSize(182,31);
    GDCombo->setMaximumSize(182,31);
    laYout->addWidget(GDCombo);
    laYout->addStretch(10);


    QLabel * labelJC = new QLabel("接车方向",ui->detailPlan);
    labelJC->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    labelGD->setMinimumSize(91,31);
    labelGD->setMaximumSize(91,31);
    laYout->addWidget(labelJC);

    QComboBox * JCCombo = new QComboBox(ui->detailPlan);
    JCCombo->setObjectName("接车");
    connect(JCCombo,SIGNAL(activated(int)),this,SLOT(getPlanType_JCFX(int)));
    JCCombo->setMinimumSize(182,31);
    JCCombo->setMaximumSize(182,31);
    laYout->addWidget(JCCombo);
    laYout->addStretch(10);

    QLabel * labelFC = new QLabel("发车方向",ui->detailPlan);
    labelFC->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    labelFC->setMinimumSize(91,31);
    labelFC->setMaximumSize(91,31);
    laYout->addWidget(labelFC);

    QComboBox * FCCombo = new QComboBox(ui->detailPlan);
    FCCombo->setObjectName("发车");
    for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
    {
        GDCombo->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
    }

    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
    {
        JCCombo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
    }
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
    {
        FCCombo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
    }
    FCCombo->setMinimumSize(182,31);
    FCCombo->setMaximumSize(182,31);
    laYout->addWidget(FCCombo);
    laYout->addStretch(10);


    Comfirm_Btn * Button = new Comfirm_Btn("确定",ui->detailPlan);
    group->addButton(Button,index);
    Button->setMinimumSize(91,31);
    Button->setMaximumSize(91,31);
    laYout->addWidget(Button);
    laYout->addStretch(450);
    Button->installEventFilter(this);

    GetNodeAppend tempAppend;
    QVBoxLayout * Vayout = new QVBoxLayout();
    Vayout->addLayout(laYout);
    if(Position == UpOne)
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = UpOne;
        tempAppend.GetNextlead = new ArrowLabel(ui->detailPlan);
        tempAppend.GetNextlead->setMinimumSize(786,76);
        tempAppend.GetNextlead->setMaximumSize(786,76);
        tempAppend.GetNextlead->installEventFilter(this);
        tempAppend.GetNextlead->setProperty("STName",StationName);

        tempAppend.LeadIndex = index++;
        tempAppend.NextHboxLayout = laYout;
        Vayout->addWidget(tempAppend.GetNextlead,Qt::AlignCenter);

    }else if (Position == LastOne)
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = LastOne;
        tempAppend.NextHboxLayout = laYout;
    }else
    {
        tempAppend.Btn = Button;
        tempAppend.isfirst_or_LastOne = MiddleOne;
        tempAppend.GetNextlead = new ArrowLabel(ui->detailPlan);
        tempAppend.GetNextlead->setMinimumSize(786,76);
        tempAppend.GetNextlead->setMaximumSize(786,76);
        tempAppend.GetNextlead->installEventFilter(this);
        tempAppend.GetNextlead->setProperty("STName",StationName);

        tempAppend.LeadIndex = index+1;
        tempAppend.NextHboxLayout = laYout;


        tempAppend.LeadIndex = index++;
        tempAppend.NextHboxLayout = laYout;
        Vayout->addWidget(tempAppend.GetNextlead,Qt::AlignCenter);

    }
    layout->addLayout(Vayout);
    NodeAppendList.push_back(tempAppend);
}
#include"cmsgbox.h"
void jsj_adp::Get_YindaoLine_Show(QPushButton *Btn)
{
    if(NodeAppendList.size()<=1)
    {
        CMsgBox::showMsgBox( "单站不支持操作,请检查",this);
        return;
    }
    for(int i = 0; i < NodeAppendList.size();++i)
    {
        if(NodeAppendList[i].Btn == Btn)
        {
            if(NodeAppendList[i].isfirst_or_LastOne == UpOne)
            {
                if(CurDirect !=-1)
                    return;
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                NodeAppendList[i+1].direct = Down_Ahead;
                CurDirect = Down_Ahead;
                NodeAppendList[i].Btn->is_Select = true;
                NodeAppendList[i+1].Btn->is_Select = true;
                NodeAppendList[i].isGet = true;
                NodeAppendList[i].GetNextlead->is_show = true;//第一次的选取站场名称
                break;
            }
            else if (NodeAppendList[i].isfirst_or_LastOne == LastOne)
            {
                if(CurDirect !=-1)
                    return;
                NodeAppendList[i-1].direct = Up_Ahead;
                CurDirect = Up_Ahead;
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                NodeAppendList[i].Btn->is_Select = true;
                NodeAppendList[i-1].Btn->is_Select = true;
                NodeAppendList[i].isGet = true;
                NodeAppendList[i].GetNextlead->is_show = true;//第一次的选取站场名称
                break;
            }else
            {
                if(CurDirect == -1)//说明第一次不是从头或尾开始
                {
                    int Size = NodeAppendList.size();
                    int tw = GetAhead_twards(i);
                    if(tw == -1)
                    {
                        NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                        NodeAppendList[i].Btn->is_Select = true;
                        NodeAppendList[i].GetNextlead->is_show = true;
                        if((i+1)<Size)
                            NodeAppendList[i+1].direct = Down_Ahead;

                        if(i-1>=0)
                        {
                            NodeAppendList[i-1].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                            NodeAppendList[i-1].direct = Up_Ahead;
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < NodeAppendList.size();++i)
    {
        NodeAppendList[i].Btn->hide();
    }
}

int jsj_adp::GetFirst_Clicked_Btn(QPushButton *Btn)
{
    int size = NodeAppendList.size();
    for(int i = 0; i < size;++i)
    {
        if(NodeAppendList[i].Btn == Btn)
            return i;
    }
    return -1;
}

void jsj_adp::Get_ArrowShow(int Arrow_Forward, int var, Comfirm_Btn *Btn)
{
    qDebug()<<"站:"<<Btn->property("STName");
    //Btn->is_Select = true;//向上的时候 默认获取下面一个箭头 而向下的时候 需要+1 +2
    int Size = NodeAppendList.size();
    int index = -1;
    if(CurDirect == -1)//说明第一次是从中间开始选取的
    {
        if(Arrow_Forward == Up_Ahead)
        {
            for(int i = var;i >=0;--i)
            {
                if(NodeAppendList[i].Btn == Btn)
                {
                    index = i;
                    CurDirect = Up_Ahead;
                    NodeAppendList[i].isGet = true;
                    goto here;
                }
            }

        }else if (Arrow_Forward == Down_Ahead)
        {
            for(int ii = var;ii<NodeAppendList.size();++ii)
            {
                if(NodeAppendList[ii].Btn == Btn)
                {
                    index = ii;
                    CurDirect = Down_Ahead;
                    NodeAppendList[ii].isGet = true;
                    goto here;
                }
            }
        }
here:{
            if(CurDirect == Down_Ahead)
            {

                ArrowLabel * Uplabel = NodeAppendList[index-1].GetNextlead;
                ArrowLabel * dowLabel = NodeAppendList[index+1].GetNextlead;
                if(Uplabel!=nullptr)
                    Uplabel->setStyleSheet("");
                if(dowLabel!=nullptr)
                {
                    dowLabel->is_show = true;
                    NodeAppendList[index+1].Btn->is_Select = true;
                }
                return;
            }

            else if (CurDirect == Up_Ahead)
            {

                ArrowLabel * Uplabel = nullptr;int var_change = -1;
                if(index>0)
                    var_change = index-1;
                else
                    var_change = 0;
                Uplabel = NodeAppendList[index].GetNextlead;
                ArrowLabel * dowLabel = NodeAppendList[index+1].GetNextlead;
                if(Uplabel!=nullptr)
                {
                    Uplabel->is_show = true;
                    NodeAppendList[index].Btn->is_Select = true;
                }

                if(dowLabel!=nullptr)
                    dowLabel->setStyleSheet("");
                return;
            }
        }
    }else
    {
        for(int i = 0 ; i < Size;++i)
        {
            if(NodeAppendList[i].Btn == Btn)
            {
                if(CurDirect == Up_Ahead)
                {
                    int var = i-1;
                    {
                        if(var <0)
                            var = 0;
                        if(!NodeAppendList[i].isGet)//判断之前一个是否已经选取并显示
                            return;
                        {

                            NodeAppendList[var].GetNextlead->setStyleSheet
                                    ("border-image: url(:/new/jsj/rc/目录/向上.png);");
                            NodeAppendList[var].Btn->is_Select = true;
                            NodeAppendList[var].isGet = true;
                        }
                    }
                }
                else if (CurDirect == Down_Ahead)
                {
                    if(!NodeAppendList[i].isGet)//判断之前一个是否已经选取并显示
                        return;
                    int var_Change = i+1;
                    int Var_Append = i+2;
                    if(Var_Append>=Size)
                    {
                        NodeAppendList[var_Change].Btn->is_Select = true;
                        NodeAppendList[var_Change].isGet = true;
                    }else
                    {
                        NodeAppendList[var_Change].Btn->is_Select = true;
                        NodeAppendList[Var_Append].Btn->is_Select = true;
                        NodeAppendList[var_Change].isGet = true;
                        NodeAppendList[Var_Append].isGet = true;
                    }

                    NodeAppendList[var_Change].GetNextlead->setStyleSheet
                            ("border-image: url(:/new/jsj/rc/目录/向下.png);");
                }
            }
        }
    }
}


unsigned int jsj_adp::GetAhead_twards(int index)
{
    int direct = NodeAppendList[index].direct;
    return direct;
}
void jsj_adp::SetInit_Info(unsigned int type)
{
    TypeOpreate = type;
    ui->pushButton->show();
    if(type == Edit)
    {
        EditExistPlan();
        layout->addStretch();
    }
    else if(type == APPEND) {
        group = new QButtonGroup(this);
        ui->pushButton->setText("确认添加");
        int Size = DBSERVICES.CurBackInfoList.size();
        for(int i = 0 ; i < Size;++i)
        {
            if(!DBSERVICES.CurBackInfoList[i].isAppend())
                continue;
            QString Name = DBSERVICES.CurBackInfoList[i].StationName;
            if(i == 0)
                AddStaion(Name,UpOne,i);
            else if (i == Size - 1 )
                AddStaion(Name,LastOne,i);
            else
                AddStaion(Name,MiddleOne,i);
        }
        layout->addStretch();
    }
}
#include"jsj_plannode.h"
void jsj_adp::EditExistPlan()
{
    ui->checi_Edit->setText(Node->PlanName);
    ui->cx_combo->setCurrentText(Node->ChaoXianInfo);
    ui->qytype_Combo->setCurrentText(Node->QY_type);
    ui->CCLX_Combo->setCurrentText(Node->YunxingLeixing);
    ui->checi_Edit_2->setText(Node->Checi);
    ui->checi_Edit->setEnabled(false);
    ui->cx_combo->setEnabled(false);
    ui->qytype_Combo->setEnabled(false);
    ui->CCLX_Combo->setEnabled(false);

    int Size = Node->CurMultiPlanInfo.size();
    for(int i = 0 ; i < Size;++i)
    {
        if(Node->CurMultiPlanInfo[i].jfc_TYPE == "")
            continue;
        if(i == Size-1)
            AddExist_Station(Node->CurMultiPlanInfo[i],i,true);
        else
            AddExist_Station(Node->CurMultiPlanInfo[i],i,false);
    }
    layout->addStretch();
}

void jsj_adp::AddExist_Station(PalenInfo &Plae, int index, bool isEnd)
{
    QHBoxLayout * laYout = new QHBoxLayout();
    QLabel * label = new QLabel(ui->detailPlan);
    label->setMinimumSize(91,31);
    label->setMaximumSize(91,31);
    label->setText(Plae.StationName);
    label->setAlignment(Qt::AlignLeft|Qt::AlignCenter);
    laYout->addWidget(label);

    laYout->addStretch();
    QLabel * label_ = new QLabel(ui->detailPlan);
    label_->setMinimumSize(91,31);
    label_->setMaximumSize(91,31);
    label_->setText("计划类型");
    label_->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    laYout->addWidget(label_);


    QComboBox * PlanCombo = new QComboBox(ui->detailPlan);
    PlanCombo->setObjectName("接发车类型");
    PlanCombo->addItem("接发车",JFC);
    PlanCombo->addItem("始发",SF);
    PlanCombo->addItem("终到",ZD);
    PlanCombo->addItem("通过",TG);
    PlanCombo->setMinimumSize(120,31);
    PlanCombo->setMaximumSize(120,31);
    laYout->addWidget(PlanCombo);
    PlanCombo->setCurrentText(Plae.jfc_TYPE);
    PlanCombo->setStyleSheet("color:rgb(0,0,0)");
    PlanCombo->setEnabled(false);
    laYout->addStretch();

    QLabel * labelGD = new QLabel("股道",ui->detailPlan);
    labelGD->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    labelGD->setMinimumSize(91,31);
    labelGD->setMaximumSize(91,31);
    laYout->addWidget(labelGD);

    DBSERVICES.info.initInfo();
    QString Name =  DBSERVICES.GetStationId(Plae.StationName);
    emit DBSERVICES.GetGDInfo(Name);
    QComboBox * GDCombo = new QComboBox(ui->detailPlan);
    GDCombo->setObjectName("股道名称");
    //GDCombo->addItems(DBSERVICES.info.GDList);
    GDCombo->setMinimumSize(91,31);
    GDCombo->setMaximumSize(91,31);
    laYout->addWidget(GDCombo);
    laYout->addStretch(10);


    QLabel * labelJC = new QLabel("接车方向",ui->detailPlan);
    labelGD->setMinimumSize(91,31);
    labelGD->setMaximumSize(91,31);
    laYout->addWidget(labelJC);

    QComboBox * JCCombo = new QComboBox(ui->detailPlan);
    JCCombo->setObjectName("接车");
    //JCCombo->addItems(DBSERVICES.info.jcList);
    JCCombo->setMinimumSize(91,31);
    JCCombo->setMaximumSize(91,31);
    laYout->addWidget(JCCombo);
    laYout->addStretch();

    QLabel * labelFC = new QLabel("发车方向",ui->detailPlan);
    labelFC->setAlignment(Qt::AlignRight|Qt::AlignCenter);
    labelFC->setMinimumSize(91,31);
    labelFC->setMaximumSize(91,31);
    laYout->addWidget(labelFC);

    QComboBox * FCCombo = new QComboBox(ui->detailPlan);
    FCCombo->setObjectName("发车");
    //FCCombo->addItems(DBSERVICES.info.FcList);
    for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
    {
        GDCombo->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
    }
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
    {
        JCCombo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
    }
    for(int i = 0; i < DBSERVICES.info.FCL.size();++i)
    {
        FCCombo->addItem(DBSERVICES.info.FCL[i].FCName,DBSERVICES.info.FCL[i].FCIndex);
    }
    FCCombo->setMinimumSize(91,31);
    FCCombo->setMaximumSize(91,31);
    laYout->addWidget(FCCombo);
    laYout->addStretch();

    if(Plae.jfc_TYPE == "始发")
    {
        JCCombo->setEnabled(false);
        JCCombo->setCurrentText(Plae.JC_Info);
        FCCombo->setEnabled(true);
        FCCombo->setCurrentText(Plae.FC_Info);
    }
    else if(Plae.jfc_TYPE == "接发车")
    {
        JCCombo->setEnabled(true);
        JCCombo->setCurrentText(Plae.JC_Info);
        FCCombo->setEnabled(true);
        FCCombo->setCurrentText(Plae.FC_Info);
    }
    else if(Plae.jfc_TYPE == "终到")
    {
        JCCombo->setEnabled(true);
        JCCombo->setCurrentText(Plae.JC_Info);
        FCCombo->setEnabled(false);
        FCCombo->setCurrentText(Plae.FC_Info);
    }
    else if(Plae.jfc_TYPE == "通过")
    {
        JCCombo->setEnabled(true);
        JCCombo->setCurrentText(Plae.JC_Info);
        FCCombo->setEnabled(false);
        FCCombo->setCurrentText(Plae.FC_Info);
    }

    QLabel * labelArr = new QLabel("出发时间",ui->detailPlan);
    labelArr->setMinimumSize(91,31);
    laYout->addWidget(labelArr);

    QTime Arrive = QTime::fromString(Plae.ArriveTime,"hh:mm");
    qDebug()<<Arrive;
    MyTimeEdit * ArrEdit = new MyTimeEdit(Arrive,ui->detailPlan);
    ArrEdit->setDisplayFormat("hh:mm");
    ArrEdit->setObjectName("到达时间");
    ArrEdit->setMinimumSize(150,31);
    ArrEdit->setMaximumSize(150,31);
    qDebug()<<"到达:"<<Plae.ArriveTime;
    laYout->addWidget(ArrEdit);
    laYout->addStretch();
    ArrEdit->setEnabled(false);

    QLabel * labelCF = new QLabel("到达时间",ui->detailPlan);
    labelCF->setMinimumSize(91,31);
    labelCF->setMaximumSize(91,31);
    laYout->addWidget(labelCF);

    QTime tt = QTime::fromString(Plae.ChuFaTime,"hh:mm");
    MyTimeEdit * CFEdit = new MyTimeEdit(tt,ui->detailPlan);

    CFEdit->setDisplayFormat("hh:mm");
    CFEdit->setObjectName("出发时间");
    CFEdit->setMinimumSize(150,31);
    CFEdit->setMaximumSize(150,31);
    laYout->addWidget(CFEdit);
    laYout->addStretch();
    CFEdit->setEnabled(false);

    QVBoxLayout * V_layout = new QVBoxLayout();
    V_layout->addLayout(laYout);
    if(!isEnd)
    {
        QLabel * Next = new QLabel(ui->detailPlan);
        Next->setProperty("index",index);
        Next->setMinimumSize(1324,69);
        Next->setMaximumSize(1324,69);
        if(CurDirect == Up_Ahead)
            Next->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png)");
        else
            Next->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png)");
        V_layout->addWidget(Next);
    }
    layout->addLayout(V_layout,index);

}

bool jsj_adp::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        int Size = group->buttons().size();
        for(int i = 0; i < Size;++i)
        {
            if(target == group->buttons()[i])
            {
                Get_YindaoLine_Show(static_cast<QPushButton*>( group->buttons()[i]));
                TakeFirst_One = GetFirst_Clicked_Btn(static_cast<QPushButton*>( group->buttons()[i]));
                return false;
            }
        }
        Size = NodeAppendList.size();
        for(int i = 0 ; i < Size;++i)
        {
            if(target == NodeAppendList[i].GetNextlead)
            {
                int Var = -1;
                if(i  == TakeFirst_One)
                    Var = Down_Ahead;
                else if (i+1  == TakeFirst_One)
                    Var = Up_Ahead;
                else
                    Var = MiddleOne;
                Get_ArrowShow(Var,i,static_cast<Comfirm_Btn*>( NodeAppendList[i].Btn));
                return false;
            }
        }
    }

    return false;
}
void jsj_adp::UpdateExistInfo()
{
    int Size = layout->children().size();
    for(int i = 0; i < Size;++i)
    {
        QVBoxLayout * formLayout = qobject_cast<QVBoxLayout*>(layout->children()[i]);
        if (formLayout != nullptr)
        {
            //int itemCount =  formLayout->count();
            int Length = formLayout->children().size();
            for(int z = 0; z < Length;++z)
            {
                QHBoxLayout*hLayout=qobject_cast<QHBoxLayout*>(formLayout->children()[z]);
                if(hLayout!=nullptr)
                {
                    int Cout = hLayout->count();
                    for(int itemIndex = 0; itemIndex < Cout;itemIndex++)
                    {
                        QLayoutItem * item = hLayout->itemAt(itemIndex);
                        QLabel * label = qobject_cast<QLabel*>(item->widget());
                        if(label!=nullptr)
                        {
                            QString obj = label->objectName();QString text = label->text();
                            if(obj.contains("车站名称"))
                                Node->CurMultiPlanInfo[i].StationName = text;
                        }
                        QComboBox * combo = qobject_cast<QComboBox*>(item->widget());
                        if (combo != nullptr)
                        {
                            if (combo->currentText().isEmpty())
                                return;
                            else
                            {
                                QString Obj = combo->objectName();
                                qDebug()<<"控件:"<<Obj;
                                QString text = combo->currentText();
                                unsigned int CurIndex = combo->currentData().toUInt();
                                if(Obj.contains("接发车"))
                                {
                                    Node->CurMultiPlanInfo[i].jfc_TYPE = text;
                                    Node->CurMultiPlanInfo[i].JFC_Data = CurIndex;
                                }
                                else if (Obj.contains("股道"))
                                {
                                    Node->CurMultiPlanInfo[i].GDSetName = text;
                                    Node->CurMultiPlanInfo[i].GDIndex = CurIndex;
                                }
                                else if (Obj==("接车"))
                                {
                                    Node->CurMultiPlanInfo[i].JC_Info = text;
                                    Node->CurMultiPlanInfo[i].JCIndex = CurIndex;
                                }
                                else if (Obj==("发车"))
                                {
                                    Node->CurMultiPlanInfo[i].FC_Info = text;
                                    Node->CurMultiPlanInfo[i].FCIndex = CurIndex;
                                }
                            }
                        }
                        MyTimeEdit * time = qobject_cast<MyTimeEdit*>(item->widget());
                        if(time != nullptr)
                        {
                            if(time->text().isEmpty())
                                return;
                            else
                            {
                                QString timeStr = time->text();
                                QString Obj = time->objectName();
                                if(Obj.contains("到达"))
                                {
                                    QDate Arrive = QDate::currentDate();
                                    QTime time_ = time->time();
                                    Node->CurMultiPlanInfo[i].ArriveTime = timeStr;
                                    Node->CurMultiPlanInfo[i].Arrivetime_[0]=Arrive.year();
                                    Node->CurMultiPlanInfo[i].Arrivetime_[1]=Arrive.month();
                                    Node->CurMultiPlanInfo[i].Arrivetime_[2]=Arrive.day();
                                    Node->CurMultiPlanInfo[i].Arrivetime_[3]=time_.hour();
                                    Node->CurMultiPlanInfo[i].Arrivetime_[4]=time_.minute();
                                    Node->CurMultiPlanInfo[i].Arrivetime_[5]=time_.second();
                                }
                                else if (Obj.contains("出发"))
                                {
                                    Node->CurMultiPlanInfo[i].ChuFaTime = timeStr;

                                    QDate chufa = QDate::currentDate();
                                    QTime time_ = time->time();
                                    Node->CurMultiPlanInfo[i].ArriveTime = timeStr;
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[0]=chufa.year();
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[1]=chufa.month();
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[2]=chufa.day();
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[3]=time_.hour();
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[4]=time_.minute();
                                    Node->CurMultiPlanInfo[i].ChuFaTime_[5]=time_.second();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#include<QMessageBox>
void jsj_adp::Insert_Append_PlanInfo()
{
    int Size = layout->children().size();
    for(int i = 0; i < Size;++i)
    {
        QVBoxLayout * formLayout = qobject_cast<QVBoxLayout*>(layout->children()[i]);
        PalenInfo tempPale;
        if (formLayout != nullptr)
        {
            GetInsertLay(formLayout,tempPale);
        }
    }
    DBSERVICES.PlanNodeList.push_back(Node);
}
bool jsj_adp::Bool_ExistSameOne(QString PlanName)
{
    for(int i = 0 ; i < DBSERVICES.PlanNodeList.size();++i)
    {
        if(PlanName == DBSERVICES.PlanNodeList[i]->PlanName)
            return false;
        else
            continue;
    }
    return true;
}
void jsj_adp::SetSatisified_Info_multiPlan(QString StationName)
{

    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(DBSERVICES.CurBackInfoList[i].StationName == StationName)
        {
            CurList = DBSERVICES.CurBackInfoList[i].TGList;
            break;
        }
    }

}


unsigned int jsj_adp::GetIndexByName_Multi(QString Name)
{
    for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
    {
        if(DBSERVICES.info.GDL[i].GDName == Name)
            return DBSERVICES.info.GDL[i].GDIndex;
    }
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
    {
        if(DBSERVICES.info.JCL[i].JCName == Name)
            return DBSERVICES.info.JCL[i].JCIndex;
    }
    for(int i = 0; i < DBSERVICES.info.FCL.size();++i)
    {
        if(DBSERVICES.info.FCL[i].FCName == Name)
            return DBSERVICES.info.FCL[i].FCIndex;
    }
    return 0xff;
}
QString jsj_adp::GetTypeByLayout(QHBoxLayout *lay)
{
    for(int i = 0; i < lay->count();++i)
    {
        QLayoutItem * item = lay->itemAt(i);
        QComboBox *box = static_cast<QComboBox*>(item->widget());
        if(box!=nullptr)
        {
            QString GetName = box->objectName();
            if(GetName =="接发车类型")
            {
                return box->currentText();
            }
        }
    }
    return "";
}
void jsj_adp::GetOtherInfo(QHBoxLayout *lay, QComboBox *box_)
{
    QString Name = box_->objectName();
    QString text = box_->currentText();

    if(Name == "接车")
    {
        if(GetTypeByLayout(lay) == "通过")
        {
            for(int i = 0; i < lay->count();++i)
            {
                QLayoutItem * item = lay->itemAt(i);
                QComboBox *box = static_cast<QComboBox*>(item->widget());
                if(box!=nullptr)
                {
                    QString GetName = box->objectName();
                    if(GetName == "股道名称")
                    {
                        box->clear();
                        box->setEnabled(false);
                        for(int i = 0; i < CurList.size();++i)
                        {
                            if(CurList[i].JC_Info == text)
                            {
                                box->addItem(CurList[i].GD_Info,GetIndexByName_Multi(CurList[i].GD_Info));
                            }
                        }
                    }
                    else if (GetName == "发车")
                    {
                        box->clear();
                        box->setEnabled(false);
                        for(int i = 0; i < CurList.size();++i)
                        {
                            if(CurList[i].JC_Info == text)
                            {
                                box->addItem(CurList[i].FC_Info,GetIndexByName_Multi(CurList[i].FC_Info));
                            }
                        }
                    }
                }
            }
        }
    }else
    {
        for(int i = 0; i < lay->count();++i)
        {
            QLayoutItem * item = lay->itemAt(i);
            QComboBox *box = static_cast<QComboBox*>(item->widget());
            if(box!=nullptr)
            {
                QString GetName = box->objectName();
                if(Name =="接发车类型")
                {
                    if(text == "接发车")
                    {
                        if(GetName == "接车")
                            box->setEnabled(true);
                        else if (GetName == "发车")
                            box->setEnabled(true);
                        else if (GetName == "股道名称")
                        {
                            box->clear();
                            box->setEnabled(true);
                            for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
                            {
                                box->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
                            }
                        }

                    }
                    else if (text == "始发")
                    {
                        if(GetName == "接车")
                            box->setEnabled(false);
                        else if (GetName == "发车")
                            box->setEnabled(true);
                        else if (GetName == "股道名称")
                        {
                            box->clear();
                            box->setEnabled(true);
                            for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
                            {
                                box->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
                            }
                        }
                    }
                    else if (text == "终到")
                    {
                        if(GetName == "接车")
                            box->setEnabled(true);
                        else if (GetName == "发车")
                            box->setEnabled(false);
                        else if (GetName == "股道名称")
                        {
                            box->clear();
                            box->setEnabled(true);
                            for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
                            {
                                box->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
                            }
                        }
                    }
                    else if (text == "通过")
                    {
                        if(GetName == "接车")
                        {
                            box->clear();
                            box->setEnabled(true);
                            for(int z = 0;z<CurList.size();++z)
                            {
                                box->addItem(CurList[z].JC_Info,GetIndexByName_Multi(CurList[z].JC_Info));
                            }
                        }
                        if (GetName == "发车")
                        {
                            box->clear();
                            box->setEnabled(false);
                            for(int z = 0;z<CurList.size();++z)
                            {
                                box->addItem(CurList[z].FC_Info,GetIndexByName_Multi(CurList[z].FC_Info));
                            }
                        }

                        if (GetName == "股道名称")
                        {
                            box->setEnabled(false);
                            for(int z = 0;z<CurList.size();++z)
                            {
                                box->addItem(CurList[z].GD_Info,GetIndexByName_Multi(CurList[z].GD_Info));
                            }
                        }
                    }
                }
            }
        }
    }
}
QList<Jc_XHJ> jsj_adp::GetItem(QString arg1)
{
    QList<Jc_XHJ> tempList = DBSERVICES.info.JCL;
    QString str = arg1.left(1);
    for(int i = 0 ; i < tempList.size();++i)
    {
        if(tempList[i].JCName.contains(str))
        {
            tempList.removeAt(i);
            i--;
        }
    }
    return tempList;
}
void jsj_adp::SetOtherInfoAppend(QHBoxLayout *lay, PalenInfo &tempPale)
{
    int itemcout = lay->count();
    for(int ii = 0 ; ii < itemcout;++ii)
    {
        QLayoutItem * item = lay->itemAt(ii);
        QLabel * label = qobject_cast<QLabel*>(item->widget());

        if(label!=nullptr)
        {
            QString obj = label->objectName();QString text = label->text();
            if(obj.contains("车站名称"))
            {
                tempPale.StationName = text;
                for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
                {
                    if(DBSERVICES.CurBackInfoList[i].StationName == tempPale.StationName)
                    {
                        tempPale.Stationid = DBSERVICES.CurBackInfoList[i].StationId;
                        break;
                    }
                }
            }
        }
        QComboBox * combo = qobject_cast<QComboBox*>(item->widget());
        if (combo != nullptr)
        {
            if (combo->currentText().isEmpty())
                return;
            else
            {
                QString Obj = combo->objectName();
                QString text = combo->currentText();
                unsigned int CurIndex = combo->currentData().toUInt();
                if(Obj.contains("接发车"))
                {
                    tempPale.jfc_TYPE = text;
                    tempPale.JFC_Data = CurIndex;
                }
                else if (Obj.contains("股道"))
                {
                    tempPale.GDSetName = text;
                    tempPale.GDIndex = CurIndex;
                }
                else if (Obj.contains("接车"))
                {
                    tempPale.JC_Info = text;
                    tempPale.JCIndex = CurIndex;
                }
                else if (Obj.contains("发车"))
                {
                    tempPale.FC_Info = text;
                    tempPale.FCIndex = CurIndex;
                }
            }
        }
        MyTimeEdit * time = qobject_cast<MyTimeEdit*>(item->widget());
        if(time != nullptr)
        {
            if(time->text().isEmpty())
                return;
            else
            {
                QString timeStr = time->text();
                QString Obj = time->objectName();
                if(Obj.contains("到达"))
                {
                    QDate Arrive = time->date();
                    QTime time_ = time->time();
                    tempPale.ArriveTime = timeStr;
                    tempPale.Arrivetime_[0]=Arrive.year();
                    tempPale.Arrivetime_[1]=Arrive.month();
                    tempPale.Arrivetime_[2]=Arrive.day();
                    tempPale.Arrivetime_[3]=time_.hour();
                    tempPale.Arrivetime_[4]=time_.minute();
                    tempPale.Arrivetime_[5]=time_.second();
                }
                else if (Obj.contains("出发"))
                {
                    tempPale.ChuFaTime = timeStr;

                    QDate chufa = time->date();
                    QTime time_ = time->time();
                    tempPale.ArriveTime = timeStr;
                    tempPale.ChuFaTime_[0]=chufa.year();
                    tempPale.ChuFaTime_[1]=chufa.month();
                    tempPale.ChuFaTime_[2]=chufa.day();
                    tempPale.ChuFaTime_[3]=time_.hour();
                    tempPale.ChuFaTime_[4]=time_.minute();
                    tempPale.ChuFaTime_[5]=time_.second();
                }
            }
        }
    }
}


void jsj_adp::GetInsertLay(QVBoxLayout *formLayout, PalenInfo &tempPale)
{

    int itemCount =  formLayout->count();
    for(int itemIndex = 0; itemIndex < itemCount; itemIndex++)
    {
        QLayoutItem * item = formLayout->itemAt(itemIndex);
        QHBoxLayout * layout = qobject_cast<QHBoxLayout*>(item->layout());
        if(layout!=nullptr)
        {
            int itemcout = layout->count();
            for(int ii = 0 ; ii < itemcout;++ii)
            {
                QLayoutItem * item = layout->itemAt(ii);
                {
                    Comfirm_Btn *Btn = qobject_cast<Comfirm_Btn*>(item->widget());
                    if(Btn == nullptr)
                        continue;
                    if(Btn->is_Select)
                    {
                        SetOtherInfoAppend(layout,tempPale);
                        Node->CurMultiPlanInfo.push_back(tempPale);
                        return;
                    }

                }
            }
        }
    }
}


void jsj_adp::GetTimeAppend(int OperateCmd)
{
    if(OperateCmd == APPEND)
    {
        int Size = Node->CurMultiPlanInfo.size();
        if(CurDirect == Down_Ahead){
            for (int i =0;i < Size;++i) {
                SetTimeDetailInfo(i,Node->CurMultiPlanInfo[i]);
            }
        }
        else if (CurDirect == Up_Ahead){
            for(int i = Size - 1;i>=0;--i){
                SetTimeDetailInfo(i,Node->CurMultiPlanInfo[i]);
            }
        }

    }
}
void jsj_adp::GetTimeInit(PalenInfo &tempPale)
{
    if(tempPale.ChuFaTime!="")
    {
        QTime tm=QTime::fromString(tempPale.ChuFaTime,"hh:mm");
        QDate dt = QDate::currentDate();

        tempPale.ChuFaTime_[0] = dt.year();
        tempPale.ChuFaTime_[1] = dt.month();
        tempPale.ChuFaTime_[2] = dt.day();
        tempPale.ChuFaTime_[3] = tm.hour();
        tempPale.ChuFaTime_[4] = tm.minute();
        tempPale.ChuFaTime_[5] = tm.second();
    }
    if(tempPale.ArriveTime!="")
    {
        QTime tm=QTime::fromString(tempPale.ArriveTime,"hh:mm");
        QDate dt = QDate::currentDate();

        tempPale.Arrivetime_[0] = dt.year();
        tempPale.Arrivetime_[1] = dt.month();
        tempPale.Arrivetime_[2] = dt.day();
        tempPale.Arrivetime_[3] = tm.hour();
        tempPale.Arrivetime_[4] = tm.minute();
        tempPale.Arrivetime_[5] = tm.second();
    }
}
void jsj_adp::SetTimeDetailInfo(int var, PalenInfo &tempPale)
{
    int Size = Node->CurMultiPlanInfo.size();
    QString Type = tempPale.jfc_TYPE;
    QTime time = QTime::currentTime();
    if(CurDirect == Down_Ahead)
    {
        if(var == 0x00)
        {
            if(Type == "终到")
            {
                tempPale.ChuFaTime = "";
                tempPale.ArriveTime = time.toString("hh:mm");

            }
            else if (Type == "始发")
            {
                tempPale.ChuFaTime = time.toString("hh:mm");
                tempPale.ArriveTime = "";
            }
            else if (Type == "接发车")
            {

                QTime Arrtime = time.addSecs(240);
                tempPale.ChuFaTime =  time.toString("hh:mm");
                tempPale.ArriveTime = Arrtime.toString("hh:mm");
            }
            else if (Type == "通过")
            {
                tempPale.ChuFaTime =  time.toString("hh:mm");
                tempPale.ArriveTime = time.toString("hh:mm");
            }
        }else
        {
            QString LastType = Node->CurMultiPlanInfo[var-1].jfc_TYPE;
            QTime LastTime;
            if(LastType !="始发")
                LastTime = QTime::fromString(Node->CurMultiPlanInfo[var-1].ArriveTime,"hh:mm");
            else
                LastTime = QTime::currentTime().addSecs(240);
            QString Type = Node->CurMultiPlanInfo[var].jfc_TYPE;
            if(Type == "终到")
            {
                tempPale.ChuFaTime = "";
                tempPale.ArriveTime = LastTime.addSecs(240).toString("hh:mm");
            }
            else if (Type == "始发")
            {
                tempPale.ChuFaTime = time.toString("hh:mm");
                tempPale.ArriveTime = "";
            }
            else if (Type == "接发车")
            {
                QTime Arrtime = LastTime;
                tempPale.ChuFaTime =  Arrtime.toString("hh:mm");
                tempPale.ArriveTime = Arrtime.addSecs(240).toString("hh:mm");
            }
            else if (Type == "通过")
            {
                QTime Alltime =  LastTime.addSecs(240);
                tempPale.ChuFaTime =  Alltime.toString("hh:mm");
                tempPale.ArriveTime = Alltime.toString("hh:mm");
            }
        }
    }else
    {
        if(var == Size-1)
        {
            if(Type == "终到")
            {
                tempPale.ChuFaTime = "";
                tempPale.ArriveTime = time.toString("hh:mm");
            }
            else if (Type == "始发")
            {
                tempPale.ChuFaTime = time.toString("hh:mm");
                tempPale.ArriveTime = "";
            }
            else if (Type == "接发车")
            {
                QTime Arrtime = time.addSecs(240);
                tempPale.ChuFaTime  =  time.toString("hh:mm");
                tempPale.ArriveTime = Arrtime.toString("hh:mm");
            }
            else if (Type == "通过")
            {
                tempPale.ChuFaTime =  time.toString("hh:mm");
                tempPale.ArriveTime = time.toString("hh:mm");
            }
        }else
        {
            if(var+1 >= Size)
                return;
            QString LastType = Node->CurMultiPlanInfo[var+1].jfc_TYPE;
            QTime LastTime;
            if(LastType !="始发")
                LastTime = QTime::fromString(Node->CurMultiPlanInfo[var+1].ArriveTime,"hh:mm");
            else
                LastTime = QTime::currentTime().addSecs(240);
            QString Type = Node->CurMultiPlanInfo[var].jfc_TYPE;
            if(Type == "终到")
            {
                tempPale.ChuFaTime = "";
                tempPale.ArriveTime = LastTime.addSecs(240).toString("hh:mm");
            }
            else if (Type == "始发")
            {
                tempPale.ChuFaTime = time.toString("hh:mm");
                tempPale.ArriveTime = "";
            }
            else if (Type == "接发车")
            {
                QTime Arrtime = LastTime.addSecs(240);
                tempPale.ChuFaTime =  Arrtime.toString("hh:mm");
                tempPale.ArriveTime = Arrtime.addSecs(240).toString("hh:mm");
            }
            else if (Type == "通过")
            {
                QTime Alltime =  LastTime.addSecs(240);
                tempPale.ChuFaTime =  Alltime.toString("hh:mm");
                tempPale.ArriveTime = Alltime.toString("hh:mm");
            }
        }
    }
    GetTimeInit(tempPale);
}
void jsj_adp::on_pushButton_clicked()
{
    if(TypeOpreate == Edit)
    {
        UpdateExistInfo();
        emit DBSERVICES.UpdateMultiPlan(Edit);
    }else if (TypeOpreate == APPEND)
    {

        Node->PlanName = ui->checi_Edit->text();
        if(Node->PlanName == "")
        {
            QMessageBox::information(this,"提示","请检查计划名称是否为空");
            return;
        }else
        {
            if(!Bool_ExistSameOne(Node->PlanName))
            {
                QMessageBox::information(this,"提示","已存在同名计划,请检查计划名称输入");
                return;
            }
        }
        Node->Direct = CurDirect;
        if(Node->Direct == -1)
        {
            QMessageBox::information(this,"提示","请检查多站状态选择是否正确");
            return;
        }

        Node->ChaoXianInfo = ui->cx_combo->currentText();
        Node->QY_type = ui->qytype_Combo->currentText();
        Node->QY_Index = ui->qytype_Combo->currentData().toUInt();
        Node->cxindex = ui->cx_combo->currentData().toUInt();
        Node->YunxingLeixing = ui->CCLX_Combo->currentText();
        Node->YunxinType_ = ui->CCLX_Combo->currentData().toUInt();
        Node->Checi = ui->checi_Edit_2->text();
        Insert_Append_PlanInfo();
        GetTimeAppend(TypeOpreate);
        if(Node->Checi == "")
        {
            QMessageBox::information(this,"提示","请检查车次号输入是否正确");
            return;
        }
        emit DBSERVICES.UpdateMultiPlan(APPEND);
    }
    this->close();

}

void jsj_adp::getPlanType(int index)
{
    QComboBox * box = static_cast<QComboBox*>(sender());
    int Size = layout->children().size();
    for(int i = 0; i < Size;++i)
    {
        QVBoxLayout * formLayout = qobject_cast<QVBoxLayout*>(layout->children()[i]);
        if (formLayout != nullptr)
        {
            for(int ii = 0 ; ii < formLayout->children().size();++ii)
            {
                QHBoxLayout * Hlayout = qobject_cast<QHBoxLayout*>(formLayout->children()[ii]);
                for(int z = 0; z < Hlayout->count();++z)
                {
                    QLayoutItem * item = Hlayout->itemAt(z);
                    QComboBox * tempBox = qobject_cast<QComboBox*>(item->widget());
                    if(tempBox == box)
                    {
                        QLayoutItem * item = Hlayout->itemAt(0);//获取站场名称
                        QLabel *label = qobject_cast<QLabel*>(item->widget());
                        if(label)
                        {
                            DBSERVICES.info.initInfo();
                            QString Name =  DBSERVICES.GetStationId(label->text());
                            emit DBSERVICES.GetGDInfo(Name);
                            SetSatisified_Info_multiPlan(label->text());
                        }
                        GetOtherInfo(Hlayout,box);
                        return;
                    }
                }
            }
        }
    }
}

void jsj_adp::getPlanType_JCFX(int index)
{
    QComboBox * box = static_cast<QComboBox*>(sender());
    int Size = layout->children().size();
    for(int i = 0; i < Size;++i)
    {
        QVBoxLayout * formLayout = qobject_cast<QVBoxLayout*>(layout->children()[i]);
        if (formLayout != nullptr)
        {
            for(int ii = 0 ; ii < formLayout->children().size();++ii)
            {
                QHBoxLayout * Hlayout = qobject_cast<QHBoxLayout*>(formLayout->children()[ii]);
                for(int z = 0; z < Hlayout->count();++z)
                {
                    QLayoutItem * item = Hlayout->itemAt(z);
                    QComboBox * tempBox = qobject_cast<QComboBox*>(item->widget());
                    if(tempBox == box)
                    {
                        QLayoutItem * item = Hlayout->itemAt(0);//获取站场名称
                        QLabel *label = qobject_cast<QLabel*>(item->widget());
                        if(label)
                        {
                            DBSERVICES.info.initInfo();
                            QString Name =  DBSERVICES.GetStationId(label->text());
                            emit DBSERVICES.GetGDInfo(Name);
                            SetSatisified_Info_multiPlan(label->text());
                        }
                        GetOtherInfo(Hlayout,box);
                        return;
                    }
                }
            }
        }
    }
}
