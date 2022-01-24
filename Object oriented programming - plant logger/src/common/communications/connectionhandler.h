#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

#include <QObject>
#include <QByteArray>

#include <QMutex>
#include <QWaitCondition>


namespace IAS0410 {
namespace Communication {
class Client;
class ConnectionHandler : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionHandler( QObject *parent = nullptr);
    ~ConnectionHandler();

    QScopedPointer<QTcpSocket> m_pSocket;

signals:
    void socketStatus(quint8);
    void sendData(QByteArray);

private slots:
    void readyRead(QByteArray);

public slots:
    void closeConnection(bool);
    void recievedCommand(QByteArray);
    void startSocket();

private:

    bool on_transmission = false;
    QByteArray data;
    QByteArray text;
};
}
}
#endif // CONNECTIONHANDLER_H
