#include "DashboardWidget.h"

DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    clientLabel = new QLabel("Waiting for connection...", this);
    systemInfoButton = new QPushButton("Request System Info", this);
    displayProcessesButton = new QPushButton("Request running processes", this);

    layout->addWidget(clientLabel);
    layout->addWidget(systemInfoButton);
    layout->addSpacing(50);
    layout->addWidget(displayProcessesButton);
    setLayout(layout);

    connect(systemInfoButton, &QPushButton::clicked,
            this, &DashboardWidget::requestSystemInfo);

    connect(displayProcessesButton, &QPushButton::clicked,
            this, &DashboardWidget::requestProcessList);
}

void DashboardWidget::updateClientCount(int count) {
    if (count == 0)
        clientLabel->setText("Waiting for connection...");
    else
        clientLabel->setText(QString("Clients connected: %1").arg(count));
}
