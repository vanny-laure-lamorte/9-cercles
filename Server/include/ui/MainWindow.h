#pragma once
#include "LPTF_Server.h"
#include "InfoSystemWidget.h"
#include "ProcessManagerWidget.h"
#include "SystemCommandWidget.h"
#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
* MainWindow
 * The main window of the LPTF Server GUI application.
 * It initializes the server, connects UI elements to server signals,
 * and manages the display of system information and process management.
*/
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    // Constructor that initializes the main window with an optional parent
    explicit MainWindow(QWidget *parent = nullptr);
    // Destructor that cleans up resources
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LPTF_Server *server;
    InfoSystemWidget *infoWidget;
    ProcessManagerWidget *processManagerWidget;
    SystemCommandWidget *systemCommandWidget;

    QThread *serverThread;
    QLabel *clientLabel;
    QPushButton *systemInfoButton;
    QPushButton *displayProcessesButton;
    QPushButton *commandSystemButton;

signals:
    // Signal to request system information from the server
    void requestSystemInfo();
    // Signal to request the list of processes from the server
    void requestProcessList();

public slots:
    // Slot to update the client count displayed in the UI
    void updateClientCount(int count);
    // Slot to handle the reception of system information
    void handleClientDisconnected(const QString &socketId);
};
