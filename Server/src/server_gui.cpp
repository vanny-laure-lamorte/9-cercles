#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    qRegisterMetaType<std::vector<std::vector<std::string>>>("std::vector<std::vector<std::string>>");

    MainWindow mainWindow;
    mainWindow.setWindowTitle("LPTF Server GUI");
    mainWindow.resize(800, 500);
    mainWindow.show();

    return app.exec();
}
