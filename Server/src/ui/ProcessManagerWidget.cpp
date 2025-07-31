#include "ProcessManagerWidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QDebug>

void ProcessManagerWidget::setActive(bool isActive)
{
    qDebug() << "[ProcessManagerWidget] setActive:" << isActive;
    BaseClientWidget::setActive(isActive);

    if (active) {
        clearDynamicContent();
        prepareView("Processes List",
                    "Display client processes running on his system",
                    ":/assets/logo_6.png");

        connect(this, &BaseClientWidget::clientSelected,
                this, &ProcessManagerWidget::onClientClicked,
                Qt::UniqueConnection);

        for (const QString &socketId : connectedClients) {
            qDebug() << "[ProcessManagerWidget] Adding client:" << socketId;
            addClient(socketId);
        }

        if (!clientButtons.isEmpty()) {
            QString firstSocketId = clientButtons.firstKey();
            qDebug() << "[ProcessManagerWidget] First client auto-selected:" << firstSocketId;
            clientLabel->setText("Client : " + firstSocketId);
            infoLabel->setText("Waiting for information...");
            emit processesForClientSelected(firstSocketId);
        }
    }
}

void ProcessManagerWidget::onClientClicked(const QString &socketId)
{
    qDebug() << "[ProcessManagerWidget] onClientClicked ->" << socketId;
    emit processesForClientSelected(socketId);
}

void ProcessManagerWidget::clearDynamicContent()
{
    qDebug() << "[ProcessManagerWidget] clearDynamicContent called";
    if (processTable) {
        auto layout = qobject_cast<QVBoxLayout *>(displayInfoBox->layout());
        if (layout) layout->removeWidget(processTable);
        processTable->deleteLater();
        processTable = nullptr;
    }
}

void ProcessManagerWidget::displayProcesses(
        const std::vector<std::vector<std::string>> &processes,
        const QString &socketId)
{
    qDebug() << "[ProcessManagerWidget] displayProcesses called for socket:" << socketId
             << "active:" << active
             << "clientLabel contains:" << (clientLabel ? clientLabel->text() : "<null>");

    if (!(active && clientLabel && clientLabel->text().contains(socketId))) {
        qDebug() << "[ProcessManagerWidget] Display aborted: not active or wrong client";
        return;
    }

    if (infoLabel) {
        qDebug() << "[ProcessManagerWidget] Hiding infoLabel";
        infoLabel->hide();
    }

    if (!processTable) {
        qDebug() << "[ProcessManagerWidget] Creating new processTable";
        processTable = new QTableWidget(displayInfoBox);
        processTable->setColumnCount(7);
        processTable->setHorizontalHeaderLabels(
            {"N°", "Process", "PID", "PPID", "Threads", "Priority", "Runtime"});
        processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        processTable->verticalHeader()->setVisible(false);
        processTable->setStyleSheet("color:white; font-family:Calibri; font-size:14px;");
        processTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        auto mainLayout = qobject_cast<QVBoxLayout *>(displayInfoBox->layout());
        if (mainLayout) {
            int index = mainLayout->indexOf(clientLabel);
            if (index != -1) {
                mainLayout->insertWidget(index + 1, processTable, 7);
                qDebug() << "[ProcessManagerWidget] Inserted processTable at index" << index+1;
            } else {
                mainLayout->addWidget(processTable, 7);
                qDebug() << "[ProcessManagerWidget] Added processTable at end";
            }

            mainLayout->setStretchFactor(clientLabel, 1);
            mainLayout->setStretchFactor(processTable, 7);
        }
    }

    qDebug() << "[ProcessManagerWidget] Filling processTable with rows:" << processes.size();
    processTable->clearContents();
    processTable->setRowCount(processes.size());

    int row = 0;
    for (const auto &process : processes) {
        if (process.size() < 6) continue;

        processTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        processTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(process[0])));
        processTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(process[1])));
        processTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(process[2])));
        processTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(process[3])));
        processTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(process[4])));
        processTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(process[5])));
        row++;
    }

    qDebug() << "[ProcessManagerWidget] Finished filling table";
    processTable->resizeRowsToContents();
}
