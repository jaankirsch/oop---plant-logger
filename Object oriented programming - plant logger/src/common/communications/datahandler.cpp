#include "datahandler.h"
#include <iostream>
#include <QVector>
#include <QTime>
#include <QDateTime>
#include <QTextStream>


namespace IAS0410::Communication {

    DataHandler::DataHandler(QString fileName, QObject * parent): QThread(parent) {
        fileToOpen = fileName;
    }

    DataHandler::~DataHandler() {
        m_pFile.data() -> close();
        emit fileStatus(false);
        qDebug() << "file closed destrucor";

    }

    void DataHandler::incomingData(QByteArray rawdata) {
        QString fullMeasurment;
        QString timeDate = QDateTime::currentDateTime().toString(Qt::TextDate);
        fullMeasurment.append(timeDate);
        fullMeasurment.append("\n");
        //qDebug() << timeDate;

        QDataStream ds(rawdata);
        ds.setByteOrder(QDataStream::LittleEndian);
        qint32 messagelength;
        ds >> messagelength;

        if (messagelength < 25) {
            DataHandler::shortMessage(rawdata, messagelength);
            return;
        }

        qint32 channelCount;
        ds >> channelCount;
        //qDebug() <<"Channelcount:"<<channelCount;
        QByteArray leftover = rawdata.last(messagelength - 8);

        for (int i = 0; i < channelCount; i++) {
            QDataStream ds(leftover);
            ds.setByteOrder(QDataStream::LittleEndian);
            qint32 measurepoints;
            ds >> measurepoints;
            leftover = leftover.last(leftover.size() - 4);
            QString channelName = leftover.constData();
            fullMeasurment.append(channelName);
            fullMeasurment.append("\n");
            leftover = leftover.last(leftover.size() - channelName.size() - 1);
            for (int i = 0; i < measurepoints; i++) {
                QString measurementName = leftover.constData();
                leftover = leftover.last(leftover.size() - measurementName.size() - 1);
                QDataStream ss(leftover);
                ss.setByteOrder(QDataStream::LittleEndian);
                double dMeasurment;
                int iMeasurment;
                if (measurementName.contains("level", Qt::CaseInsensitive) || measurementName.contains("concentration", Qt::CaseInsensitive)) {
                    ss >> iMeasurment;
                    leftover = leftover.last(leftover.size() - 4);
                    measurementName.append(QString(" %1 %").arg(iMeasurment));
                } else {
                    ss >> dMeasurment;
                    leftover = leftover.last(leftover.size() - 8);
                    if (measurementName.contains("temp", Qt::CaseInsensitive)) measurementName.append(QString(" %1 °C").arg(dMeasurment, 0, 'f', 1));
                    if (measurementName.contains("flow", Qt::CaseInsensitive)) measurementName.append(QString(" %1 m³/s").arg(dMeasurment, 0, 'f', 3));
                    if (measurementName.contains("ph", Qt::CaseInsensitive)) measurementName.append(QString(" %1 ").arg(dMeasurment, 0, 'f', 1));
                    if (measurementName.contains("conduct", Qt::CaseInsensitive)) measurementName.append(QString(" %1 S/m").arg(dMeasurment, 0, 'f', 2));
                }
                fullMeasurment.append(measurementName);
                fullMeasurment.append("\n");
                //qDebug() << measurementName;
            }
        }
        if (messagelength > 30) {
            emit connectionStatus(3);
        }
        emit parsedData(fullMeasurment);
        DataHandler::writeToFile(fullMeasurment);
    }

    void DataHandler::openFileThread() {
        m_pFile.reset(new QFile(fileToOpen));
        m_pFile.data() -> open(QFile::Append | QFile::Text);

        if (!m_pFile) {

            emit fileStatus(false);
            return;
        }
        emit fileStatus(true);
        qDebug() << "file opened";
        exec();
    }

    void DataHandler::shortMessage(QByteArray rawdata, quint32 messagelength) {
        QByteArray message = rawdata.last(messagelength - 4);
        auto str = QString::fromUtf16(reinterpret_cast <
            const ushort * > (message.constData()));

        if (str == "Identify") {
            emit connectionStatus(0);

        }
        //if (str == "Accepted") emit connectionStatus(1);
        if (str == "Rejected") emit connectionStatus(2);
        return;
        emit parsedData(str);
    }

    void DataHandler::writeToFile(QString parsed) {
        QTextStream out(m_pFile.data());
        out << parsed << "\n";
    }

}
