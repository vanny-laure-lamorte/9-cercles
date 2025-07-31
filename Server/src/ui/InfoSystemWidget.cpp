#include "InfoSystemWidget.h"
#include <QDebug>

void InfoSystemWidget::setActive(bool isActive)
{
    BaseClientWidget::setActive(isActive);
    if (active) {
        prepareView("System Information",
                    "Display client system information (Hostname, OS, ...)",
                    ":/assets/logo_9.png");

        connect(this, &BaseClientWidget::clientSelected,
                this, &InfoSystemWidget::onClientClicked,
                Qt::UniqueConnection);

        for (const QString &socketId : connectedClients)
            addClient(socketId);

        if (!clientButtons.isEmpty()) {
            QString firstSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + firstSocketId);
            infoLabel->setText("Waiting for information...");
            emit infoSystemForClientSelected(firstSocketId);
        }
    }
}

void InfoSystemWidget::onClientClicked(const QString &socketId)
{
    qDebug() << "[InfoSystemWidget] onClientClicked ->" << socketId;
    emit infoSystemForClientSelected(socketId);
}

void InfoSystemWidget::displaySystemInfo(const QString &info, const QString &socketId)
{
    if (active && clientLabel && clientLabel->text().contains(socketId)) {
        infoLabel->setText(info);
    }
}
