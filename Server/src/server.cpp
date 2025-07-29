#include "LPTF_Server.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Lancer ton serveur
    try {
        LPTF_Server server("127.0.0.1", 12345);
        //server.run();
    } catch (const std::exception& e) {
        cerr << "Error : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    MainWindow window;

    window.show();

    return app.exec();
}
