#include <QtDebug>
#include "TStation.h"
#include<QMutex>
TStation::TStation(QObject *parent) : QObject(parent)
{
    m_nStationID = 0;
    m_strStationName = "";
    m_nStationType = 0;
    m_strLS_Producer = "";
    isGT = true;
    isZCZX = true;
    m_nRBCCode = 0;
    offsetPt.setX(0);
    offsetPt.setY(0);
    m_Selected.Init();
}

//功能函数实现
void TStation::stationInit()
{
    int x = 0;
    int y = 0;
    QPoint pt;
    double a = 0.0;
    double b = 0.0;
    //依据相似三角形等数理逻辑，根据应答器偏移量计算应答器在屏幕坐标系中显示位置
    for (int i = 0; i < YDQArray.size(); i++)
    {
        for (int j = 0; j < GDArray.size(); j++)
        {
            if (YDQArray[i].getGLQDNode() == GDArray[j]->getCode())
            {
                a = GDArray[i]->p34.x() - GDArray[i]->p12.x();
                x = GDArray[i]->p12.x() + (int)(a * ((double)YDQArray[j].getPYL() / (double)(GDArray[i]->getGLB_QDright() - GDArray[i]->getGLB_QDleft())));
                if (0 == GDArray[i]->getZ())
                {
                    if (GDArray[i]->p12.y() == GDArray[i]->p34.y())
                    {
                        y = GDArray[i]->p12.y();
                    }
                    else
                    {
                        if (GDArray[i]->p34.y() > GDArray[i]->p12.y())
                        {
                            b = GDArray[i]->p34.y() - GDArray[i]->p12.y();
                            y = GDArray[i]->p12.y() + (b * ((x - GDArray[i]->p12.x()) / a));
                            //qDebug()<<"000------y="<<y<<"         a="<<a<<"      b="<<b;
                        }
                        else
                        {
                            b = GDArray[i]->p12.y() - GDArray[i]->p34.y();
                            y = GDArray[i]->p12.y() - (b * ((x - GDArray[i]->p12.x()) / a));
                            //qDebug()<<"111------y="<<y<<"         a="<<a<<"      b="<<b;
                        }

                    }
                }
                else if (1 == GDArray[i]->getZ()) //实际应用中计算
                {

                }
                else if (2 == GDArray[i]->getZ()) //实际应用中计算
                {

                }
                pt.setX(x);
                pt.setY(y);
                YDQArray[j].setYDQp(pt);
                break;
            }
        }
    }
    QDNameFlag = DCNameFlag = XHNameFlag = true; //初始化
}
void TStation::drawStation(QPainter *painter, int nElapsed, double nDiploid, unsigned char type)
{
    setGD();
    for (int i = 0; i < GDDCArray.size(); i++)
    {
        GDDCArray[i]->Draw(painter, nElapsed, nDiploid);
    }
    for (int i = 0; i < XHDArray.size(); i++)
    {
        XHDArray[i]->Draw(painter, nElapsed, nDiploid, type);
    }
    for (int i = 0; i < GDArray.size(); i++)
    {
        GDArray[i]->Draw(painter, nElapsed, nDiploid);
    }
    for (int i = 0; i < TextArray.size(); i++)
    {
        TextArray[i].Draw(painter, nElapsed, nDiploid);
    }
    for (int i = 0; i < JTXArray.size(); i++)
    {
        JTXArray[i].Draw(painter, nElapsed, nDiploid);
    }
    for (int i = 0; i < YDQArray.size(); i++)
    {
        YDQArray[i].Draw(painter, nElapsed, nDiploid);
    }
}

