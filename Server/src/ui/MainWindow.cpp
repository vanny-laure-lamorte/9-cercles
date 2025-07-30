#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new LPTF_Server("127.0.0.1", 12345);
    infoWidget = new InfoSystemWidget(this, ui->display_info_box, ui->top_title, ui->top_subtitle, ui->top_rect);

    clientLabel = ui->user_text_number;
    systemInfoButton = ui->btn_1;
    displayProcessesButton = ui->btn_2;

    connect(this, &MainWindow::requestSystemInfo,
            server, &LPTF_Server::sendSystemInfoRequest);

    connect(this, &MainWindow::requestProcessList,
            server, &LPTF_Server::sendProcessListRequest);

    connect(server, &LPTF_Server::clientCountChanged,
            this, &MainWindow::updateClientCount);

    connect(server, &LPTF_Server::systemInfoReceived,
            infoWidget, &InfoSystemWidget::displaySystemInfo);

    connect(infoWidget, &InfoSystemWidget::clientSelected,
            server, &LPTF_Server::sendSystemInfoRequestFor);

    connect(server, &LPTF_Server::clientConnected,
            infoWidget, &InfoSystemWidget::addClient);

    connect(server, &LPTF_Server::clientDisconnected,
            infoWidget, &InfoSystemWidget::removeClient);

    connect(systemInfoButton, &QPushButton::clicked, this, [this] {
        qDebug() << "System Info activated!";
        infoWidget->setActive(true);
    });

    connect(displayProcessesButton, &QPushButton::clicked, this, [this] {
        qDebug() << "Button processList clicked!";
        emit requestProcessList();
    });

    serverThread = QThread::create([this] { server->run(); });
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
