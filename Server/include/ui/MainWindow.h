#pragma once
#include <QMainWindow>
#include "DashboardWidget.h"
#include "InfoSystemWidget.h"
#include "ProcessManagerWidget.h"
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    DashboardWidget *dashboard;
    InfoSystemWidget *infoWidget;
    ProcessManagerWidget *processManagerWidget;
    Ui::MainWindow *ui;

    QStackedWidget *pages;
};
