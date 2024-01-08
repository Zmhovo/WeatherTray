#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QMainWindow.h"
#include <QContextMenuEvent>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>



class WeatherTray : public QMainWindow
{
    Q_OBJECT

public:
    WeatherTray(QWidget *parent = nullptr);

    ~WeatherTray();

    void onReplied(QNetworkReply* reply);
private:
    Ui::QMainWindow ui;

    QMenu* mExitMenu;          //�Ҽ��˳��˵�
    QAction* mExitAct;

    QPoint mOffset;
protected:
    void contextMenuEvent(QContextMenuEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
