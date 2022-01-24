#include "mainpanel.h"

#include "buttons/custom_button.h"
#include "textedits/custom_text_edit.h"
#include "src/common/communications/client.h"

#include <QGridLayout>
#include <QApplication>
#include <iostream>


namespace IAS0410::GUI {

MainPanel::MainPanel(IAS0410::Communication::Client* client, QWidget* parent) :
    QWidget{parent},
    m_buttonExit{new CustomButton(this)},
    m_buttonConnect{new CustomButton(this)},
    m_buttonDisconnect{new CustomButton(this)},
    m_buttonStartMeasure{new CustomButton(this)},
    m_buttonStopMeasure{new CustomButton(this)},
    m_buttonOpen{new CustomButton(this)},
    m_buttonClose{new CustomButton(this)},
    m_textEdit{new CustomTextEdit(this)},
    m_client{client}
{
    setupStyle();
    setupLayout();
    setupWidgets();
}

void MainPanel::setupStyle()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
}

void MainPanel::setupLayout()
{
    setMinimumSize(600, 400);

    auto l = new QGridLayout(this);
    l->setColumnMinimumWidth(0, 50);
    l->setColumnStretch(0, 25);
    l->setColumnStretch(1, 50);
    l->setColumnMinimumWidth(2, 50);
    l->setColumnStretch(2, 25);
    l->setRowStretch(0, 80);
    l->setRowStretch(1, 20);
    l->setHorizontalSpacing(200);

    l->addWidget(m_textEdit, 0, 0, 1, 3);

    l->addWidget(m_buttonOpen, 1, 0);
    l->addWidget(m_buttonClose, 1, 1);
    l->addWidget(m_buttonConnect, 2, 0);
    l->addWidget(m_buttonDisconnect, 2, 1);
    l->addWidget(m_buttonStartMeasure, 3, 0);
    l->addWidget(m_buttonStopMeasure, 3, 1);
    l->addWidget(m_buttonExit, 4, 0,4,1);
}



void MainPanel::setupWidgets()
{
    m_buttonExit->setText("Exit");
    connect(m_buttonExit, &QPushButton::pressed, this,
        &QCoreApplication::quit, Qt::QueuedConnection);

    m_buttonOpen->setText("Open");
    m_buttonOpen->setEnabled(true);
    connect(m_buttonOpen, &QPushButton::pressed,  this,
            &MainPanel::FileDialog);
    connect(this, &MainPanel::FileName,  m_client,
            &IAS0410::Communication::Client::openFile);

    m_buttonConnect->setText("Connect");
    m_buttonConnect->setEnabled(false);
    connect(m_buttonConnect, &QPushButton::pressed, m_client,
            &IAS0410::Communication::Client::setupConnection);

    m_buttonStartMeasure->setText("Start");
    m_buttonStartMeasure->setEnabled(false);
    connect(m_buttonStartMeasure, &QPushButton::pressed, m_client,
            &IAS0410::Communication::Client::startTransmission);


    m_buttonStopMeasure->setText("Stop");
    m_buttonStopMeasure->setEnabled(false);
    connect(m_buttonStopMeasure, &QPushButton::pressed, m_client,
            &IAS0410::Communication::Client::stopTransmission);

    m_buttonDisconnect->setText("Disconnect");
    m_buttonDisconnect->setEnabled(false);
    connect(m_buttonDisconnect, &QPushButton::pressed, m_client,
            &IAS0410::Communication::Client::closeConnection);


    m_buttonClose->setText("Close");
    m_buttonClose->setEnabled(false);
    connect(m_buttonClose, &QPushButton::pressed, m_client,
            &IAS0410::Communication::Client::closeFile);


    // Capture client connected signals
    connect(m_client, &IAS0410::Communication::Client::signal_fileOpened, this, &MainPanel::onOpened);
    connect(m_client, &IAS0410::Communication::Client::signal_connected, this, &MainPanel::onConnect);
    connect(m_client, &IAS0410::Communication::Client::signal_transmission, this, &MainPanel::onTransmission);
    connect(m_client, &IAS0410::Communication::Client::parsedData, this, &MainPanel::showData);

    /*
     *
     *
    m_buttonDisconnect->setText("Disconnect");
    m_buttonDisconnect->setEnabled(false);
    connect(m_buttonDisconnect, &QPushButton::pressed, [this] {
        m_textEdit->append("Disconnect");
    });

        m_buttonOpen->setText("Open");
        m_buttonOpen->setEnabled(true);
        connect(m_buttonConnect, &QPushButton::pressed, [this] () {
            m_client->openFile();
        });

        m_buttonConnect->setText("Connect");
        m_buttonConnect->setEnabled(true);
        connect(m_buttonConnect, &QPushButton::pressed, [this] () {
            m_client->setupConnection();
        });
        connect(m_buttonConnect, &QPushButton::pressed, [this] () {
            //m_buttonConnect->setEnabled(false);
            m_client->setupConnection();
        });
    */


}

void MainPanel::onOpened(bool opened)
{
    if (!opened) QMessageBox::warning(this, "Warning", "no file opened");
    opened ? m_buttonConnect->setEnabled(true) : m_buttonConnect->setEnabled(false);
    opened ? m_buttonConnect->setEnabled(true) : m_buttonConnect->setEnabled(false);
    m_buttonDisconnect->setEnabled(false);
    m_buttonStartMeasure->setEnabled(false);
    m_buttonStopMeasure->setEnabled(false);
    opened ? m_buttonOpen->setEnabled(false) : m_buttonOpen->setEnabled(true);
    opened ? m_buttonClose->setEnabled(true) : m_buttonClose->setEnabled(false);

}

void MainPanel::onConnect(bool connected)
{
    m_buttonConnect->setEnabled(false);
    m_buttonDisconnect->setEnabled(true);
    m_buttonStartMeasure->setEnabled(true);
    m_buttonStopMeasure->setEnabled(false);
    m_buttonOpen->setEnabled(false);
    m_buttonClose->setEnabled(true);
}

void MainPanel::onTransmission(bool transmission)
{
    m_buttonConnect->setEnabled(false);
    m_buttonDisconnect->setEnabled(true);
    m_buttonStartMeasure->setEnabled(false);
    m_buttonStopMeasure->setEnabled(true);
    m_buttonOpen->setEnabled(false);
    m_buttonClose->setEnabled(false);
}

void MainPanel::showData(QString message)
{
    //qDebug() << message;
    m_textEdit->append(message);
}

void MainPanel::FileDialog()
{
QString fileName = QFileDialog::getOpenFileName(this, "Open log file");
emit FileName(fileName);
}

}
