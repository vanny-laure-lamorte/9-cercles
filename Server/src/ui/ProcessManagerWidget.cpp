#include "ProcessManagerWidget.h"

ProcessManagerWidget::ProcessManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    processView = new QTextEdit(this);
    processView->setReadOnly(true);
    processView->setText("No processes received yet.");

    layout->addWidget(processView);
    setLayout(layout);
}

void ProcessManagerWidget::displayProcesses(const std::vector<std::vector<std::string>> &processes)
{
    processView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    QString text = "[Client] : List of processes received:\n\n";

    for (const auto &process : processes)
    {
        if (process.size() < 6)
            continue;

        text += QString("Process: %1 PID: %2 PPID: %3 Threads: %4 Priority: %5 Runtime: %6\n")
                    .arg(QString::fromStdString(process[0]).leftJustified(20, ' '))
                    .arg(QString::fromStdString(process[1]).leftJustified(6, ' '))
                    .arg(QString::fromStdString(process[2]).leftJustified(6, ' '))
                    .arg(QString::fromStdString(process[3]).leftJustified(4, ' '))
                    .arg(QString::fromStdString(process[4]).leftJustified(3, ' '))
                    .arg(QString::fromStdString(process[5]));
    }

    processView->setPlainText(text);
}
