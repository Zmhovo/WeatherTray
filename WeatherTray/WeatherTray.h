#pragma once

#include <QtWidgets/QMainWindow>
#include <QContextMenuEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox> 
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "ui_QMainWindow.h"
#include "WeatherData.h"



class WeatherTray : public QMainWindow
{
    Q_OBJECT

public:
    WeatherTray(QWidget *parent = nullptr);

    ~WeatherTray();


private:
    void onReplied(QNetworkReply* reply);


    Ui::QMainWindow ui;

    QMenu* mExitMenu;          //右键退出菜单
    QAction* mExitAct;

    QPoint mOffset;

    QNetworkAccessManager* mNetAccessManager;

    Today mToday;
    Day mDay[6];

protected:
    void contextMenuEvent(QContextMenuEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    void getWeatherInfo(QString cityCode);          //获取天气信息

    void parseJson(QByteArray &byteArray);          //解析天气数据
};
