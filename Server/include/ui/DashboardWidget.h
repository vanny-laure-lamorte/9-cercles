#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class DashboardWidget : public QWidget {
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget *parent = nullptr);

signals:
    void requestSystemInfo();
    void requestProcessList();

public slots:
    void updateClientCount(int count);

private:
    QLabel *clientLabel;
    QPushButton *systemInfoButton;
    QPushButton *displayProcessesButton;
};
