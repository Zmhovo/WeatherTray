#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_WeatherTray.h"

class WeatherTray : public QMainWindow
{
    Q_OBJECT

public:
    WeatherTray(QWidget *parent = nullptr);
    ~WeatherTray();
private:
    Ui::WeatherTrayClass ui;

    QMenu* mExitMenu;          //�Ҽ��˳��˵�
    QAction* mExitAct;
protected:
    void contextMenuEvent(QContextMenuEvent* event);
};
