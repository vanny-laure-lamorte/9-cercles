#include "InfoSystemWidget.h"

InfoSystemWidget::InfoSystemWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    infoView = new QTextEdit(this);
    infoView->setReadOnly(true);
    infoView->setText("No system info received yet.");

    layout->addWidget(infoView);
    setLayout(layout);
}

void InfoSystemWidget::displaySystemInfo(const QString &info) {
    infoView->setText(info);
}

