#include <QtDebug>
#include "station.h"
#include "QMessageBox"

CStation::CStation(QObject *parent) : QObject(parent)
{
    m_nStationID = 0;
    m_strStationName = "";
    m_nStationType = 0;
    m_strLS_Producer = "";
    isGT = true;
    isZCZX = true;
    offsetPt.setX(1);
    offsetPt.setY(1);
    m_nCTCSoftID = 0;
    m_nLS_ProcessID = 0xFFFF; //联锁进程唯一识别号
    m_nLS_SoftID = 0;  //联锁软件ID
    //isLSProcessFlag=false;
    m_nXLMNJSoftID = 0;
    isLSComState = false; //联锁通信状态标识
    LSComTime = 0; //与联锁持续无通信周期计数器
    isSYHZSB = false;
    isXYHZSB = false;
    jtgdFlag = false;
    nJTGDType = 0;
    nJTGDTime = 15;
    nJTGDTimeCount = 0;
    isGDNameIncludeWCQD = true;
    isSDJSFlag = false;

    for (int i = 0; i < 20; i++)
    {
        JLANArray[i].ANCode = 0xffff;
        JLANArray[i].ANType = 0x00;
        JLANArray[i].ANTime = 0;
    }
}
void CStation::timePro(long nElapsed)
{
    if (isSDJSFlag == false)
    {
        bool flag = true;
        if (getLSComState() == false)
        {
            flag = false;
        }
        else
        {
            for (auto ementDev : DevArray)
            {
                if (ementDev->getDevType() == Dev_QD)
                {
                    CQD *qd = (CQD *)ementDev;
                    if (qd->getQDColor() == Qt::white)
                    {
                        flag = false;
                        break;
                    }
                }
            }
        }
        isSDJSFlag = flag;
    }
    //监测通信状态，并在必要时导向安全测
    if (getLSComState() == true)
    {
        if (getLSComTime() > 0)
        {
            int count = getLSComTime();
            count--;
            setLSComTime(count);
        }
        else
        {
            setLSComState(false);
        }
    }
    else
    {
        setLSComTime(0);
    }
    if (getLSComState() == false)
    {
        setDevStateToSafe();
    }
    //轨道道岔设备更新接通光带状态-更新咽喉总锁闭状态
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC *)DevArray[i];
            gddc->setDisLight(getIsJtgd());
            if (gddc->getSX() == 1)
            {
                gddc->setIsYHZS(getIsSYHZSB());
            }
            else
            {
                gddc->setIsYHZS(getIsXYHZSB());
            }
        }
    }
    //接通光带倒计时
    if (getJTGD_Type() == 0)
    {
        if (getIsJtgd() == true)
        {
            if (getJTGD_TimeCount() > 0)
            {
                int count = getJTGD_TimeCount();
                count--;
                setJTGDTimeCount(count);
            }
            else
            {
                setIsJtgd(false);
            }
        }
        else
        {
            setJTGDTimeCount(0);
        }
    }
    JLANPro();
}
void CStation::setDevStateToSafe()
{
    isSDJSFlag = false;
    for (auto ement : DevArray)
    {
        ement->setDevStateToSafe();
    }
}
void CStation::resolutionDevStateOfLS(unsigned char *array)
{
    int dcNum = (array[17]) | (array[18] << 8);
    int xhdNum = (array[19]) | (array[20] << 8);
    int qdNum = (array[21]) | (array[22] << 8);
    int count = 23;
    int num = 0;
    for (auto ement : DevArray)
    {
        if (num > dcNum)
        {
            break;
        }
        if (ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC *)ement;
            int state = array[count];
            count++;
            //qDebug()<<"收到联锁车站设备状态信息，道岔设备："<<ement->getName()<<"     状态字节信息"<<state;
            gddc->setGDDC_StateOfLS(state);
            num++;
        }
    }
    num = 0;
    for (auto ement : DevArray)
    {
        if (num > xhdNum)
        {
            break;
        }
        if (ement->getDevType() == Dev_XH)
        {
            CXHD *xhd = (CXHD *)ement;
            int state1 = array[count];
            count++;
            int state2 = array[count];
            count++;
            int state3 = array[count];
            count++;
            int state4 = array[count];
            count++;
            //qDebug()<<"收到联锁车站设备状态信息，信号设备："<<ement->getName()<<"     状态字节信息"<<state1<<state2<<state3;
            xhd->setXHD_StateOfLS(state1, state2, state3, state4);
            num++;
        }
    }
    num = 0;
    for (auto ement : DevArray)
    {
        if (num > qdNum)
        {
            break;
        }
        if (ement->getDevType() == Dev_QD)
        {
            CQD *qd = (CQD *)ement;
            int state1 = array[count];
            count++;
            int state2 = array[count];
            count++;
            int state3 = array[count];
            count++;
            //qDebug()<<"收到联锁车站设备状态信息，区段设备："<<ement->getName()<<"     状态字节信息"<<state1<<state2;
            setQD_StateOfLS(qd, state1, state2, state3);
            num++;
        }
    }
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_TG)
        {
            CTG *tg = (CTG *)ement;
            int state = array[count];
            count++;
            tg->setTG_StateOfLS(state);
        }
    }

    count++;  //主副电源一个字节

    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_ZDBS)
        {
            CZDBS *zdbs = (CZDBS *)ement;
            int state1 = array[count];
            count++;
            int state2 = array[count];
            count++;
            int state3 = array[count];
            count++;
            int state4 = array[count];
            count++;
            int state5 = array[count];
            count++;
            zdbs->setZDBS_StateOfLS(state1, state2, state3, state4, state5);
        }
    }

    setYHZS_StateOfLS(array[count]);
    count++;  //咽喉总锁一个字节

}
void CStation::setQD_StateOfLS(CQD *qd, int state1, int state2, int state3)
{
    if (qd == NULL)
    {
        return;
    }
    if ((state1 & 0xE0) == 0x00)
    {
        setQD_Color(qd, Qt::red);
    }
    else if ((state1 & 0xE0) == 0x20)
    {
        setQD_Color(qd, Qt::white);
    }
    else if ((state1 & 0xE0) == 0x40)
    {
        setQD_Color(qd, SkyBlue);
    }
    else if ((state1 & 0xE0) == 0x60)
    {
        setQD_Color(qd, Qt::white);
    }
    else if ((state1 & 0xE0) == 0x80) //深蓝色预锁闭
    {
        setQD_Color(qd, SkyBlue);
    }
    qd->setQDDMH_StateOfLS(state1, state2);
    for (int i = 0; i < qd->getChildNum(); i++)
    {
        for (int j = 0; j < DevArray.size(); j++)
        {
            if (qd->getChild(i) == DevArray[j]->getCode())
            {
                if (DevArray[j]->getDevType() == Dev_GD)
                {
                    CGD *gd;
                    gd = (CGD*)(DevArray[j]);
                    if (gd->getIsHaveGDTW() == true)
                    {
                        gd->setGDTWTime(state3);   //设置股道停稳灯
                    }
                    break;
                }
            }
        }
    }
}
void CStation::setQD_Color(CQD *qd, QColor color)
{
    if (qd == NULL)
    {
        return;
    }
    qd->setQDColor(color);
    for (int i = 0; i < qd->getChildNum(); i++)
    {
        for (int j = 0; j < DevArray.size(); j++)
        {
            if (qd->getChild(i) == DevArray[j]->getCode())
            {
                if (DevArray[j]->getDevType() == Dev_DC)
                {
                    CGDDC *gddc;
                    gddc = (CGDDC*)(DevArray[j]);
                    gddc->setQDColor(color);
                    break;
                }
                else if (DevArray[j]->getDevType() == Dev_GD)
                {
                    CGD *gd;
                    gd = (CGD*)(DevArray[j]);
                    gd->setQDColor(color);
                    break;
                }
            }
        }
    }
}
void CStation::setYHZS_StateOfLS(int state)
{
    emit signal_YHZSBState(state, getStationID());
    if ((state & 0x80) == 0x80)
    {
        setIsSYHZSB(true);
    }
    else
    {
        setIsSYHZSB(false);
    }
    if ((state & 0x40) == 0x40)
    {
        setIsXYHZSB(true);
    }
    else
    {
        setIsXYHZSB(false);
    }
}
//打包设备状态到CTC
int CStation::packDev_StateForCTC(QByteArray &msg, int index, int modeType)
{
    int qdnum = 0;
    int dcnum = 0;
    int xhdnum = 0;
    int devNumCount = index;
    int lenth = index + 6;
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC *)ement;
            int state = 0x00;
            gddc->getGDDC_StateForCTC(&state);
            msg[lenth] = state;
            lenth++;
            dcnum++;
        }
    }
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_XH)
        {
            CXHD *xhd = (CXHD *)ement;
            int state = 0x00;
            xhd->getXHD_StateForCTC(&state);
            msg[lenth] = state;
            lenth++;
            xhdnum++;
        }
    }
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_QD)
        {
            CQD *qd = (CQD *)ement;
            int state1 = 0x00;
            int state2 = 0x00;
            qd->getQD_StateForCTC(&state1, &state2);
            msg[lenth] = state1;
            lenth++;
            msg[lenth] = state2;
            lenth++;
            qdnum++;
        }
    }
    //自动闭塞设备状态打包
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_ZDBS)
        {
            CZDBS *zdbs = (CZDBS *)ement;
            int state1 = 0x00;
            int state2 = 0x00;
            int state3 = 0x00;
            int state4 = 0x00;
            int state5 = 0x00;
            zdbs->getZDBS_StateForCTC(&state1, &state2, &state3, &state4, &state5);
            msg[lenth] = state1;
            lenth++;
            msg[lenth] = state2;
            lenth++;
            msg[lenth] = state3;
            lenth++;
            msg[lenth] = state4;
            lenth++;
            msg[lenth] = state5;
            lenth++;
        }
    }
    //半自动闭塞设备状态打包

    //计轴设备状态打包

    //机务段设备状态打包

    //道岔尖轨心轨故障状态打包
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC *)ement;
            int state = 0x00;
            state = gddc->getDCFaultStateForCTC();
            msg[lenth] = state;
            lenth++;
        }

    }

    //信号机倒计时状态打包
    msg[lenth] = 0xDA;
    lenth++;
    msg[lenth] = 0xDA;
    lenth++;
    for (auto ement : DevArray)
    {
        if (ement->getDevType() == Dev_XH)
        {
            CXHD *xhd = (CXHD *)ement;
            int state = 0x00;
            state = xhd->getTimeCount();
            msg[lenth] = state;
            lenth++;
        }
    }

    //上电解锁状态打包
    msg[lenth] = isSDJSFlag;
    lenth++;

    //X人解30s读秒
    msg[lenth] = 0x00;
    lenth++;
    //X人解180s读秒
    msg[lenth] = 0x00;
    lenth++;
    //S人解30s读秒
    msg[lenth] = 0x00;
    lenth++;
    //S人解180s读秒
    msg[lenth] = 0x00;
    lenth++;
    //坡道解锁读秒
    msg[lenth] = 0x00;
    lenth++;
    //允许转回状态灯
    msg[lenth] = 0x00;
    lenth++;
    //咽喉总锁状态
    int state_yhzsb = 0x00;
    if (getIsSYHZSB() == false)
    {
        state_yhzsb |= 0x01;
    }
    if (getIsXYHZSB() == false)
    {
        state_yhzsb |= 0x02;
    }
    msg[lenth] = state_yhzsb;
    lenth++;

    //CTC控制模式状态
    msg[lenth] = modeType;
    //qDebug()<<modeType;
    lenth++;

    msg[devNumCount] = dcnum;
    msg[devNumCount + 1] = dcnum >> 8; //道岔个数
    msg[devNumCount + 2] = xhdnum;
    msg[devNumCount + 3] = xhdnum >> 8; //信号机个数
    msg[devNumCount + 4] = qdnum;
    msg[devNumCount + 5] = qdnum >> 8; //区段个数
    return lenth;
}
void CStation::OperationalOfCTC(unsigned char *array)
{
    int anType = array[11];
    //qDebug()<<"接收到CTC操作指令信息，正在解析当前指令类型，操作按钮类型："<<anType;
    if ((anType == 0x01) || (anType == 0x02) || (anType == 0x03)) //列车进路、调车进路
    {
        anylzJLAN(array);
        return;
    }
    int code = array[12] | (array[13] << 8);
    if (anType == 0x04) //总取消
    {
        //qDebug()<<"收到CTC总取消命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_ZQX, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                emit signal_sendCmdOfCTC(Fun_ZQX, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, DCAN);
                //qDebug()<<"收到CTC总取消命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x05) //信号重开
    {
        //qDebug()<<"收到CTC信号重开命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_XHANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                emit signal_sendCmdOfCTC(Fun_XHANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, DCAN);
                //qDebug()<<"收到CTC信号重开命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x06) //道岔总定
    {
        //qDebug()<<"收到CTC道岔总定命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                emit signal_sendCmdOfCTC(Fun_DCZD, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                //qDebug()<<"收到CTC道岔总定命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x07) //道岔总反
    {
        //qDebug()<<"收到CTC道岔总反命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                emit signal_sendCmdOfCTC(Fun_DCZF, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                //qDebug()<<"收到CTC道岔总反命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x08) //道岔单锁
    {
        //qDebug()<<"收到CTC道岔单锁命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                emit signal_sendCmdOfCTC(Fun_DCDS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                //qDebug()<<"收到CTC道岔单锁命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x09) //道岔单解
    {
        //qDebug()<<"收到CTC道岔单解命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                emit signal_sendCmdOfCTC(Fun_DCDJ, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                //qDebug()<<"收到CTC道岔单解命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x10) //封闭操作
    {
        //qDebug()<<"收到CTC封闭命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getCode() == code)
            {
                if (ement->getDevType() == Dev_XH)
                {
                    emit signal_sendCmdOfCTC(Fun_ANFS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                    emit signal_sendCmdOfCTC(Fun_ANFS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, DCAN);
                    emit signal_sendCmdOfCTC(Fun_ANFS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, YDAN);
                    for (auto ementTG : DevArray)
                    {
                        if (ementTG->getDevType() == Dev_TG)
                        {
                            CTG *tg = (CTG*)ement;
                            if (tg->getGLXH1() == code)
                            {
                                emit signal_sendCmdOfCTC(Fun_ANFS, getStationID(), getLSProcessID(), getLSSoftID(), tg->getCode(), 0xFFFF, TGAN);
                                break;
                            }
                        }
                    }
                    //qDebug()<<"收到CTC信号封闭命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
                else if (ement->getDevType() == Dev_DC)
                {
                    emit signal_sendCmdOfCTC(Fun_DCFS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                    //qDebug()<<"收到CTC道岔封闭命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x11) //解封操作
    {
        //qDebug()<<"收到CTC解封命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getCode() == code)
            {
                if (ement->getDevType() == Dev_XH)
                {
                    emit signal_sendCmdOfCTC(Fun_ANJF, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                    emit signal_sendCmdOfCTC(Fun_ANJF, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, DCAN);
                    emit signal_sendCmdOfCTC(Fun_ANJF, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, YDAN);
                    for (auto ementTG : DevArray)
                    {
                        if (ementTG->getDevType() == Dev_TG)
                        {
                            CTG *tg = (CTG*)ement;
                            if (tg->getGLXH1() == code)
                            {
                                emit signal_sendCmdOfCTC(Fun_ANJF, getStationID(), getLSProcessID(), getLSSoftID(), tg->getCode(), 0xFFFF, TGAN);
                                break;
                            }
                        }
                    }
                    //qDebug()<<"收到CTC信号解封命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
                else if (ement->getDevType() == Dev_DC)
                {
                    emit signal_sendCmdOfCTC(Fun_DCJF, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, GDDCAN);
                    //qDebug()<<"收到CTC道岔解封命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x12) //引导按钮
    {
        //qDebug()<<"收到CTC引导按钮命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_XHANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, YDAN);
                //qDebug()<<"收到CTC引导按钮命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x13) //引导总锁
    {
        //qDebug()<<"收到CTC引导总锁按钮命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                if (ement->getSX() == 0)
                {
                    emit signal_sendCmdOfCTC(Fun_XYDZS, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0x00);
                    //qDebug()<<"收到CTC  X引导总锁按钮命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
                else
                {
                    emit signal_sendCmdOfCTC(Fun_SYDZS, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0x01);
                    //qDebug()<<"收到CTC  S引导总锁按钮命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x14) //总人解
    {
        //qDebug()<<"收到CTC总人解命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_ZRJ, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                emit signal_sendCmdOfCTC(Fun_ZRJ, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, DCAN);
                //qDebug()<<"收到CTC总人解命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x15) //坡道解锁
    {
        //qDebug()<<"收到CTC坡道解锁命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_PDJS, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                //qDebug()<<"收到CTC坡道解锁命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x18) //总辅助
    {
        //qDebug()<<"收到CTC总辅助命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getDevType() == Dev_ZDBS)
            {
                CZDBS *zdbs = (CZDBS*)ement;
                if (zdbs->getGLXH() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_ZFZ, getStationID(), getLSProcessID(), getLSSoftID(), zdbs->getCode(), 0xFFFF, ZDBSAN);
                    //qDebug()<<"收到CTC总辅助命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x19) //发车辅助
    {
        //qDebug()<<"收到CTC发车辅助命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getDevType() == Dev_ZDBS)
            {
                CZDBS *zdbs = (CZDBS*)ement;
                if (zdbs->getGLXH() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_FCFZ, getStationID(), getLSProcessID(), getLSSoftID(), zdbs->getCode(), 0xFFFF, ZDBSAN);
                    //qDebug()<<"收到CTC发车辅助命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x20) //接车辅助
    {
        //qDebug()<<"收到CTC接车辅助命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getDevType() == Dev_ZDBS)
            {
                CZDBS *zdbs = (CZDBS*)ement;
                if (zdbs->getGLXH() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_JCFZ, getStationID(), getLSProcessID(), getLSSoftID(), zdbs->getCode(), 0xFFFF, ZDBSAN);
                    //qDebug()<<"收到CTC接车辅助命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x21) //允许改方
    {
        //qDebug()<<"收到CTC允许改方命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getDevType() == Dev_ZDBS)
            {
                CZDBS *zdbs = (CZDBS*)ement;
                if (zdbs->getGLXH() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_GFAN, getStationID(), getLSProcessID(), getLSSoftID(), zdbs->getCode(), 0xFFFF, ZDBSAN);
                    //qDebug()<<"收到CTC允许改方命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x23) //上电解锁
    {
        emit signal_sendCmdOfCTC(Fun_SDJS, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0xFF);
        //qDebug()<<"收到CTC上电解锁命令"<<"执行命令成功";
    }
    else if (anType == 0x24) //引导总锁取消
    {
        //qDebug()<<"收到CTC引导总锁取消命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                if (ement->getSX() == 0)
                {
                    emit signal_sendCmdOfCTC(Fun_XYDZS, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0x00);
                    //qDebug()<<"收到CTC X引导总锁取消命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
                else
                {
                    emit signal_sendCmdOfCTC(Fun_SYDZS, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0x01);
                    //qDebug()<<"收到CTC S引导总锁取消命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x25) //区故解
    {
        //qDebug()<<"收到CTC区故解命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if (ement->getDevType() == Dev_GD)
            {
                if (ement->getCode() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_QGJ, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, QDAN);
                    //qDebug()<<"收到CTC引导区故解命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
            else if (ement->getDevType() == Dev_DC)
            {
                CGDDC *gddc = (CGDDC *)ement;
                if (gddc->getQDCode() == code)
                {
                    emit signal_sendCmdOfCTC(Fun_QGJ, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, QDAN);
                    //qDebug()<<"收到CTC区故解命令"<<ement->getName()<<"执行命令成功";
                    break;
                }
            }
        }
    }
    else if (anType == 0x36) //半自动闭塞
    {

    }
    else if (anType == 0x37) //半自动事故
    {

    }
    else if (anType == 0x38) //半自动复原
    {

    }
    else if (anType == 0x39) //点灯
    {
        //qDebug()<<"收到CTC点灯命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_DD, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                //qDebug()<<"收到CTC点灯命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x40) //灭灯
    {
        //qDebug()<<"收到CTC灭灯命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_XH))
            {
                emit signal_sendCmdOfCTC(Fun_MD, getStationID(), getLSProcessID(), getLSSoftID(), code, 0xFFFF, LCAN);
                //qDebug()<<"收到CTC灭灯命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x41) //全场封锁
    {

    }
    else if (anType == 0x47) //尖轨故障
    {
        //qDebug()<<"收到CTC尖轨故障命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                CGDDC *gddc = (CGDDC*)ement;
                gddc->onLBtnDownJG();
                //qDebug()<<"收到CTC尖轨故障命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x48) //心轨故障
    {
        //qDebug()<<"收到CTC心轨故障命令"<<"收到命令";
        for (auto ement : DevArray)
        {
            if ((ement->getCode() == code) && (ement->getDevType() == Dev_DC))
            {
                CGDDC *gddc = (CGDDC*)ement;
                gddc->onLBtnDownXG();
                //qDebug()<<"收到CTC心轨故障命令"<<ement->getName()<<"执行命令成功";
                break;
            }
        }
    }
    else if (anType == 0x50) //计轴复零
    {

    }
    else if (anType == 0x51) //计轴使用
    {

    }
    else if (anType == 0x52) //计轴停止
    {

    }
    else if (anType == 0x60) //清除
    {
        emit signal_sendCmdOfCTC(Fun_CLEAR, getStationID(), getLSProcessID(), getLSSoftID(), 0xFFFF, 0xFFFF, 0xFF);
        //qDebug()<<"收到CTC 清除 命令"<<"执行命令成功";
    }
    else if (anType == 0x63) //故障通知
    {
        emit signal_ANCmdOfCTC(Fun_GZTZ, 0xFFFF, 0xFFFF);
        //qDebug()<<"收到CTC 故障通知 命令"<<"执行命令成功";
    }
}
void CStation::add_forJLAN(unsigned int code, unsigned int type, unsigned int time)
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < 20; i++)
    {
        if (JLANArray[i].ANCode == 0xffff)
        {
            count = i;
            break;
        }
    }
    if (count >= 20)
    {
        return;
    }
    JLANArray[count].ANCode = code;
    JLANArray[count].ANType = type;

    if (count == 0)
    {
        JLANArray[count].ANTime = time;
    }
    else
    {
        JLANArray[count].ANTime = time + 2;
    }
}
void CStation::anylzJLAN(unsigned char *REC_data)
{
    unsigned int xhNode;
    unsigned int anType;
    unsigned int nTime;
    unsigned int Framelen = 0;
    unsigned int jllx;  //记录进路类型用于区分各个按钮的等待时间防止按钮多了，最后一个按钮还未按下第一个按钮已经失效
    unsigned int sdan_SX;  //记录始端按钮上下行咽喉用
    int pdjz_Count = 0; //用于记录坡道进站接车及延续进路列车按钮个数，主要加大延续部分按下的时机
    bool pdjz_NeedBigFlag = false; //标识符，配合pdjz_Count使用
    Framelen = (int)(REC_data[4] | (REC_data[5] << 8));
    jllx = REC_data[11];
    //qDebug()<<"收到CTC进路操作指令"<<jllx<<"收到命令";
    for (unsigned int i = 0; i < 20; i++)
    {
        if ((12 + i * 2 + 1) >= Framelen - 4)
        {
            break;   //自主防护帧尾，防止帧尾解析为按钮设备编号
        }
        if ((REC_data[12 + i * 2] == 0x00) && (REC_data[12 + i * 2 + 1] == 0x00))
        {
            break;  //检测到传输为全零，则识别为填充位，不做处理
        }

        if ((int)(REC_data[12 + i * 2 + 1] >> 6) == 0)
        {
            anType = LCAN;
        }
        else if ((int)(REC_data[12 + i * 2 + 1] >> 6) == 1)
        {
            anType = DCAN;
        }
        else if ((int)(REC_data[12 + i * 2 + 1] >> 6) == 2)
        {
            anType = TGAN;
        }
        xhNode = (int)(REC_data[12 + i * 2] | ((REC_data[12 + i * 2 + 1] & 0x3F) << 8));
        //qDebug()<<"00---收到CTC进路操作指令"<<jllx<<"正在执行"<<anType<<xhNode;
        CXHD *xhd = nullptr;
        for (auto ement : DevArray)
        {
            if ((ement->getDevType() == Dev_XH) && (ement->getCode() == xhNode))
            {
                xhd = (CXHD*)ement;
                break;
            }
        }
        if (xhd == nullptr)
        {
            continue;
        }
        //qDebug()<<"11---收到CTC进路操作指令"<<jllx<<"正在执行"<<xhd->getName()<<anType<<xhNode;
        if (i == 0)
        {
            nTime = 1;
            sdan_SX = xhd->getSX();
            if (xhd->getXH_PDJZ() == true)
            {
                pdjz_Count++;
            }
        }
        else
        {
            if (jllx == 0x03) //通过进路
            {
                nTime = 1;
            }
            else
            {
                if (pdjz_Count == 0)
                {
                    if (sdan_SX == xhd->getSX())
                    {
                        nTime = 1;
                    }
                    else
                    {
                        nTime = 3;
                        sdan_SX = xhd->getSX(); //后面的几个按钮为另一个咽喉，及时刷新始端信号机的咽喉记录变量
                    }
                }
                else
                {
                    if (pdjz_NeedBigFlag == true)
                    {
                        nTime = 9; //5---此处因有组合进路导致延续终端点下太早，如只有基本进路则5即可
                        pdjz_NeedBigFlag = false;
                    }
                    else
                    {
                        nTime = 1;
                    }
                    if (anType == LCAN)
                    {
                        pdjz_NeedBigFlag = true;
                    }
                }
            }
        }
        //qDebug()<<"22---收到CTC进路操作指令"<<jllx<<"正在执行"<<xhd->getName()<<anType<<xhNode;
        add_forJLAN(xhNode, anType, nTime);
    }
}
void CStation::JLANPro()
{
    bool flag = false;
    if (JLANArray[0].ANCode != 0xffff)
    {
        if (JLANArray[0].ANTime > 0)
        {
            JLANArray[0].ANTime--;
        }
        else
        {
            if (JLANArray[0].ANType == LCAN)
            {
                emit signal_sendCmdOfCTC(Fun_XHANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), JLANArray[0].ANCode, 0xFFFF, LCAN);
                //qDebug()<<"进路按钮处理"<<"列车按钮"<<JLANArray[0].ANCode<<LCAN<<"下一个按钮"<<JLANArray[1].ANCode<<JLANArray[1].ANType;
            }
            else if (JLANArray[0].ANType == DCAN)
            {
                emit signal_sendCmdOfCTC(Fun_XHANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), JLANArray[0].ANCode, 0xFFFF, DCAN);
                //qDebug()<<"进路按钮处理"<<"调车按钮"<<JLANArray[0].ANCode<<DCAN<<"下一个按钮"<<JLANArray[1].ANCode<<JLANArray[1].ANType;
            }
            else if (JLANArray[0].ANType == TGAN)
            {
                emit signal_sendCmdOfCTC(Fun_TGANDOWN, getStationID(), getLSProcessID(), getLSSoftID(), JLANArray[0].ANCode, 0xFFFF, TGAN);
                //qDebug()<<"进路按钮处理"<<"通过按钮"<<JLANArray[0].ANCode<<TGAN<<"下一个按钮"<<JLANArray[1].ANCode<<JLANArray[1].ANType;
            }
            if (JLANArray[19].ANCode != 0xffff)
            {
                flag = true;
            }
            for (unsigned int i = 0; i < 19; i++)
            {
                JLANArray[i].ANCode = JLANArray[i + 1].ANCode;
                JLANArray[i].ANType = JLANArray[i + 1].ANType;
                JLANArray[i].ANTime = JLANArray[i + 1].ANTime;
            }
            if (flag == true)
            {
                JLANArray[19].ANCode = 0xffff;
                JLANArray[19].ANType = 0x00;
                JLANArray[19].ANTime = 0;
            }
        }
    }
}
//功能函数实现
void CStation::setShow_Menu(int type)
{
    if (type == 0x11)  //设置站名显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_TEXT)
            {
                CText *text;
                text = (CText *)(DevArray[i]);
                if (text->getTextType() == STATIONNAMETEXT)
                {
                    text->setDisplayName(!(text->getDisplayName()));
                }
            }
        }
    }
    else if (type == 0x22) //设置信号机名称显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_XH)
            {
                DevArray[i]->setDisplayName(!(DevArray[i]->getDisplayName()));
            }
            if (DevArray[i]->getDevType() == Dev_TG)
            {
                DevArray[i]->setDisplayName(!(DevArray[i]->getDisplayName()));
            }
        }
    }
    else if (type == 0x33) //设置道岔名称显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_DC)
            {
                DevArray[i]->setDisplayName(!(DevArray[i]->getDisplayName()));
            }
        }
    }
    else if (type == 0x44) //设置无区段名称显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_GD)
            {
                CGD *gd = (CGD*)DevArray[i];
                if ((gd->getGDType() == GD_QD) && (getIsGDNameIncludeWCQD() == false))
                {
                    continue;
                }
                DevArray[i]->setDisplayName(!(DevArray[i]->getDisplayName()));
            }
        }
    }
    else if (type == 0x55) //设置文本显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_TEXT)
            {
                CText *text;
                text = (CText *)(DevArray[i]);
                if (text->getTextType() == TEXT)
                {
                    text->setDisplayName(!(text->getDisplayName()));
                }
            }
        }
    }
    else if (type == 0x66) //设置道岔区段显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_TEXT)
            {
                CText *text;
                text = (CText *)(DevArray[i]);
                if (text->getTextType() == DCQDNAMETEXT)
                {
                    text->setDisplayName(!(text->getDisplayName()));
                }
            }
        }
    }
    else if (type == 0x77) //设置道岔位置显示隐藏
    {
        for (int i = 0; i < DevArray.size(); i++)
        {
            if (DevArray[i]->getDevType() == Dev_DC)
            {
                CGDDC *gddc;
                gddc = (CGDDC *)(DevArray[i]);
                gddc->setDisDCWZ(!(gddc->getDisDCWZ()));
            }
        }
    }
    else if (type == 0x88) //设置道岔接通光带显示隐藏
    {
        if (getJTGD_Type() == 0)
        {
            setIsJtgd(true);
            setJTGDTimeCount(getJTGD_Time() * 2);
        }
        else
        {
            setIsJtgd(!(getIsJtgd()));
        }
    }
}
void CStation::set_ZCReset()
{
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() == Dev_XH)
        {
            CXHD *xhd;
        }
        else if (DevArray[i]->getDevType() == Dev_DC)
        {
            CGDDC *gddc;
            gddc = (CGDDC *)(DevArray[i]);
            //gddc->setIsQDGZ(false);
            //gddc->setIsQDZY(false);
            gddc->setIsFLBL_CQ(false);
            gddc->setIsFLBL_DW(false);
            gddc->setIsFLBL_FW(false);
            if (gddc->getDCWZ() == DCSK)
            {
                gddc->setDCWZ(DCDW);
            }
        }
        else if (DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *gd;
            gd = (CGD *)(DevArray[i]);
            //gd->setIsQDGZ(false);
            //gd->setIsQDZY(false);
            gd->setGDFLBL(false);
        }
    }
}
void CStation::setQDZYFun(CQD *qd, bool flag)
{
    if (qd == NULL)
    {
        return;
    }
    //qd->setIsQDZY(flag);
    for (int i = 0; i < qd->getChildNum(); i++)
    {
        for (int j = 0; j < DevArray.size(); j++)
        {
            if (qd->getChild(i) == DevArray[j]->getCode())
            {
                if (DevArray[j]->getDevType() == Dev_DC)
                {
                    CGDDC *gddc;
                    gddc = (CGDDC*)(DevArray[j]);
                    //gddc->setIsQDZY(flag);
                    break;
                }
                else if (DevArray[j]->getDevType() == Dev_GD)
                {
                    CGD *gd;
                    gd = (CGD*)(DevArray[j]);
                    //gd->setIsQDZY(flag);
                    break;
                }
            }
        }
    }
}
void CStation::setQDGZFun(CQD *qd, bool flag)
{
    if (qd == NULL)
    {
        return;
    }
    //qd->setIsQDGZ(flag);
    for (int i = 0; i < qd->getChildNum(); i++)
    {
        for (int j = 0; j < DevArray.size(); j++)
        {
            if (qd->getChild(i) == DevArray[j]->getCode())
            {
                if (DevArray[j]->getDevType() == Dev_DC)
                {
                    CGDDC *gddc;
                    gddc = (CGDDC*)(DevArray[j]);
                    //gddc->setIsQDGZ(flag);
                    break;
                }
                else if (DevArray[j]->getDevType() == Dev_GD)
                {
                    CGD *gd;
                    gd = (CGD*)(DevArray[j]);
                    //gd->setIsQDGZ(flag);
                    break;
                }
            }
        }
    }
}
CQD* CStation::getQDFromGDDCNode(int code)
{
    CQD *qd;
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() != Dev_QD)
        {
            continue;
        }
        qd = (CQD *)(DevArray[i]);
        if (qd->getChildNum() <= 0)
        {
            continue;
        }
        for (int j = 0; j < qd->getChildNum(); j++)
        {
            if (qd->getChild(j) == code)
            {
                return qd;
            }
        }
    }
    return NULL;
}
int CStation::getDCNodeFromDCName(QString name)
{
    for (auto ement : DevArray)
    {
        if ((ement->getDevType() == Dev_DC) && (ement->getName() == name))
        {
            return ement->getCode();
        }
    }
    return 0xffff;
}
void CStation::stationInit(int dymType)
{
}

void CStation::drawStation(QPainter *painter, long nElapsed, double nDiploid, int JmType)
{
    setGD();
    for (int i = 0; i < DevArray.size(); i++)
    {
        if ((DevArray[i]->getDevType() == Dev_LZZDBS)  && LZZDBSFlag
                || (DevArray[i]->getDevType() == Dev_LZBZD)  && LZBZDFlag
                || (DevArray[i]->getDevType() != Dev_LZZDBS && DevArray[i]->getDevType() != Dev_LZBZD))
            DevArray[i]->Draw(painter, nElapsed, nDiploid, offsetPt, JmType, isZCZX);
    }

    //绘制鼠标移动提示信息
    for (int i = 0; i < DevArray.size(); i++)
    {
        DevArray[i]->Draw_ToolTip(painter, nDiploid);
    }
}
void CStation::volloverPro()
{
    QPoint pt_Min = QPoint(0, 0);
    QPoint pt_Max = QPoint(0, 0);
    QPoint pt_Base = QPoint(0, 0);
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() == Dev_GD)
        {
            CGD *gd = (CGD *)(DevArray[i]);
            if ((gd->getp12().x() >= 0) && (gd->getp12().y() >= 0) && (gd->getp34().x() >= 0) && (gd->getp34().y() >= 0))
            {
                if ((gd->getp12().x() < pt_Min.x()) || (pt_Min.x() == 0))
                {
                    pt_Min.setX(gd->getp12().x());
                }
                if ((gd->getp12().y() < pt_Min.y()) || (pt_Min.y() == 0))
                {
                    pt_Min.setY(gd->getp12().y());
                }
                if ((gd->getp34().x() > pt_Max.x()) || (pt_Max.x() == 0))
                {
                    pt_Max.setX(gd->getp34().x());
                }
                if ((gd->getp34().y() > pt_Max.y()) || (pt_Max.y() == 0))
                {
                    pt_Max.setY(gd->getp34().y());
                }
            }
        }
    }
    pt_Base = QPoint(((pt_Min.x() + pt_Max.x()) / 2), ((pt_Min.y() + pt_Max.y()) / 2));
    for (int i = 0; i < DevArray.size(); i++)
    {
        //翻转算法
        DevArray[i]->setVollover(pt_Base);
    }
}
//成员变量封装函数实现
void CStation::setMainGD()
{
    int i, j, k, l = -1;
    CGDDC *p, *q, *r, *s = NULL;
    for (unsigned int m = 0; m < DevArray.size(); m++)
    {
        if (DevArray[m]->getDevType() != Dev_QD)
        {
            continue;
        }
        CQD *qd = (CQD *)(DevArray[m]);
        if (qd->getChildNum() > 1)
        {
            i = qd->getChild(0);
            j = qd->getChild(1);
            for (int x = 0; x < DevArray.size(); x++)
            {
                if (i == DevArray[x]->getCode())
                {
                    p = (CGDDC*)(DevArray[x]);
                }
                else if (j == DevArray[x]->getCode())
                {
                    q = (CGDDC*)(DevArray[x]);
                }
            }
            if (qd->getChildNum() > 2)
            {
                k = qd->getChild(2);
                for (int x = 0; x < DevArray.size(); x++)
                {
                    if (k == DevArray[x]->getCode())
                    {
                        r = (CGDDC*)(DevArray[x]);
                    }
                    if (p->getDWdev() == k && r->getDWdev() == i)                //道岔1，3定位相连
                    {
                        if ((p->getCQdev() == j && q->getCQdev() == i) ||        //道岔1，2岔前相连
                                (r->getCQdev() == j && q->getCQdev() == k))          //道岔2，3岔前相连
                        {
                            s = q;
                            l = j;                             //取出道岔2
                        }
                        q = r;
                        j = k;
                    }
                    else if (q->getDWdev() == k && r->getDWdev() == j)           //道岔2，3定位相连
                    {
                        if ((p->getCQdev() == j && q->getCQdev() == i) ||        //道岔1，2岔前相连
                                (r->getCQdev() == i && p->getCQdev() == k))          //道岔1，3岔前相连
                        {
                            s = p;
                            l = i;                             //取出道岔1
                        }
                        p = r;
                        i = k;
                    }
                    else if (p->getDWdev() == j && q->getDWdev() == i)           //道岔1，2定位相连
                    {
                        if ((q->getCQdev() == k && r->getCQdev() == j) ||        //道岔2，3岔前相连
                                (r->getCQdev() == i && p->getCQdev() == k))          //道岔1，3岔前相连
                        {
                            s = r;
                            l = k;                             //取出道岔1
                        }
                    }
                }
            }
            else if (p->getCQdev() == j && q->getCQdev() == i)               //只有两个道岔，道岔1，2岔前相连
            {
                p->setIsMainGD(1);
                q->setIsMainGD(1);
            }
            if (p->getDWdev() == j && q->getDWdev() == i)                    //道岔1，2定位相连
            {
                if ((p->getDCWZ() == DCFW) && (q->getDCWZ() == DCDW))  //道岔1反位，2定位
                {
                    if (s)
                    {
                        if (q->getCQdev() == l && s->getCQdev() == j)        //道岔2，3岔前相连
                            s->setIsMainGD(0);
                        else
                            s->setIsMainGD(1);
                    }
                    p->setIsMainGD(1);
                    q->setIsMainGD(0);
                }
                else if ((q->getDCWZ() == DCFW) && (p->getDCWZ() == DCDW))  //道岔1定位，2反位
                {
                    if (s)
                    {
                        if (p->getCQdev() == l && s->getCQdev() == i)             //道岔2，3岔前相连
                            s->setIsMainGD(0);
                        else
                            s->setIsMainGD(1);
                    }
                    p->setIsMainGD(0);
                    q->setIsMainGD(1);
                }
                else if ((q->getDCWZ() == DCDW) && (p->getDCWZ() == DCDW))     //道岔1定位，2定位
                {
                    if (s)
                        s->setIsMainGD(1);
                    p->setIsMainGD(1);
                    q->setIsMainGD(1);
                }
                else if ((q->getDCWZ() == DCFW) && (p->getDCWZ() == DCFW))     //道岔1反位，2反位
                {
                    p->setIsMainGD(1);
                    q->setIsMainGD(1);
                }
                else if (s)
                    s->setIsMainGD(1);
            }
        }
    }
}
void CStation::clearDrawGD()
{
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() == Dev_DC)
        {
            CGDDC *gddc = (CGDDC*)(DevArray[i]);
            gddc->setDrawGD(false);
        }
    }
}
void CStation::setGD()
{
    setMainGD();
    clearDrawGD();
    CGDDC *gddc;
    for (int i = 0; i < DevArray.size(); i++)
    {
        if (DevArray[i]->getDevType() != Dev_QD)
        {
            continue;
        }
        CQD *qd = (CQD *)(DevArray[i]);
        if (qd->getChildNum() > 1)
        {
            for (int k = 0; k < qd->getChildNum(); k++)
            {
                for (int j = 0; j < DevArray.size(); j++)
                {
                    if (qd->getChild(k) == DevArray[j]->getCode())
                    {
                        gddc = (CGDDC*)(DevArray[j]);
                        if (gddc->getIsMainGD() == true)
                        {
                            int nNext = gddc->setDrawGD(true);
                            int m = 1;
                            while (nNext != -1) //直到后继为空
                            {
                                m++;
                                for (int l = 0; l < DevArray.size(); l++)
                                {
                                    if (nNext == DevArray[l]->getCode())
                                    {
                                        if (DevArray[l]->getDevType() == Dev_DC)
                                        {
                                            gddc = (CGDDC *)DevArray[l];
                                            nNext = gddc->setDrawGD(true);
                                            break;
                                        }
                                    }
                                }
                                if (m >= 3)
                                    break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}
void CStation::setStationName(QString name)
{
    m_strStationName = name;
}
QString CStation::getStationName()
{
    return m_strStationName;
}
void CStation::setStationID(unsigned int id)
{
    m_nStationID = id;
}
unsigned int CStation::getStationID()
{
    return m_nStationID;
}
void CStation::setStationType(unsigned int type)
{
    m_nStationType = type;
}
unsigned int CStation::getStationType()
{
    return m_nStationType;
}
void CStation::setLSProducer(QString producer)
{
    m_strLS_Producer = producer;
}
QString CStation::getLSProducer()
{
    return m_strLS_Producer;
}
void CStation::setIsGT(bool flag)
{
    isGT = flag;
}
bool CStation::getIsGT()
{
    return isGT;
}
void CStation::setIsZCZX(bool flag)
{
    isZCZX = flag;
}
bool CStation::getIsZCZX()
{
    return isZCZX;
}
void CStation::setOffSet(QPoint offset)
{
    offsetPt.setX(offset.x());
    offsetPt.setY(offset.y());
}
QPoint CStation::getOffSet()
{
    return offsetPt;
}
void CStation::setCTCSoftID(int x)
{
    m_nCTCSoftID = x;
}
int CStation::getCTCSoftID()
{
    return m_nCTCSoftID;
}
void CStation::setLSProcessID(int x)
{
    m_nLS_ProcessID = x;
}
int CStation::getLSProcessID()
{
    return m_nLS_ProcessID;
}
void CStation::setLSSoftID(int x)
{
    m_nLS_SoftID = x;
}
int CStation::getLSSoftID()
{
    return m_nLS_SoftID;
}
void CStation::setXLMNJSoftID(int x)
{
    m_nXLMNJSoftID = x;
}
int CStation::getXLMNJSoftID()
{
    return m_nXLMNJSoftID;
}
void CStation::setLSComState(bool flag)
{
    isLSComState = flag;
}
bool CStation::getLSComState()
{
    return isLSComState;
}
void CStation::setLSComTime(int x)
{
    LSComTime = x;
}
int CStation::getLSComTime()
{
    return LSComTime;
}
void CStation::setIsSYHZSB(bool flag)
{
    isSYHZSB = flag;
}
bool CStation::getIsSYHZSB()
{
    return isSYHZSB;
}
void CStation::setIsXYHZSB(bool flag)
{
    isXYHZSB = flag;
}
bool CStation::getIsXYHZSB()
{
    return isXYHZSB;
}
void CStation::setIsJtgd(bool flag)
{
    jtgdFlag = flag;
}
bool CStation::getIsJtgd()
{
    return jtgdFlag;
}
void CStation::setJTGD_Type(int x)
{
    nJTGDType = x;
}
int CStation::getJTGD_Type()
{
    return nJTGDType;
}
void CStation::setJTGD_Time(int x)
{
    nJTGDTime = x;
}
int CStation::getJTGD_Time()
{
    return nJTGDTime;
}
void CStation::setJTGDTimeCount(int x)
{
    nJTGDTimeCount = x;
}
int CStation::getJTGD_TimeCount()
{
    return nJTGDTimeCount;
}
void CStation::setIsGDNameIncludeWCQD(bool flag)
{
    isGDNameIncludeWCQD = flag;
}
bool CStation::getIsGDNameIncludeWCQD()
{
    return isGDNameIncludeWCQD;
}
