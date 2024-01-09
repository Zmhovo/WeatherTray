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

    //将控件添加到控件对象数组
//    mWeekList << ui.lblWeek0 << ui.lblWeek1 << ui.lblWeek2 << ui.lblWeek3 << ui.lblWeek4 << ui.lblWeek5;
//    mDateList << ui.lblDate0 << ui.lblDate1 << ui.lblDate2 << ui.lblDate3 << ui.lblDate4 << ui.lblDate5;
//
//    mTypeList << ui.lblType0 << ui.lblType1 << ui.lblType2 << ui.lblType3 << ui.lblType4 << ui.lblType5;
//    mTypeIconList << ui.lblTypeIcon0 << ui.lblTypeIcon1 << ui.lblTypeIcon2 << ui.lblTypeIcon3 << ui.lblTypeIcon4 << ui.lblTypeIcon5;
//
//    mAqiList << ui.lblQuality0 << ui.lblQuality1 << ui.lblQuality2 << ui.lblQuality3 << ui.lblQuality4 << ui.lblQuality5;
//
//    mFxList << ui.lblFx0 << ui.lblFx1 << ui.lblFx2 << ui.lblFx3 << ui.lblFx4 << ui.lblFx5;
//    mFlList << ui.lblFl0 << ui.lblFl1 << ui.lblFl2 << ui.lblFl3 << ui.lblFl4 << ui.lblFl5;

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

void WeatherTray::parseJson(QByteArray& byteArray)
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

    QJsonObject objYesterday = objData.value("yesterday").toObject();

    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();

    mDay[0].type = objYesterday.value("type").toString();

    QString s;
    s = objYesterday.value("high").toString().split(" ").at(1);
    s = s.left(s.length() - 1);
    mDay[0].high = s.toInt();

    s = objYesterday.value("low").toString().split(" ").at(1);
    s = s.left(s.length() - 1);
    mDay[0].low = s.toInt();

    mDay[0].fx = objYesterday.value("fx").toString();
    mDay[0].fl = objYesterday.value("fl").toString();

    mDay[0].aqi = objYesterday.value("aqi").toDouble();


    QJsonArray forecastArr = objData.value("forecast").toArray();

    for (int i = 0; i < 5; i++)
    {
        QJsonObject objForecast = forecastArr[i].toObject();
        mDay[i + 1].week = objForecast.value("week").toString();
        mDay[i + 1].date = objForecast.value("ymd").toString();

        mDay[i + 1].type = objForecast.value("type").toString();

        QString s;
        s = objForecast.value("high").toString().split(" ").at(1);
        s = s.left(s.length() - 1);
        mDay[i + 1].high = s.toInt();

        s = objForecast.value("low").toString().split(" ").at(1);
        s = s.left(s.length() - 1);
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

    updateUI();
}

void WeatherTray::updateUI()
{
    ui.lblDate->setText(QDateTime::fromString(mToday.date,"yyyyMMdd").toString("yyyy/MM/dd") + " " + mDay[1].week);
    ui.lblCity->setText(mToday.city);
}
