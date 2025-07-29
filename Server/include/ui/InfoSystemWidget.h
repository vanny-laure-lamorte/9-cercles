#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include "LPTF_Server.h"

class InfoSystemWidget : public QWidget {
    Q_OBJECT
public:
    explicit InfoSystemWidget(QWidget *parent = nullptr);

public slots:
    void displaySystemInfo(const QString &info);

private:
    QTextEdit *infoView;
};
