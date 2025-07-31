#include "DatabaseWidget.h"
#include <QDebug>

using namespace std;

void DatabaseWidget::setActive(bool isActive)
{
    emit requestInfoSystemForAllClient();
    if (active == isActive && isActive)
    {
        qDebug() << "[DatabaseWidget] Already active, skipping reload";
        return;
    }
    BaseClientWidget::setActive(isActive);
    if (active)
    {
        prepareView("System Information",
                    "Display client system information (Hostname, OS, ...)",
                    ":/assets/logo_9.png");
    }
}

void DatabaseWidget::displayAllHostInfo(const QVector<QMap<QString, QString>> &hosts)
{
    if (active && !hosts.isEmpty())
    {
        QString info;
        for (const QMap<QString, QString> &host : hosts)
        {
            info += "Host ID: " + host.value("id") +
                    ", Hostname: " + host.value("hostname") +
                    ", OS: " + host.value("os") + "\n";
        }
        infoLabel->setText(info);
    }
}
