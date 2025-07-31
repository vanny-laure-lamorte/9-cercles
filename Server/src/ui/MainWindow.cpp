#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new LPTF_Server("127.0.0.1", 12345);

    // Sytem info
    infoWidget = new InfoSystemWidget(this, ui->display_info_box, ui->top_title, ui->top_subtitle, ui->top_rect);

    // Process
    processManagerWidget = new ProcessManagerWidget(this, ui->display_info_box, ui->top_title, ui->top_subtitle, ui->top_rect);

    clientLabel = ui->user_text_number;
    systemInfoButton = ui->btn_1;
    displayProcessesButton = ui->btn_2;

    // Common
    connect(server, &LPTF_Server::clientCountChanged,
            this, &MainWindow::updateClientCount);

    // // System Info
    // connect(this, &MainWindow::requestSystemInfo,
    //         server, &LPTF_Server::sendSystemInfoRequest);

    connect(systemInfoButton, &QPushButton::clicked, this, [this]()
            {
    processManagerWidget->setActive(false);
    infoWidget->setActive(true); });

    connect(infoWidget, &InfoSystemWidget::infoSystemForClientSelected,
            server, &LPTF_Server::sendSystemInfoRequestFor);

    connect(server, &LPTF_Server::systemInfoReceived,
            infoWidget, &InfoSystemWidget::displaySystemInfo);

    connect(server, &LPTF_Server::clientConnected,
            infoWidget, &InfoSystemWidget::addClient);

    connect(server, &LPTF_Server::clientDisconnected,
            infoWidget, &InfoSystemWidget::removeClient);

    // Processes List
    connect(displayProcessesButton, &QPushButton::clicked, this, [this]()
            {
    infoWidget->setActive(false);
    processManagerWidget->setActive(true); });

    connect(server, &LPTF_Server::clientConnected,
            processManagerWidget, &ProcessManagerWidget::addClient);

    connect(server, &LPTF_Server::clientDisconnected,
            processManagerWidget, &ProcessManagerWidget::removeClient);

    connect(processManagerWidget, &ProcessManagerWidget::processesForClientSelected,
            server, &LPTF_Server::sendProcessListRequestFor);

    connect(server, &LPTF_Server::processListReceived,
            processManagerWidget, &ProcessManagerWidget::displayProcesses);

    serverThread = QThread::create([this]
                                   { server->run(); });
    serverThread->start();
}

MainWindow::~MainWindow()
{
    serverThread->quit();
    serverThread->wait();
    delete server;
    delete ui;
}

void MainWindow::updateClientCount(int count)
{
    qDebug() << "Updating client count to:" << count;
    clientLabel->setText(QString::number(count));
}

void MainWindow::handleClientDisconnected(const QString &socketId)
{
    qDebug() << "[MAINWINDOW] Client disconnected:" << socketId;
    infoWidget->removeClient(socketId);
}
