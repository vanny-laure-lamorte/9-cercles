#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QSet>
#include <QHBoxLayout>
#include <QVBoxLayout>

class BaseClientWidget : public QWidget
{
    Q_OBJECT

public:
    BaseClientWidget(QWidget *parent,
                     QWidget *displayInfoBox,
                     QLabel *topTitle,
                     QLabel *topSubtitle,
                     QLabel *topRect);

    virtual void setActive(bool isActive);
    void isSwitching(bool switching);

    virtual void clearDynamicContent();

    void addClient(const QString &socketId);
    void removeClient(const QString &socketId);

signals:
    void clientSelected(const QString &socketId);

protected:
    QWidget *displayInfoBox = nullptr;
    QLabel *topTitle = nullptr;
    QLabel *topSubtitle = nullptr;
    QLabel *topRect = nullptr;

    QWidget *clientBar = nullptr;
    QLabel *clientLabel = nullptr;
    QLabel *infoLabel = nullptr;
    QHBoxLayout *clientLayout = nullptr;

    bool active = false;
    bool switching = false;
    QSet<QString> connectedClients;
    QMap<QString, QPushButton *> clientButtons;

    void prepareView(const QString &title,
                     const QString &subtitle,
                     const QString &logoPath);

    void onClientButtonClicked(const QString &socketId);
};
