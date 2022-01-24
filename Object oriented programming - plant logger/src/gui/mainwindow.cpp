#include "mainwindow.h"

#include "src/gui/widgets/mainpanel.h"
#include "src/common/communications/client.h"

namespace IAS0410::GUI {

MainWindow::MainWindow(IAS0410::Communication::Client* client, QWidget* parent) :
    QMainWindow{parent},
    m_panel{new MainPanel(client, this)},
    m_client{client}
{
    setCentralWidget(m_panel);
}

MainWindow::~MainWindow()
{
    delete m_client; // explicitly delete here because you allocate it in main.cpp which is not managed
}

}
