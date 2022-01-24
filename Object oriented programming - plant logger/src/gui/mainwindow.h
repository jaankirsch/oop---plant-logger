#pragma once

#include <QMainWindow>
#include "src/gui/mainwindow.h"
#include "src/common/communications/client.h"

namespace IAS0410 {
namespace Communication {
class Client;
}

namespace GUI{

class MainPanel;
class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(IAS0410::Communication::Client* client, QWidget* parent = nullptr);
        ~MainWindow();

    private:
        MainPanel* m_panel;
        IAS0410::Communication::Client* m_client;
        //IAS0410::Communication::Client& m_client;
    };
}

}
