#pragma once

#include <QWidget>
#include <QString>
#include <QFileDialog>


namespace IAS0410 {
namespace Communication {
class Client;
}



namespace GUI {

class CustomButton;
class CustomTextEdit;
class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(IAS0410::Communication::Client* client, QWidget* parent = nullptr);


public slots:
    void onOpened(bool);
    void onConnect(bool);
    void onTransmission(bool); // could be messages from emulator?
    void showData(QString);

private slots:
    void FileDialog();

signals:
    void FileName(QString);
    

private:
    bool m_connected;
    bool opened{false};

private:
    void setupStyle();
    void setupLayout();
    void setupWidgets();

private:
    CustomButton* m_buttonExit;

    CustomButton* m_buttonConnect;
    CustomButton* m_buttonDisconnect;

    CustomButton* m_buttonStartMeasure;
    CustomButton* m_buttonStopMeasure;

    CustomButton* m_buttonOpen;
    CustomButton* m_buttonClose;
    CustomTextEdit* m_textEdit;

    IAS0410::Communication::Client* m_client;
};

}
}
