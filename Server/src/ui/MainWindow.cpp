#include "MainWindow.h"
#include "DashboardWidget.h"
#include "InfoSystemWidget.h"
#include "ProcessManagerWidget.h"
#include "ui_MainWindow.h"
#include "LPTF_Server.h"
#include <QHBoxLayout>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
// {
//         QWidget *central = new QWidget(this);
//         setCentralWidget(central);

//         QHBoxLayout *layout = new QHBoxLayout(central);

//         dashboard = new DashboardWidget(this);
//         pages = new QStackedWidget(this);

//         infoWidget = new InfoSystemWidget(this);
//         processManagerWidget = new ProcessManagerWidget(this);

//         pages->addWidget(infoWidget);
//         pages->addWidget(processManagerWidget);

//         layout->addWidget(dashboard, 1);
//         layout->addWidget(pages, 3);

//         QThread *serverThread = new QThread(this);
//         LPTF_Server *server = new LPTF_Server("127.0.0.1", 12345);
//         server->moveToThread(serverThread);

//         connect(serverThread, &QThread::started, [=]()
//                 { server->run(); });

//         connect(server, &LPTF_Server::clientCountChanged,
//                 dashboard, &DashboardWidget::updateClientCount);

//         connect(dashboard, &DashboardWidget::requestSystemInfo, [=]()
//                 {
//                 pages->setCurrentWidget(infoWidget);
//                 server->sendSystemInfoRequest(); });

//         connect(server, &LPTF_Server::systemInfoReceived,
//                 infoWidget, &InfoSystemWidget::displaySystemInfo);

//         connect(dashboard, &DashboardWidget::requestProcessList, [=]()
//                 {
//                 pages->setCurrentWidget(processManagerWidget);
//                 server->sendProcessListRequest(); });

//         connect(server, &LPTF_Server::processListReceived,
//                 processManagerWidget, &ProcessManagerWidget::displayProcesses);

//         serverThread->start();
// }
