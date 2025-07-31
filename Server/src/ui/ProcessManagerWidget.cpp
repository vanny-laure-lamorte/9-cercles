#include "ProcessManagerWidget.h"

ProcessManagerWidget::ProcessManagerWidget(QWidget *parent, QWidget *display_info_box, QLabel *top_title, QLabel *top_subtitle, QLabel *top_rect)
    : QWidget(parent),
      displayInfoBox(display_info_box),
      infoLabel(top_subtitle),
      topTitle(top_title),
      topSubtitle(top_subtitle),
      topRect(top_rect)
{
    if (!displayInfoBox)
    {
        qWarning() << "Display info box is null.";
        return;
    }
}

void ProcessManagerWidget::setActive(bool isActive)
{
    active = isActive;
    if (active)
    {
        prepareProcessesView();

        for (const QString &socketId : connectedClients)
        {
            addClient(socketId);
        }

        if (!clientButtons.isEmpty())
        {
            QString firstSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + firstSocketId);
            if (!infoLabel)
            {
                infoLabel->setVisible(true);
            }
            infoLabel->setText("Waiting for information...");
            emit processesForClientSelected(firstSocketId);
        }
    }
    else
    {
        if (auto oldLayout = displayInfoBox->layout())
        {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr)
            {
                if (auto w = item->widget())
                    w->deleteLater();
                delete item;
            }
            delete oldLayout;
        }
    }
}

void ProcessManagerWidget::prepareProcessesView()
{
    if (auto oldLayout = displayInfoBox->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (auto w = item->widget())
                w->deleteLater();
            delete item;
        }
        delete oldLayout;
    }

    clientLabel = nullptr;
    infoLabel   = nullptr;
    processTable = nullptr;

    QVBoxLayout *mainLayout = new QVBoxLayout(displayInfoBox);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QWidget *clientBar = new QWidget(displayInfoBox);
    clientLayout = new QHBoxLayout(clientBar);
    clientLayout->setSpacing(4);
    clientLayout->setContentsMargins(0, 0, 0, 0);
    clientBar->setStyleSheet("background: none; border: none;");
    mainLayout->addWidget(clientBar);

    // Label du client
    clientLabel = new QLabel("Waiting for clients...", displayInfoBox);
    clientLabel->setStyleSheet(
        "color: #faa4a4;"
        "font-size:20px;"
        "font-family: Calibri;"
        "background: none;"
        "border: none;");
    mainLayout->addWidget(clientLabel);

    // Label d'info
    infoLabel = new QLabel("", displayInfoBox);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "color: #faa4a4;"
        "font-size:16px;"
        "font-family: Calibri;"
        "background: none;"
        "border: none;");
    mainLayout->addWidget(infoLabel);

    mainLayout->addStretch(1);

    displayInfoBox->setLayout(mainLayout);
}

void ProcessManagerWidget::addClient(const QString &socketId)
{
    connectedClients.insert(socketId);
    if (!active || !clientLayout || clientButtons.contains(socketId))
        return;

    QPushButton *btn = new QPushButton("Client " + socketId, displayInfoBox);
    btn->setFixedHeight(30);
    btn->setStyleSheet(
        "border-radius: 5px;"
        "padding: 4px 8px;"
        "color: #faa4a4;"
        "background-color: #2c0c0c;"
        "font-size: 14px;"
        "font-family: Calibri;");

    clientButtons[socketId] = btn;
    clientLayout->addWidget(btn, 0, Qt::AlignLeft);

    connect(btn, &QPushButton::clicked, this, [this, socketId]()
            {
        clientLabel->setText("Client : " + socketId);
        infoLabel->setText("Waiting for information...");
        emit processesForClientSelected(socketId); });
}

void ProcessManagerWidget::removeClient(const QString &socketId)
{
    connectedClients.remove(socketId);

    if (!active || !clientButtons.contains(socketId))
        return;

    QPushButton *btn = clientButtons.take(socketId);
    clientLayout->removeWidget(btn);
    btn->deleteLater();

    if (clientLabel->text().contains(socketId))
    {
        if (!clientButtons.isEmpty())
        {
            QString newSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + newSocketId);
            infoLabel->setText("Waiting for information...");
            emit processesForClientSelected(newSocketId);
        }
        else
        {
            clientLabel->setText("Waiting for clients...");
            infoLabel->clear();
        }
    }
}

void ProcessManagerWidget::displayProcesses(const std::vector<std::vector<std::string>> &processes,
                                            const QString &socketId)
{
    if (!(active && clientLabel && clientLabel->text().contains(socketId)))
        return;

    if (infoLabel)
        infoLabel->hide();

    if (!processTable) {
        processTable = new QTableWidget(displayInfoBox);
        processTable->setColumnCount(7);
        processTable->setHorizontalHeaderLabels(
            {"N°", "Process", "PID", "PPID", "Threads", "Priority", "Runtime"});
        processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        processTable->verticalHeader()->setVisible(false);
        processTable->setStyleSheet("color: white; font-family: Calibri; font-size: 14px;");
        processTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        auto mainLayout = qobject_cast<QVBoxLayout *>(displayInfoBox->layout());
        if (mainLayout) {
            int index = mainLayout->indexOf(clientLabel);
            if (index != -1)
                mainLayout->insertWidget(index + 1, processTable, 7);
            else
                mainLayout->addWidget(processTable, 7);

            mainLayout->setStretchFactor(clientLabel, 1);
            mainLayout->setStretchFactor(processTable, 7);
        }
    }

    processTable->clearContents();
    processTable->setRowCount(processes.size());

    int row = 0;
    for (const auto &process : processes) {
        if (process.size() < 6) continue;
        processTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
        processTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(process[0])));
        processTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(process[1])));
        processTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(process[2])));
        processTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(process[3])));
        processTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(process[4])));
        processTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(process[5])));
        row++;
    }
}