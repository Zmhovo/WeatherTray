#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QMainWindow.h"
#include <QContextMenuEvent>



class WeatherTray : public QMainWindow
{
    Q_OBJECT

public:
    WeatherTray(QWidget *parent = nullptr);

    ~WeatherTray();
private:
    Ui::QMainWindow ui;

    QMenu* mExitMenu;          //ÓÒ¼üÍË³ö²Ëµ¥
    QAction* mExitAct;

    QPoint mOffset;
protected:
    void contextMenuEvent(QContextMenuEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
