#pragma once
#include <QMainWindow>
#include "DashboardWidget.h"
#include "InfoSystemWidget.h"
#include "ProcessManagerWidget.h"
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    DashboardWidget *dashboard;
    InfoSystemWidget *infoWidget;
    ProcessManagerWidget *processManagerWidget;

    QStackedWidget *pages;
};
