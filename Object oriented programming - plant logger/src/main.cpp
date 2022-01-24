#include <QApplication>

#include "src/gui/mainwindow.h"
#include "common/communications/client.h"
#include "test.h"
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    auto* client = new IAS0410::Communication::Client();
    IAS0410::GUI::MainWindow mainWindow(client);

    mainWindow.show();
    std::cout << "Custon value: " << IAS0410::returns4() << std::endl;

    return app.exec();
}
