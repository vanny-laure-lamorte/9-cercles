#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFontDatabase>
#include "LPTF_Server.h"

class ProcessManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessManagerWidget(QWidget *parent = nullptr);

public slots:
    void displayProcesses(const std::vector<std::vector<std::string>> &processes);

private:
    QTextEdit *processView;
};
