#pragma once
#include "BaseClientWidget.h"
#include <vector>
#include <map>
#include <QString>

class DatabaseWidget : public BaseClientWidget
{
    Q_OBJECT

public:
    using BaseClientWidget::BaseClientWidget;

    void setActive(bool isActive) override;

public slots:
    void displayAllHostInfo(const QVector<QMap<QString, QString>> &hosts);

private slots:

signals:
    void requestInfoSystemForAllClient();
};
