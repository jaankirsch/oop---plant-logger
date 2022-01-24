#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QThread>
#include <QObject>
#include <memory>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

#include <QtEndian>
#include <iostream>


#include <QVector>

#include <QDateTime>
#include <QByteArray>

#include <QMutex>
#include <QWaitCondition>



namespace IAS0410::Communication {
class Client;
class ConnectionHandler;
class DataHandler : public QThread
{
    Q_OBJECT
public:
    explicit DataHandler(QString fileName, QObject *parent = nullptr);
    ~DataHandler();

    QScopedPointer<QFile> m_pFile;

private:
    QString fileToOpen;
    void shortMessage(QByteArray, quint32);
    void writeToFile(QString);


public slots:
    void openFileThread();
    void incomingData(QByteArray);


signals:
    void fileStatus(bool);
    void connectionStatus(quint8);
    void parsedData(QString);
    void fullMeasurment(QString);
};

}
#endif // DATAHANDLER_H
