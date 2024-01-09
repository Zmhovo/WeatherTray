#include "WeatherTray.h"

WeatherTray::WeatherTray(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);          //设置窗口无边框
    setFixedSize(width(), height());

    mExitMenu = new QMenu(this);
    mExitAct = new QAction();

    mExitAct->setText("退出Exit");
    mExitAct->setIcon(QIcon(":/res/close.png"));

    mExitMenu->addAction(mExitAct);

    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
        });

    mNetAccessManager = new QNetworkAccessManager(this);

    connect(mNetAccessManager, &QNetworkAccessManager::finished, this, &WeatherTray::onReplied);

    getWeatherInfo("101010100");
}

WeatherTray::~WeatherTray()
{
}

void WeatherTray::onReplied(QNetworkReply* reply)
{
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != QNetworkReply::NoError || status_code != 200)
    {
        QMessageBox::warning(this, "天气", "请求数据失败", QMessageBox::Ok);
    }
    else
    {
        QByteArray byteArray = reply->readAll();
        parseJson(byteArray);
    }

    reply->deleteLater();
}

void WeatherTray::contextMenuEvent(QContextMenuEvent* event)          //弹出右键菜单
{
    mExitMenu->exec(QCursor::pos());

    event->accept();
}

void WeatherTray::mousePressEvent(QMouseEvent* event)
{
    mOffset = event->globalPos() - this->pos();
}

void WeatherTray::mouseMoveEvent(QMouseEvent* event)
{
    this->move(event->globalPos() - mOffset);
}

void WeatherTray::getWeatherInfo(QString cityCode)
{
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}

void WeatherTray::parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);

    if (err.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject rootObj = doc.object();

    //解析数据
    mToday.date = rootObj.value("date").toString();
    mToday.city = rootObj.value("cityInfo").toObject().value("city").toString();

    QJsonObject objData = rootObj.value("data").toObject();

    QJsonObject objYesterday = rootObj.value("yesterday").toObject();

    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();

    mDay[0].type = objYesterday.value("type").toString();

    QString s;
    s = objYesterday.value("high").toString().split(" ").at(1);
    s.left(s.length() - 1);
    mDay[0].high = s.toInt();

    s = objYesterday.value("low").toString().split(" ").at(1);
    s.left(s.length() - 1);
    mDay[0].low = s.toInt();

    mDay[0].fx = objYesterday.value("fx").toString();
    mDay[0].fl = objYesterday.value("fl").toString();

    mDay[0].aqi = objYesterday.value("aqi").toDouble();


    QJsonArray forecastArr = objData.value("forcast").toArray();

    for (int i = 0; i < 5; i++)
    {
        QJsonObject objForecast = forecastArr[i].toObject();
        mDay[i + 1].week = objForecast.value("week").toString();
        mDay[i + 1].date = objForecast.value("ymd").toString();

        mDay[i + 1].type = objForecast.value("type").toString();

        QString s;
        s = objForecast.value("high").toString().split(" ").at(1);
        s.left(s.length() - 1);
        mDay[i + 1].high = s.toInt();

        s = objForecast.value("low").toString().split(" ").at(1);
        s.left(s.length() - 1);
        mDay[i + 1].low = s.toInt();

        mDay[i + 1].fx = objForecast.value("fx").toString();
        mDay[i + 1].fl = objForecast.value("fl").toString();

        mDay[i + 1].aqi = objForecast.value("aqi").toDouble();

    }


    mToday.ganmao = objData.value("ganmao").toString();

    mToday.wendu = objData.value("wendu").toString().toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toDouble();
    mToday.quality = objData.value("quality").toString();

    mToday.type = mDay[1].type;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
    mToday.high = mDay[1].high;
    mToday.low = mDay[1].low;

