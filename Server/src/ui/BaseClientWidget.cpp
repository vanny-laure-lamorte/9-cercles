    #include "BaseClientWidget.h"
    #include <QPixmap>
    #include <QDebug>
    #include <QTimer>

    BaseClientWidget::BaseClientWidget(QWidget *parent,
                                    QWidget *displayInfoBox,
                                    QLabel *topTitle,
                                    QLabel *topSubtitle,
                                    QLabel *topRect)
        : QWidget(parent),
        displayInfoBox(displayInfoBox),
        topTitle(topTitle),
        topSubtitle(topSubtitle),
        topRect(topRect)
    {
    }

    void BaseClientWidget::prepareView(const QString &title,
                                    const QString &subtitle,
                                    const QString &logoPath)
    {
        if (topTitle)
            topTitle->setText(title);
        if (topSubtitle)
            topSubtitle->setText(subtitle);
        if (topRect)
        {
            QPixmap pix(logoPath);
            QPixmap scaled = pix.scaled(topRect->size(),
                                        Qt::KeepAspectRatioByExpanding,
                                        Qt::SmoothTransformation);
            topRect->setPixmap(scaled);
            topRect->setAlignment(Qt::AlignCenter);
            topRect->setScaledContents(false);
        }

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

        QVBoxLayout *mainLayout = new QVBoxLayout(displayInfoBox);
        mainLayout->setSpacing(6);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        clientBar = new QWidget(displayInfoBox);
        clientLayout = new QHBoxLayout(clientBar);
        clientLayout->setSpacing(4);
        clientLayout->setContentsMargins(0, 0, 0, 0);
        clientBar->setStyleSheet("background: none; border: none;");
        mainLayout->addWidget(clientBar);

        clientLabel = new QLabel("Waiting for clients...", displayInfoBox);
        clientLabel->setStyleSheet("color:#faa4a4; font-size:20px; font-family:Calibri; background:none; border:none;");
        mainLayout->addWidget(clientLabel);

        infoLabel = new QLabel("", displayInfoBox);
        infoLabel->setWordWrap(true);
        infoLabel->setStyleSheet("color:#faa4a4; font-size:16px; font-family:Calibri; background:none; border:none;");
        mainLayout->addWidget(infoLabel);

        mainLayout->addStretch(1);
        displayInfoBox->setLayout(mainLayout);

        for (const QString &socketId : connectedClients)
            addClient(socketId);
    }

    void BaseClientWidget::clearDynamicContent()
    {
        // par défaut : rien, spécialisation dans les sous-classes
    }

    void BaseClientWidget::addClient(const QString &socketId)
    {
        connectedClients.insert(socketId);
        if (!active || !clientLayout || clientButtons.contains(socketId))
            return;

        QPushButton *btn = new QPushButton("Client " + socketId, displayInfoBox);
        btn->setFixedHeight(30);
        btn->setStyleSheet("border-radius:5px; padding:4px 8px; color:#faa4a4; background-color:#2c0c0c; font-size:14px; font-family:Calibri;");

        clientButtons[socketId] = btn;
        clientLayout->addWidget(btn, 0, Qt::AlignLeft);

        connect(btn, &QPushButton::clicked, this, [this, socketId]()
                { onClientButtonClicked(socketId); });
    }

    void BaseClientWidget::removeClient(const QString &socketId)
    {
        connectedClients.remove(socketId);

        if (!active || !clientButtons.contains(socketId))
            return;

        QPushButton *btn = clientButtons.take(socketId);
        clientLayout->removeWidget(btn);
        btn->disconnect(this);
        btn->deleteLater();

        if (clientLabel && clientLabel->text().contains(socketId))
        {
            if (!clientButtons.isEmpty())
            {
                QString newSocketId = clientButtons.firstKey();
                clientLabel->setText("Client : " + newSocketId);
                if (infoLabel)
                    infoLabel->setText("Waiting for information...");
                emit clientSelected(newSocketId);
            }
            else
            {
                clientLabel->setText("Waiting for clients...");
                if (infoLabel)
                    infoLabel->clear();
            }
        }
    }

void BaseClientWidget::setActive(bool isActive)
{
    switching = true;
    active = isActive;

    if (!active && displayInfoBox && displayInfoBox->layout()) {
        qDebug() << "[BaseClientWidget] setActive(false) - cleaning buttons and layout";

        for (auto btn : clientButtons) {
            if (btn) {
                btn->setEnabled(false);
                btn->disconnect(this);
                QTimer::singleShot(0, btn, &QObject::deleteLater);
            }
        }
        clientButtons.clear();

        auto layout = displayInfoBox->layout();
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (auto w = item->widget())
                QTimer::singleShot(0, w, &QObject::deleteLater);
            delete item;
        }
        delete layout;

        clientBar = nullptr;
        clientLabel = nullptr;
        infoLabel = nullptr;
    }

    QTimer::singleShot(100, this, [this]() { switching = false; });
}

void BaseClientWidget::onClientButtonClicked(const QString &socketId)
{
    if (!active || switching) {
        qDebug() << "[BaseClientWidget] Ignored click during switch for client:" << socketId;
        return;
    }

    if (!clientLabel || !infoLabel) return;

    clientLabel->setText("Client : " + socketId);
    infoLabel->setText("Waiting for information...");
    infoLabel->show();

    qDebug() << "[BaseClientWidget] Client button clicked:" << socketId;
    emit clientSelected(socketId);
}

void BaseClientWidget::isSwitching(bool switching){
    
}
