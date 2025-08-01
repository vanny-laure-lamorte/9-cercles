#include "SystemCommandWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QDebug>

void SystemCommandWidget::setActive(bool isActive)
{
    qDebug() << "[Sys Debug] setActive called, isActive =" << isActive << "current active =" << active;

    if (active == isActive && isActive)
    {
        qDebug() << "[Sys Debug] Already active, skip reload";
        return;
    }

    BaseClientWidget::setActive(isActive);

    if (active)
    {
        qDebug() << "[Sys Debug] Widget becoming active";
        clearDynamicContent();

        prepareView("System commands",
                    "Launch powershell command on client system",
                    ":/assets/logo-2.png");

        connect(this, &BaseClientWidget::clientSelected,
                this, &SystemCommandWidget::onClientClicked,
                Qt::UniqueConnection);

        // Split horizontal
        splitContainer = new QWidget(displayInfoBox);
        QHBoxLayout *splitLayout = new QHBoxLayout(splitContainer);
        splitLayout->setContentsMargins(0, 0, 0, 0);
        splitLayout->setSpacing(10);
        qDebug() << "[Sys Debug] Created splitContainer";

        // Panneau gauche
        QWidget *leftPanel = new QWidget(splitContainer);
        QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

        struct CommandDef
        {
            QString name;
            bool requiresInput;
            QString prompt;
        };
        QList<CommandDef> commands = {
            {"list_desktop_files", false, ""},
            {"create_test_file", false, ""},
            {"open_file", true, "Enter filename:"},
            {"open_url", true, "Enter URL:"}};

        for (const auto &cmd : commands)
        {
            QPushButton *btn = new QPushButton(cmd.name, leftPanel);
            btn->setStyleSheet("padding:6px; font-size:14px; color:white; background:#2c0c0c; border-radius:4px;");
            leftLayout->addWidget(btn);
            qDebug() << "[Sys Debug] Added command button:" << cmd.name;

            connect(btn, &QPushButton::clicked, this, [this, cmd]()
                    {
                if (currentSocketId.isEmpty()) {
                    qDebug() << "[Sys Debug] No client selected, command ignored:" << cmd.name;
                    return;
                }

                QString payload = cmd.name;
                if (cmd.requiresInput) {
                    bool ok;
                    QString input = QInputDialog::getText(this, cmd.name, cmd.prompt,
                                                          QLineEdit::Normal, "", &ok);
                    if (!ok || input.isEmpty()) {
                        qDebug() << "[Sys Debug] Input cancelled for command:" << cmd.name;
                        return;
                    }
                    payload += " " + input;
                }

                qDebug() << "[Sys Debug] Emitting commandForClientSelected with payload:" << payload
                         << "for socket:" << currentSocketId;
                emit commandForClientSelected(payload, currentSocketId); });
        }
        leftLayout->addStretch(1);

        // Panneau droit
        QWidget *rightPanel = new QWidget(splitContainer);
        QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

        resultLabel = new QLabel("Waiting for command result...", rightPanel);
        resultLabel->setWordWrap(true);
        resultLabel->setStyleSheet("color:#faa4a4; font-size:14px; font-family:Calibri;");
        rightLayout->addWidget(resultLabel);
        qDebug() << "[Sys Debug] Result label created";

        splitLayout->addWidget(leftPanel, 1);
        splitLayout->addWidget(rightPanel, 2);

        if (auto mainLayout = qobject_cast<QVBoxLayout *>(displayInfoBox->layout()))
        {
            int index = (infoLabel ? mainLayout->indexOf(infoLabel) : -1);
            qDebug() << "[Sys Debug] mainLayout found, index of infoLabel =" << index;

            if (index >= 0)
            {
                mainLayout->insertWidget(index + 1, splitContainer, 8);
                qDebug() << "[Sys Debug] Inserted splitContainer after infoLabel";
            }
            else
            {
                mainLayout->addWidget(splitContainer, 8);
                qDebug() << "[Sys Debug] Added splitContainer at end of layout";
            }
        }

        // Clients
        qDebug() << "[Sys Debug] Adding clients, count =" << connectedClients.size();
        for (const QString &socketId : connectedClients)
            addClient(socketId);

        if (!clientButtons.isEmpty())
        {
            QString firstSocketId = clientButtons.firstKey();
            clientLabel->setText("Client : " + firstSocketId);
            currentSocketId = firstSocketId;
            qDebug() << "[Sys Debug] First client selected:" << currentSocketId;
            emit commandForClientSelected("list_desktop_files", currentSocketId);
        }
        else
        {
            qDebug() << "[Sys Debug] No clients available to select";
        }
    }
    else
    {
        qDebug() << "[Sys Debug] Widget deactivated";
    }
}

void SystemCommandWidget::clearDynamicContent()
{
    qDebug() << "[Sys Debug] clearDynamicContent called";
    splitContainer = nullptr;
    resultLabel = nullptr;
}

void SystemCommandWidget::onClientClicked(const QString &socketId)
{
    qDebug() << "[Sys Debug] onClientClicked ->" << socketId;
    currentSocketId = socketId;
    if (resultLabel)
        resultLabel->setText("Waiting for command result...");
}

void SystemCommandWidget::displayCommand(const QString &info, const QString &socketId)
{
    qDebug() << "[Sys Debug] displayCommand called for socket:" << socketId
             << "| active =" << active
             << "| label contains socket =" << (clientLabel && clientLabel->text().contains(socketId));

    if (active && resultLabel)
        resultLabel->setText(info);
}
