#pragma once
#include "BaseClientWidget.h"
#include <QLabel>
#include <QVBoxLayout>

class SystemCommandWidget : public BaseClientWidget
{
    Q_OBJECT
public:
    using BaseClientWidget::BaseClientWidget;
    void setActive(bool isActive) override;
    void clearDynamicContent() override;

public slots:
    void displayCommand(const QString &info, const QString &socketId);

private slots:
    void onClientClicked(const QString &socketId);

signals:
    void commandForClientSelected(const QString &command, const QString &socketId);

private:
    QWidget *splitContainer = nullptr;
    QLabel *resultLabel = nullptr;
    QString currentSocketId;
};
