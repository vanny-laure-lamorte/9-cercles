#pragma once
#include <QMainWindow>
#include "DashboardWidget.h"
#include "InfoSystemWidget.h"
#include "ProcessManagerWidget.h"
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DashboardWidget *dashboard;
    InfoSystemWidget *infoWidget;
    ProcessManagerWidget *processManagerWidget;
    QStackedWidget *pages;
};
