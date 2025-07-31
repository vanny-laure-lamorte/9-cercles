#pragma once
#include "BaseClientWidget.h"

class InfoSystemWidget : public BaseClientWidget
{
    Q_OBJECT

public:
    using BaseClientWidget::BaseClientWidget;
    void setActive(bool isActive) override;

public slots:
    void displaySystemInfo(const QString &info, const QString &socketId);

private slots:
    void onClientClicked(const QString &socketId);

signals:
    void infoSystemForClientSelected(const QString &socketId);
};
