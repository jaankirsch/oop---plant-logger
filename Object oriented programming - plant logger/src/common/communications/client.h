#pragma once

#include <memory>
#include <QObject>
#include <QWidget>
#include <QFile>

#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QtEndian>


#include <iostream>


namespace IAS0410::Communication {
class DataHandler;
class ConnectionHandler;

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

    bool is_transmitting;


signals:
    void signal_fileOpened(bool);
    void signal_transmission(bool);
    void signal_connected(bool);
    void parsedData(QString);
    void sendCommand(QByteArray);
    void bufferedData(QByteArray);


private:
    ConnectionHandler *m_pHandler = nullptr;
    QThread *m_pConnectionThread = nullptr;

    DataHandler *m_pData = nullptr;
    QThread *m_pDataThread = nullptr;

    //std::unique_ptr<ConnectionHandler> m_handler;
    QString currentFile = "";
    QByteArray CommandCreator(QByteArray);



public slots:
    void openFile(QString);
    void closeFile();
    void setupConnection();
    void closeConnection();
    void startTransmission();
    void stopTransmission();
    void dataBuffer(QByteArray);
    void parsedBuffer(QString);
    void fileStatus(bool);
    void connectionStatus (quint8);
};

}
