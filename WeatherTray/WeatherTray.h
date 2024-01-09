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

private slots:
    void on_btnSearch_clicked();
private:
    void onReplied(QNetworkReply* reply);


    Ui::QMainWindow ui;

    QMenu* mExitMenu;          //�Ҽ��˳��˵�
    QAction* mExitAct;

    QPoint mOffset;

    QNetworkAccessManager* mNetAccessManager;

    Today mToday;
    Day mDay[6];

    //�ؼ���������
    QList<QLabel*> mWeekList;          //�ܼ�
    QList<QLabel*> mDateList;          //����

    QList<QLabel*> mTypeList;          //����
    QList<QLabel*> mTypeIconList;          //ͼ��

    QList<QLabel*> mAqiList;          //��Ⱦָ��

    QList<QLabel*> mFxList;          //����
    QList<QLabel*> mFlList;          //����

    QMap<QString, QString> mTypeMap;
protected:
    void contextMenuEvent(QContextMenuEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

//    void getWeatherInfo(QString cityCode);          //��ȡ������Ϣ
    void getWeatherInfo(QString cityName);          //��ȡ������Ϣ

    void parseJson(QByteArray &byteArray);          //������������

    void updateUI();          //����UI


};
