#ifndef CTRAIN_H
#define CTRAIN_H

#include <QObject>
#define LEFT  0xa5
#define RIGHT 0x5a
#define DCJL 0x55
#define LCJL 0x5a
class CTrain : public QObject
{
    Q_OBJECT
public:
    explicit CTrain(QObject *parent = nullptr);

    QString type;
    QString name;

    QString  TrainNumber;/*列车车次*/
    QString  Train_Position ;/*列车位置*/
    unsigned int ChannelIndex;//列车所属车站索引
    int StationIndex;
    QString  Train_Staion ;/*列车位置车站*/
    QString  Train_SX ;/*列车行驶方向*/
    QString  Train_Type ;/*列车机车属性*/
    QString Stop_state;//列车当前状态
    bool SXFlag;/*列车行驶方向按钮按下标识符*/
    bool StartFlag;/*停/发车按钮按下标识符*/
    bool Manualstart;/*手动发车按下标识符*/
    bool TypeFlag;/*列车机车属性按钮按下标识符*/
    bool Delte;/*删除标识符*/
    int ANtime;/*列车按钮时间*/
    bool Show;/*列车显示标识符*/
    bool Train_state;/*列车当前状态*/
    int timeCount = 0;//车辆刷新计时  如果1200MS没有收到列车信息 删车
    int nSpeed;//车速（nSpeed*2+50=实际车速KM/h）
    bool XgspdFlag;//修改车速按钮按下标识符 new,20180817

    unsigned int nQY_Type;  //列车牵引类型  1-电力   0-非电力
    unsigned int nCX_Info;  //列车超限信息  0-无超限 1-一级超限 2-二级超限 3-三级超限 4-超级超限
    unsigned int nTrainNumType_Count;  //列车类型序号
    unsigned int DevCode;//区段编号

signals:

};

#endif // CTRAIN_H
