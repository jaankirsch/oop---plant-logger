#include "client.h"

#include "connectionhandler.h"

#include "datahandler.h"


namespace IAS0410::Communication {

    Client::Client() {}

    Client::~Client() {
        //exit logic
    }

    void Client::openFile(QString fileName) {
        m_pData = new DataHandler(fileName);
        m_pDataThread = new QThread;
        m_pData -> moveToThread(m_pDataThread);

        QObject::connect(m_pDataThread, & QThread::started, m_pData, & DataHandler::openFileThread, Qt::DirectConnection);
        QObject::connect(m_pDataThread, & QThread::finished, m_pData, & QObject::deleteLater);
        QObject::connect(m_pData, & DataHandler::connectionStatus, this, & Client::connectionStatus);
        QObject::connect(this, & Client::bufferedData, m_pData, & DataHandler::incomingData, Qt::QueuedConnection);
        QObject::connect(m_pData, & DataHandler::fileStatus, this, & Client::fileStatus);

        m_pDataThread -> start();
    }

    void Client::setupConnection() {
        m_pHandler = new ConnectionHandler;
        m_pConnectionThread = new QThread;
        m_pHandler -> moveToThread(m_pConnectionThread);

        QObject::connect(m_pConnectionThread, & QThread::started, m_pHandler, & ConnectionHandler::startSocket, Qt::DirectConnection);
        QObject::connect(m_pHandler, & ConnectionHandler::sendData, this, & Client::dataBuffer, Qt::QueuedConnection);
        QObject::connect(m_pConnectionThread, & QThread::finished, m_pHandler, & QObject::deleteLater);
        QObject::connect(m_pHandler, & ConnectionHandler::socketStatus, this, & Client::connectionStatus, Qt::QueuedConnection);
        QObject::connect(this, & Client::sendCommand, m_pHandler, & ConnectionHandler::recievedCommand, Qt::QueuedConnection);
        QObject::connect(m_pData, & DataHandler::parsedData, this, & Client::parsedBuffer, Qt::QueuedConnection);
        QObject::connect(this, & Client::signal_fileOpened, m_pHandler, & ConnectionHandler::closeConnection, Qt::QueuedConnection);

        m_pConnectionThread -> start();
    }

    void Client::startTransmission() {
        QObject::connect(this, & Client::sendCommand, m_pHandler, & ConnectionHandler::recievedCommand, Qt::QueuedConnection);
        is_transmitting = true;
        emit sendCommand(Client::CommandCreator("Start"));
        emit signal_transmission(true);
    }

    void Client::stopTransmission() {
        is_transmitting = false;
        emit sendCommand(Client::CommandCreator("Break"));
        emit signal_transmission(false);
        emit signal_connected(true);
    }

    void Client::closeConnection() {
        emit signal_fileOpened(true);
        m_pConnectionThread -> quit();
    }

    void Client::closeFile() {
        if (m_pHandler) Client::closeConnection();
        m_pDataThread -> quit();
    }

    QByteArray Client::CommandCreator(QByteArray command) {

        QByteArray output;
        //qDebug() << "output: " << command;
        char size[4];
        qsizetype length = command.QByteArray::length();
        qint32 length2 = length * 2 + 6;
        qToLittleEndian(length2, size);
        output.QByteArray::append(size, 4);

        for (int i = 0; i < length; i++) {
            output.QByteArray::append(command[i]);
            output.QByteArray::append("\0", 1);
        }
        output.QByteArray::append("\0\0", 2);
        return output;
    }

    void Client::fileStatus(bool status) {
        emit signal_fileOpened(status);
        status ? emit parsedData("file opened") : emit parsedData("file closed");
    }

    void Client::connectionStatus(quint8 is) {
        if (is == 0) {
            emit sendCommand(Client::CommandCreator("coursework"));
            emit parsedData("Identify");
        };
        if (is == 1) {
            emit signal_connected(true);
            emit parsedData("Accepted");
        }

        if (is == 2) emit signal_connected(false);

        if (is == 3 && is_transmitting) emit sendCommand(Client::CommandCreator("Ready"));
        if (is == 4) {
            emit parsedData("emulator connection error");
            emit signal_fileOpened(true);
        }
        /*     if (is == 5) {
                 emit parsedData("emulator connection error");
                 emit signal_fileOpened(true);
             }*/
    }

    void Client::dataBuffer(QByteArray buffer) {
        emit bufferedData(buffer);
    }

    void Client::parsedBuffer(QString readyresult) {
        QString buf = readyresult;
        emit parsedData(buf);
    }

}
