#ifndef SERVERSIDEV_H
#define SERVERSIDEV_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QAbstractSocket>


class ServerSideV : public QObject
{
     Q_OBJECT
public:
    ServerSideV(QObject* parent = nullptr);
    ~ServerSideV();

private:
    QByteArray HeartBeat();
    QByteArray Receipt();
    void Unpacking(QByteArray array, QTcpSocket* m_pSocket);

public slots:
    void onConnection();
    void onError(QAbstractSocket::SocketError);

signals:
    void ReceiveData(QByteArray);
    void NewStream(QByteArray);

private:
    QTcpServer* m_pServer = nullptr;
    QVector<QTcpSocket*> m_pSocketList;

    //QFile* log = nullptr;

};

#endif // SERVERSIDEV_H
