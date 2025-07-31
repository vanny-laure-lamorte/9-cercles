#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include "LPTF_Server.h"

Q_DECLARE_METATYPE(std::vector<std::vector<std::string>>)

class ProcessManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessManagerWidget(QWidget *parent = nullptr, QWidget *display_info_box = nullptr, QLabel *top_title = nullptr, QLabel *top_subtitle = nullptr, QLabel *top_rect = nullptr);

    void setActive(bool isActive);
    void addClient(const QString &socketId);
    void removeClient(const QString &socketId);

public slots:
    void displayProcesses(const std::vector<std::vector<std::string>> &processes, const QString &socketId);

signals:
    // Signal emitted when a client is selected
    void processesForClientSelected(const QString &socketId);

private:
    QWidget *displayInfoBox = nullptr;
    QLabel *clientLabel = nullptr;
    QLabel *infoLabel = nullptr;
    QLabel *topTitle = nullptr;
    QLabel *topSubtitle = nullptr;
    QLabel *topRect = nullptr;
    QHBoxLayout *clientLayout = nullptr;
    QMap<QString, QPushButton *> clientButtons;
    QSet<QString> connectedClients;
    QLabel processView;
    QTableWidget *processTable = nullptr;

    // void displayProcesses(const QString &info, const QString &socketId);
    void prepareProcessesView();
    bool active = false;
};
