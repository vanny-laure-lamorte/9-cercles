#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QSet>

/**
 *  The InfoSystemWidget class provides a user interface for displaying system information
 */
class InfoSystemWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructor that initializes the widget with an optional parent and a display info box
    explicit InfoSystemWidget(QWidget *parent = nullptr, QWidget *display_info_box = nullptr, QLabel *top_title = nullptr, QLabel *top_subtitle = nullptr, QLabel *top_rect = nullptr);
    // Sets the active state of the widget, preparing the system info view and managing client buttons
    void setActive(bool isActive);

public slots:
    // Adds a client button for the given socket ID
    void addClient(const QString &socketId);
    // Removes the client button for the given socket ID
    void removeClient(const QString &socketId);
    // Displays system information for the given socket ID
    void displaySystemInfo(const QString &info, const QString &socketId);

signals:
    // Signal emitted when a client is selected
    void clientSelected(const QString &socketId);

private:
    // Prepares the system info view by clearing the display info box and setting up the layout
    void prepareSystemInfoView();

    QWidget *displayInfoBox = nullptr;
    QLabel *clientLabel = nullptr;
    QLabel *infoLabel = nullptr;
    QLabel *topTitle = nullptr;
    QLabel *topSubtitle = nullptr;
    QLabel *topRect = nullptr;
    QHBoxLayout *clientLayout = nullptr;
    QMap<QString, QPushButton *> clientButtons;
    QSet<QString> connectedClients;
    bool active = false;
};
