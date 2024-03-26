#ifndef REPETITIONWIDGET_H
#define REPETITIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include "PullFlow/PullFlow.h"
#include "TCPSocket/ServerSideV.h"
#include "TextAnalysis/TextAnalysis.h"
#include "PullFlow/AudioPlay.h"
#include "PullFlow/VideoPlay.h"
#include "FileRecord/FileRecord.h"
#include "TextDisplay.h"

#pragma execution_character_set("utf-8")

namespace Ui {
class RepetitionWidget;
}

class RepetitionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepetitionWidget(QWidget *parent = nullptr);
    ~RepetitionWidget();
    void PullFlowStart(QString url);
    void timerEvent(QTimerEvent * ev);
private slots:
    void onRecive(QByteArray);
    void onNewStream(QByteArray);
    void onPullFlowError();
    void onRecordEnd();
    void VideoRemuxing(QString filePath);

    void on_comboBox_currentIndexChanged(int index);

signals:
    void repetitionEnd();

private:
    Ui::RepetitionWidget *ui;

    PullFlow* m_pPull = nullptr;
    AudioPlay* audio = nullptr;
    VideoPlay* video = nullptr;
    ServerSideV* tcpServer  = nullptr;
    TextAnalysis* textAnalysis = nullptr;

    FileRecord* record = nullptr;
    StationFrame* station = nullptr;

    bool isRecording = false;

    QVector<QString> urlList;
    QString nowUrl;

    QLabel* lastLabel = nullptr;
    TextDisplay* display = nullptr;
    int timeId = 0;
};

#endif // REPETITIONWIDGET_H
