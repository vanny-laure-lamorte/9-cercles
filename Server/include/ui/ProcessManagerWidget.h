#pragma once
#include "BaseClientWidget.h"
#include <QTableWidget>

class ProcessManagerWidget : public BaseClientWidget
{
    Q_OBJECT
public:
    using BaseClientWidget::BaseClientWidget;
    void setActive(bool isActive) override;
    void clearDynamicContent() override;

public slots:
    void displayProcesses(const std::vector<std::vector<std::string>> &processes,
                          const QString &socketId);

private slots:
    void onClientClicked(const QString &socketId);

signals:
    void processesForClientSelected(const QString &socketId);

private:
    QTableWidget *processTable = nullptr;
};
