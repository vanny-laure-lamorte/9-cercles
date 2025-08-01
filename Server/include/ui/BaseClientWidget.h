#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>
#include <QSet>

class BaseClientWidget : public QWidget
{
    Q_OBJECT
public:
    BaseClientWidget(QWidget *parent,
                     QWidget *displayInfoBox,
                     QLabel *topTitle,
                     QLabel *topSubtitle,
                     QLabel *topRect,
                     QLabel * displayDetails,
                     QLabel *displayInfoTitle);

    virtual void setActive(bool isActive);
    virtual void clearDynamicContent();

protected:
    void prepareView(const QString &title,
                     const QString &subtitle,
                     const QString &logoPath);

    virtual void onClientButtonClicked(const QString &socketId);

signals:
    void clientSelected(const QString &socketId);

public slots: // 👈 Ajouté ici
    void addClient(const QString &socketId);
    void removeClient(const QString &socketId);

protected:
    QWidget *displayInfoBox = nullptr;
    QLabel *topTitle = nullptr;
    QLabel *topSubtitle = nullptr;
    QLabel *topRect = nullptr;
    QLabel * displayDetails = nullptr;
    QLabel *displayInfoTitle = nullptr;

    QWidget *clientBar = nullptr;
    QHBoxLayout *clientLayout = nullptr;
    QLabel *clientLabel = nullptr;
    QLabel *infoLabel = nullptr;

    QMap<QString, QPushButton*> clientButtons;
    QSet<QString> connectedClients;

    bool active = false;
    bool switching = false;
    bool initialized = false;

};
