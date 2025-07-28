#include "LPTF_Server.h"
#include "mainwindow.h"           // Ta classe Qt
#include <QApplication>           // Qt GUI
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Qt initialise l’interface graphique

    // Lancer ton serveur
    try {
        LPTF_Server server("127.0.0.1", 12345);
        //server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
