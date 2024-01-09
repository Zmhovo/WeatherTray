#pragma once

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QMainWindow>
#include <QContextMenuEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox> 
#include <QMenu>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QPainter>
#include <QPoint>

#include "ui_QMainWindow.h"
#include "WeatherData.h"

#define INCREMENT 3          //温度每升高/降低1°，y轴坐标的增量
#define POINT_RADIUS 3          //曲线描点的大小
#define TEXT_OFFSET_X 15
#define TEXT_OFFSET_Y 8


class WeatherTray : public QMainWindow
{
    Q_OBJECT

public:
    WeatherTray(QWidget *parent = nullptr);

    ~WeatherTray();

private slots:
    void on_btnSearch_clicked();
private:
    void onReplied(QNetworkReply* reply);


    Ui::QMainWindow ui;

    QMenu* mExitMenu;          //右键退出菜单
    QAction* mExitAct;

    QPoint mOffset;

    QNetworkAccessManager* mNetAccessManager;

    Today mToday;
    Day mDay[6];

    //控件对象数组
    QList<QLabel*> mWeekList;          //周几
    QList<QLabel*> mDateList;          //日期

    QList<QLabel*> mTypeList;          //天气
    QList<QLabel*> mTypeIconList;          //图标

    QList<QLabel*> mAqiList;          //污染指数

    QList<QLabel*> mFxList;          //风向
    QList<QLabel*> mFlList;          //风力

    QMap<QString, QString> mTypeMap;

protected:
    void contextMenuEvent(QContextMenuEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

//    void getWeatherInfo(QString cityCode);          //获取天气信息
    void getWeatherInfo(QString cityName);          //获取天气信息

    void parseJson(QByteArray &byteArray);          //解析天气数据

    void updateUI();          //更新UI

    bool eventFilter(QObject* watched, QEvent* event);          //重写

    void paintHighCurve();          //绘制高温曲线
    void paintLowCurve();          //绘制低温曲线
};
