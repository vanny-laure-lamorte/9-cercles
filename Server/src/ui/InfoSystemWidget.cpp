#include "InfoSystemWidget.h"
#include <QDebug>

InfoSystemWidget::InfoSystemWidget(QWidget *parent, QWidget *display_info_box, QLabel *top_title, QLabel *top_subtitle, QLabel *top_rect)
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

void InfoSystemWidget::setActive(bool isActive)
{
    active = isActive;
    if (active)
    {
        prepareSystemInfoView();

        for (const QString &socketId : connectedClients)
        {
            addClient(socketId);
        }

        if (!clientButtons.isEmpty())
        {
            QString firstSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + firstSocketId);
            infoLabel->setText("Waiting for information...");
            emit infoSystemForClientSelected(firstSocketId);
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

void InfoSystemWidget::prepareSystemInfoView()
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
    for (auto child : displayInfoBox->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly))
    {
        child->deleteLater();
    }

    topTitle->setText("System Information");
    topSubtitle->setText("Display client system information (Hostname, OS, ...)");
    QPixmap pix(":/assets/logo_9.png");
    QPixmap scaled = pix.scaled(topRect->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    topRect->setPixmap(scaled);
    topRect->setAlignment(Qt::AlignCenter);
    topRect->setScaledContents(false);
    QVBoxLayout *mainLayout = new QVBoxLayout(displayInfoBox);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QWidget *clientBar = new QWidget(displayInfoBox);
    clientLayout = new QHBoxLayout(clientBar);
    clientLayout->setSpacing(4);
    clientLayout->setContentsMargins(0, 0, 0, 0);
    clientBar->setStyleSheet(
        "background: none;"
        "border: none;");
    clientLabel = new QLabel("", displayInfoBox);
    clientLabel->setStyleSheet(
        "color: #faa4a4;"
        "font-size:20px;"
        "font-family: Calibri;"
        "background: none;"
        "border: none;");
    infoLabel = new QLabel("", displayInfoBox);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "color: #faa4a4;"
        "font-size:16px;"
        "font-family: Calibri;"
        "background: none;"
        "border: none;");
    mainLayout->addWidget(clientBar);
    mainLayout->addWidget(clientLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addStretch(1);
    if (connectedClients.empty())
    {
        clientLabel->setText("Waiting for clients...");
    }

    displayInfoBox->setLayout(mainLayout);
}

void InfoSystemWidget::addClient(const QString &socketId)
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
        emit infoSystemForClientSelected(socketId); });
}

void InfoSystemWidget::removeClient(const QString &socketId)
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
            emit infoSystemForClientSelected(newSocketId);
        }
        else
        {
            clientLabel->setText("Waiting for clients...");
            infoLabel->clear();
        }
    }
}

void InfoSystemWidget::displaySystemInfo(const QString &info, const QString &socketId)
{
    qDebug() << "Displaying system info for socket ID:" << socketId;
    if (active && clientLabel->text().contains(socketId))
    {
        infoLabel->setText(info);
    }
}
