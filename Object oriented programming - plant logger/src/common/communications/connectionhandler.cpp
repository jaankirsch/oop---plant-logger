#include "connectionhandler.h"
#include <iostream>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtEndian>


namespace IAS0410::Communication {

    ConnectionHandler::ConnectionHandler(QObject * parent): QThread(parent) {
        //qDebug() << QThread::currentThreadId() << "constructor";
    }

    ConnectionHandler::~ConnectionHandler() {

        qDebug() << "socket closing";
    }

    void ConnectionHandler::startSocket() {

        m_pSocket.reset(new QTcpSocket());
        connect(m_pSocket.data(), & QTcpSocket::readyRead, [this] {
            ConnectionHandler::readyRead(data);
        });
        connect(m_pSocket.data(), & QTcpSocket::errorOccurred, [this] {
            emit socketStatus(4);
        });

        m_pSocket -> connectToHost("127.0.0.1", 1234);

        if (m_pSocket -> waitForConnected(3000)) {
            qDebug() << "connected";

            emit socketStatus(1);
        }
        exec();
    }

    void ConnectionHandler::readyRead(QByteArray data) {

        data = m_pSocket -> readAll();
        emit sendData(data);

    }

    void ConnectionHandler::closeConnection(bool) {
        qDebug() << "close connection";
        m_pSocket -> close();
    }

    void ConnectionHandler::recievedCommand(QByteArray command) {
        m_pSocket -> write(command);
        //qDebug() <<command;
    }
}
