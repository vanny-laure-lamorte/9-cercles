#include "ProcessManagerWidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QDebug>

void ProcessManagerWidget::setActive(bool isActive)
{
    if (active == isActive && isActive) return;
    BaseClientWidget::setActive(isActive);

    if (active) {
        clearDynamicContent();
        prepareView("Processes List",
                    "Display client processes",
                    ":/assets/logo_6.png");

        connect(this, &BaseClientWidget::clientSelected,
                this, &ProcessManagerWidget::onClientClicked,
                Qt::UniqueConnection);

        for (const QString &socketId : connectedClients)
            addClient(socketId);

        if (!clientButtons.isEmpty()) {
            QString firstSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + firstSocketId);
            infoLabel->setText("Waiting for information...");
            emit processesForClientSelected(firstSocketId);
        }
    }
}

void ProcessManagerWidget::onClientClicked(const QString &socketId)
{
    emit processesForClientSelected(socketId);
}

void ProcessManagerWidget::clearDynamicContent()
{
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
    if (!(active && clientLabel && clientLabel->text().contains(socketId)))
        return;

    if (infoLabel) infoLabel->hide();

    if (!processTable) {
        processTable = new QTableWidget(displayInfoBox);
        processTable->setColumnCount(6);
        processTable->setHorizontalHeaderLabels(
            {"Process", "PID", "PPID", "Threads", "Priority", "Runtime"});
        processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        processTable->verticalHeader()->setDefaultSectionSize(24);

        auto mainLayout = qobject_cast<QVBoxLayout *>(displayInfoBox->layout());
        if (mainLayout) {
            int index = mainLayout->indexOf(clientLabel);
            mainLayout->insertWidget(index + 1, processTable, 8);
        }
    }

    processTable->clearContents();
    processTable->setRowCount(processes.size());

    int row = 0;
    for (const auto &process : processes) {
        if (process.size() < 6) continue;
        for (int col = 0; col < 6; ++col)
            processTable->setItem(row, col, new QTableWidgetItem(QString::fromStdString(process[col])));
        row++;
    }

    processTable->resizeRowsToContents();
}
