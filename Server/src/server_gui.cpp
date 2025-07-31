#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qRegisterMetaType<vector<vector<string>>>("vector<vector<string>>");

    MainWindow mainWindow;
    mainWindow.setWindowTitle("LPTF Server GUI");
    mainWindow.show();

    return app.exec();
}