//成员变量封装函数实现
void TStation::setMainGD()
{
    int i, j, k, l = -1;
    T_GDDC *p, *q, *r, *s = NULL;
    for (unsigned int m = 0; m < QDArray.size(); m++)
    {
        if (QDArray[m].m_nChildNum > 1)
        {
            i = QDArray[m].m_nChild[0];
            j = QDArray[m].m_nChild[1];
            for (int x = 0; x < GDDCArray.size(); x++)
            {
                if (i == GDDCArray[x]->getCode())
                {
                    p = (T_GDDC*) & (GDDCArray[x]);
                }
                else if (j == GDDCArray[x]->getCode())
                {
                    q = (T_GDDC*) & (GDDCArray[x]);
                }
            }
            if (QDArray[m].m_nChildNum > 2)
            {
                k = QDArray[m].m_nChild[2];
                for (int x = 0; x < GDDCArray.size(); x++)
                {
                    if (k == GDDCArray[x]->getCode())
                    {
                        r = (T_GDDC*) & (GDDCArray[x]);
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
                //clearDrawGD();
            }
        }
    }
}

void TStation::PressCTCButton()
{
    if (v_CTCANArray.size() > 0)
    {
        if (v_CTCANArray[0])
        {
            v_CTCANArray[0]->setIsLCDown(true);
            //            qDebug() << "PressCTCButton " << v_CTCANArray[0]->getXHDName() << endl;
            DeaLWithAN(v_CTCANArray[0]);

            v_CTCANArray.erase(v_CTCANArray.begin());
        }
    }
}
void TStation::setGD()
{
    int nMainGD = -1;
    setMainGD();
    T_GDDC *gddc;
    for (int i = 0; i < QDArray.size(); i++)
    {
        if (QDArray[i].m_nChildNum > 1)
        {
            for (int k = 0; k < QDArray[i].m_nChildNum; k++)
            {
                for (int j = 0; j < GDDCArray.size(); j++)
                {
                    if (QDArray[i].m_nChild[k] == GDDCArray[j]->getCode())
                    {
                        gddc = (T_GDDC*) & (GDDCArray[j]);
                        if (gddc->isMainGD == true)
                        {
                            int nNext = gddc->setDrawGD(true);
                            int m = 1;
                            while (nNext != -1) //直到后继为空
                            {
                                m++;
                                for (int l = 0; l < GDDCArray.size(); l++)
                                {
                                    if (nNext == GDDCArray[l]->getCode())
                                    {
                                        gddc = (T_GDDC *)&GDDCArray[l];
                                        nNext = gddc->setDrawGD(true);
                                        break;
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

void TStation::JLProcOnTimer()
{
    //int size = v_JL.size();
    JLStr *jlstr;
    QString Record;
    if (CurrentJL.ExamID != -1)
    {
        jlstr = &CurrentJL;
        if ((jlstr->state == 0) && jlstr->v_AN[0]->getIsLCDown() && !jlstr->v_AN[1]->getIsLCDown())
            jlstr->state = 1;
        else if ((jlstr->state == 1) && jlstr->v_AN[0]->getIsLCDown() && jlstr->v_AN[1]->getIsLCDown())
            jlstr->state = 2;
        else if ((jlstr->state == 2) && jlstr->v_AN[0]->getIsLCDown() && jlstr->v_AN[1]->getIsLCDown())
        {
            Record = "#LCJL," + jlstr->v_AN[0]->getXHDName() + "-" + jlstr->v_AN[1]->getXHDName();
            jlstr->v_AN[0]->setIsLCDown(false);
            jlstr->v_AN[1]->setIsLCDown(false);
            v_ANArray.clear();
            jlstr->v_AN[0]->setXHDState(jlstr->XHXS);
            jlstr->state = 3;
            for (int j = 0; j < jlstr->v_DC.size(); j++)
            {
                jlstr->v_DC[j]->setQDColor(WHITE);
            }
            for (int j = 0; j < jlstr->v_QD.size(); j++)
            {
                jlstr->v_QD[j]->setQDColor(WHITE);
            }
            if (jlstr->Reopen)
                emit SendCaseCode(jlstr->CODE);
        }
        else if (jlstr->state == 3)
        {
            if (jlstr->v_AN[0]->getIsLCDown() && !jlstr->v_AN[1]->getIsLCDown())
            {
                v_ANArray.clear();
                jlstr->v_AN[0]->setIsLCDown(false);
                jlstr->v_AN[1]->setIsLCDown(false);
                if (jlstr->Reopen)
                    emit SendCaseCode(jlstr->CODE);
            }
        }
    }
}

void TStation::HandleANOnTimer()
{
    T_XHD *pXHD;
    T_XHD *FirstXHD = nullptr;
    T_XHD *XXFirstXHD = nullptr;
    bool SXFlag = 0;
    bool XXFlag = 0;
    int size = v_ANArray.size();
    QString Record = "";
    for (int i = 0; i < size; i++)
    {
        pXHD = v_ANArray[i];

        if (pXHD->getIsLCDown() || pXHD->getIsDCDown())
        {
            if (!SXFlag && pXHD->getSX())
            {
                FirstXHD = pXHD;
                SXFlag = 1;
                if (pXHD->getIsLCDown())
                    Record = "#LCJL," + pXHD->getXHDName();
                else
                    Record = "#DCJL," + pXHD->getXHDName();
            }
            else if (!XXFlag && !pXHD->getSX())
            {
                XXFirstXHD = pXHD;
                XXFlag = 1;
                if (pXHD->getIsLCDown())
                    Record = "#LCJL," + pXHD->getXHDName();
                else
                    Record = "#DCJL," + pXHD->getXHDName();
            }
            else if ((SXFlag == 1) && pXHD->getSX())
            {
                if ((CurrentJL.v_AN.size() >= 2) && (CurrentJL.v_AN[0] == v_ANArray[0]) && (CurrentJL.v_AN[1] == v_ANArray[1]))
                    continue;
                v_ANArray.clear();
                Record = Record + "-" + pXHD->getXHDName();
                FirstXHD->setIsLCDown(false);
                pXHD->setIsLCDown(false);
                FirstXHD->setIsDCDown(false);
                pXHD->setIsDCDown(false);
            }
            else if ((XXFlag == 1) && !pXHD->getSX())
            {
                v_ANArray.clear();
                Record = Record + "-" + pXHD->getXHDName();
                XXFirstXHD->setIsLCDown(false);
                pXHD->setIsLCDown(false);
                XXFirstXHD->setIsDCDown(false);
                pXHD->setIsDCDown(false);
            }
        }
    }
}

void TStation::DeaLWithAN(T_XHD *pXHD)
{
    v_ANArray.push_back(pXHD);
    for (int i = 0; i < v_ANArray.size(); i++)
    {
        //        qDebug() << "DeaLWithAN  " << v_ANArray[i]->getXHDName();
    }
    //    qDebug() << endl;
}
void TStation::SetCancle()
{
    cancelFlag = true;
}

void TStation::CancelCurrentJL()
{
    JLStr *jlstr = &CurrentJL;
    if (jlstr->state == 3)
    {
        jlstr->v_AN[0]->setXHDState(jlstr->v_AN[0]->getSafeLamp());
        jlstr->state = 0;
        for (int j = 0; j < jlstr->v_DC.size(); j++)
        {
            jlstr->v_DC[j]->setQDColor(BLUE);
        }
        for (int j = 0; j < jlstr->v_QD.size(); j++)
        {
            jlstr->v_QD[j]->setQDColor(BLUE);
        }
    }
}

void TStation::ConvertCTCCmd(QVector<T_XHD *> tempv_ANArray)
{
    int size = tempv_ANArray.size();
    int subsize = XHDArray.size();
    for (int i = 0; i < size; i++)
    {
        //        qDebug() << "ConvertCTCCmd " << i << "\t";
        for (int j = 0; j < subsize; j++)
        {
            if (tempv_ANArray[i]->getXHDName() == XHDArray[j]->getXHDName())
            {
                v_CTCANArray.push_back(XHDArray[j]);
                break;
            }
        }
    }
    //    qDebug() << endl;
}

void TStation::ClearScene()
{
    int size = XHDArray.size();
    for (int i = 0; i < size; i++)
    {
        XHDArray[i]->clearState();
    }
    size = GDDCArray.size();
    for (int i = 0; i < size; i++)
    {
        GDDCArray[i]->setQDColor(BLUE);
        GDDCArray[i]->setDCWZ(DCDW);
    }
    size = GDArray.size();
    for (int i = 0; i < size; i++)
    {
        GDArray[i]->setQDColor(BLUE);
    }
}
void TStation::JLConfigRead(QString FileName)
{
    QFile tFile(FileName);
    if (!tFile.exists())
    {
        QMessageBox::information(nullptr, "提示", "进路文件不存在！");
        return;
    }
    if (!tFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream tText(&tFile);

    QString m_str{};
    QString layerName{};
    QString  Getstring{};
    tText.setCodec("UTF-8");
    JLStr jlstr;
    jlstr.Init();
    QStringList strList;
    while (!tText.atEnd())
    {
        m_str = tText.readLine();
        if (m_str == "")
        {
            break;
        }
        else if (m_str == "##")
        {
            v_JL.push_back(jlstr);
            jlstr.Init();
        }
        else if (m_str.left(7) == "ExamID=")
        {
            Getstring = m_str.mid(7, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            jlstr.ExamID = Getstring.toInt();
        }
        else if (m_str.left(5) == "CODE=")
        {
            Getstring = m_str.mid(5, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            jlstr.CODE = Getstring.toInt();
        }
        else if (m_str.left(5) == "JLAN=")
        {
            Getstring = m_str.mid(5, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            strList =  Getstring.split(",");
            for (int i = 0; i < strList.size(); i++)
            {
                jlstr.v_AN.push_back(GetXHByName(strList[i]));
            }
        }
        else if (m_str.left(3) == "DC=")
        {
            Getstring = m_str.mid(3, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            strList =  Getstring.split(",");
            for (int i = 0; i < strList.size(); i++)
            {
                jlstr.v_DC.push_back(GetGDDCByName(strList[i]));
            }
        }
        else if (m_str.left(3) == "QD=")
        {
            Getstring = m_str.mid(3, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            strList =  Getstring.split(",");
            for (int i = 0; i < strList.size(); i++)
            {
                jlstr.v_QD.push_back(GetGDByName(strList[i]));
            }
        }
        else if (m_str.left(5) == "XHXS=")
        {
            Getstring = m_str.mid(5, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            if (Getstring == "H")
                jlstr.XHXS = XHD_HD;
            else if (Getstring == "U")
                jlstr.XHXS = XHD_UD;
        }
        else if (m_str.left(7) == "Reopen=")
        {
            Getstring = m_str.mid(7, m_str.indexOf(("//"), 0) - m_str.indexOf(("="), 0) - 1);
            jlstr.Reopen = Getstring.toInt();
        }
    }
    tFile.close();
}

void TStation::SceneRead(QString FileName)
{
    QFile tFile(FileName);
    if (!tFile.exists())
    {
        QMessageBox::information(nullptr, "提示", "场景文件不存在！");
        return;
    }
    if (!tFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream tText(&tFile);

    QString m_str{};
    QString layerName{};
    QString  Getstring{};
    tText.setCodec("UTF-8");
    SceneStr scene;
    QStringList strList;
    T_GD *T_GD = nullptr;
    T_GDDC *T_GDDC = nullptr;
    T_XHD *pXH = nullptr;
    while (!tText.atEnd())
    {
        m_str = tText.readLine();
        if (m_str == "")
        {
            break;
        }
        else if (m_str.left(12) == "TitleNumber:")
        {
            Getstring = m_str.mid(12);
            scene.TitleID = Getstring.toInt();
            for (;;)
            {
                m_str = tText.readLine();
                if (m_str.left(4) == "#END")
                {
                    v_SceneArray.push_back(scene);
                    scene.Init();
                    break;
                }
                if (m_str.left(4) == "##QD")
                {
                    m_str = tText.readLine();
                    strList =  m_str.split("|");
                    for (int m = 0; m < strList.size(); m++)
                    {
                        Getstring = strList[m].left(strList[m].indexOf(":"));
                        T_GD = GetGDByName(Getstring);
                        Getstring = strList[m].mid(strList[m].indexOf(":") + 1);
                        if (T_GD)
                        {
                            scene.v_QDState.push_back(Getstring);
                            scene.v_QD.push_back(T_GD);
                        }
                        T_GD = nullptr;
                    }
                }
                else if (m_str.left(4) == "##DC")
                {
                    m_str = tText.readLine();
                    strList =  m_str.split("|");
                    for (int m = 0; m < strList.size(); m++)
                    {
                        Getstring = strList[m].left(strList[m].indexOf(":"));
                        T_GDDC = GetGDDCByName(Getstring);
                        Getstring = strList[m].mid(strList[m].indexOf(":") + 1);
                        if (T_GDDC)
                        {
                            scene.v_DCState.push_back(Getstring);
                            scene.v_DC.push_back(T_GDDC);
                        }
                        T_GDDC = nullptr;
                    }
                }
                else if (m_str.left(4) == "##XH")
                {
                    m_str = tText.readLine();
                    strList =  m_str.split("|");
                    for (int m = 0; m < strList.size(); m++)
                    {
                        Getstring = strList[m].left(strList[m].indexOf(":"));
                        pXH = GetXHByName(Getstring);
                        Getstring = strList[m].mid(strList[m].indexOf(":") + 1);
                        if (pXH)
                        {
                            scene.v_XHState.push_back(Getstring);
                            scene.v_AN.push_back(pXH);
                        }
                        pXH = nullptr;
                    }
                }
            }
        }
    }
    tFile.close();
}

T_XHD* TStation::GetXHByName(QString name)
{
    int size;
    size = this->XHDArray.size();
    for (int i = 0; i < size; i++)
    {
        if (XHDArray[i]->getXHDName() == name)
            return XHDArray[i];
    }
    return nullptr;
}

T_GDDC* TStation::GetGDDCByName(QString name)
{
    int size;
    size = this->GDDCArray.size();
    for (int i = 0; i < size; i++)
    {
        if (GDDCArray[i]->getGDDCName() == name)
            return GDDCArray[i];
    }
    return nullptr;
}

T_GD* TStation::GetGDByName(QString name)
{
    int size;
    size = this->GDArray.size();
    for (int i = 0; i < size; i++)
    {
        if (GDArray[i]->getGDName() == name)
            return GDArray[i];
    }
    return nullptr;
}
void TStation::setStationName(QString name)
{
    m_strStationName = name;
}
QString TStation::getStationName()
{
    return m_strStationName;
}
void TStation::setStationID(int id)
{
    m_nStationID = id;
}
int TStation::getStationID()
{
    return m_nStationID;
}
void TStation::setStationType(int type)
{
    m_nStationType = type;
}
int TStation::getStationType()
{
    return m_nStationType;
}
void TStation::setLSProducer(QString producer)
{
    m_strLS_Producer = producer;
}
QString TStation::getLSProducer()
{
    return m_strLS_Producer;
}
void TStation::setIsGT(bool flag)
{
    isGT = flag;
}
bool TStation::getIsGT()
{
    return isGT;
}
void TStation::setIsZCZX(bool flag)
{
    isZCZX = flag;
}
bool TStation::getIsZCZX()
{
    return isZCZX;
}
void TStation::setRBCCode(int rbcCode)
{
    m_nRBCCode = rbcCode;
}
int TStation::getRBCCode()
{
    return m_nRBCCode;
}
void TStation::setOffSet(QPoint offset)
{
    offsetPt.setX(offset.x());
    offsetPt.setY(offset.y());
}
int TStation::getOffSetX()
{
    return offsetPt.x();
}
int TStation::getOffSetY()
{
    return offsetPt.y();
}

void TStation::setScene(int TitleID)
{
    int size = this->v_SceneArray.size();
    for (int i = 0; i < size; i++)
    {
        if (v_SceneArray[i].TitleID == TitleID)
        {
            for (int m = 0; m < v_SceneArray[i].v_QD.size(); m++)
            {
                if (v_SceneArray[i].v_QDState[m] == "占用")
                    v_SceneArray[i].v_QD[m]->setQDColor(RED);
                else if (v_SceneArray[i].v_QDState[m] == "锁闭")
                    v_SceneArray[i].v_QD[m]->setQDColor(WHITE);
            }
            for (int m = 0; m < v_SceneArray[i].v_AN.size(); m++)
            {
                if (v_SceneArray[i].v_XHState[m] == "L")
                    v_SceneArray[i].v_AN[m]->setXHDState(XHD_LD);
                else if (v_SceneArray[i].v_XHState[m] == "U")
                    v_SceneArray[i].v_AN[m]->setXHDState(XHD_UD);
            }
            for (int m = 0; m < v_SceneArray[i].v_DC.size(); m++)
            {
                if (v_SceneArray[i].v_DCState[m] == "反位")
                    v_SceneArray[i].v_DC[m]->setDCWZ(DCFW);
                else if (v_SceneArray[i].v_DCState[m] == "占用")
                    v_SceneArray[i].v_DC[m]->setQDColor(RED);
            }
            break;
        }
    }
}

void TStation::InitAllFactor()
{

}

void TStation::QDNameShowOrHide()
{
    QDNameFlag = !QDNameFlag;
    for (int i = 0; i < GDArray.size(); i++)
    {
        GDArray[i]->setDisplayName(QDNameFlag);
    }
    for (int i = 0; i < TextArray.size(); i++)
    {
        if(DCQDNAMETEXT == TextArray[i].getTextType())
        {
            TextArray[i].IsQDTextShowFlag = QDNameFlag;
        }
    }
}
void TStation::DCNameShowOrHide()
{
    DCNameFlag = !DCNameFlag;
    for (int i = 0; i < GDDCArray.size(); i++)
    {
        GDDCArray[i]->setDisplayName(DCNameFlag);
    }
}
void TStation::XHNameShowOrHide(){
    XHNameFlag = !XHNameFlag;
    for (int i = 0; i < XHDArray.size(); i++)
    {
        XHDArray[i]->setDisplayName(XHNameFlag);
    }
}


