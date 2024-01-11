#include "WeatherTray.h"
#include "WeatherTool.h"

WeatherTray::WeatherTray(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);          //设置窗口无边框
    setFixedSize(width(), height());


    ui.leCity->setPlaceholderText("请输入需要搜索的城市");
    ui.leCity->setFocusPolicy(Qt::ClickFocus);

    mExitMenu = new QMenu(this);
    mExitAct = new QAction();

    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon(":/res/close.png"));

    mExitMenu->addAction(mExitAct);

    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
        });

    //将控件添加到控件对象数组
    mWeekList << ui.lblWeek0 << ui.lblWeek1 << ui.lblWeek2 << ui.lblWeek3 << ui.lblWeek4 << ui.lblWeek5;
    mDateList << ui.lblDate0 << ui.lblDate1 << ui.lblDate2 << ui.lblDate3 << ui.lblDate4 << ui.lblDate5;

    mTypeList << ui.lblType0 << ui.lblType1 << ui.lblType2 << ui.lblType3 << ui.lblType4 << ui.lblType5;
    mTypeIconList << ui.lblTypeIcon0 << ui.lblTypeIcon1 << ui.lblTypeIcon2 << ui.lblTypeIcon3 << ui.lblTypeIcon4 << ui.lblTypeIcon5;

    mAqiList << ui.lblQuality0 << ui.lblQuality1 << ui.lblQuality2 << ui.lblQuality3 << ui.lblQuality4 << ui.lblQuality5;

    mFxList << ui.lblFx0 << ui.lblFx1 << ui.lblFx2 << ui.lblFx3 << ui.lblFx4 << ui.lblFx5;
    mFlList << ui.lblFl0 << ui.lblFl1 << ui.lblFl2 << ui.lblFl3 << ui.lblFl4 << ui.lblFl5;

    mTypeMap.insert("暴雪", ":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨", ":/res/type/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨", ":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨", ":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨", ":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪", ":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪", ":/res/type/Daxue.png");
    mTypeMap.insert("大雨", ":/res/type/DaYu.png");
    mTypeMap.insert("冻雨", ":/res/type/DongYu.png");
    mTypeMap.insert("多云", ":/res/type/DuoYun.png");
    mTypeMap.insert("浮尘", ":/res/type/Fuchen.png");
    mTypeMap.insert("雷阵雨", ":/res/type/LeizhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹", ":/res/type/LeizhenYuBanYouBingBao.png");
    mTypeMap.insert("霾", ":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴", ":/res/type/QiangshachenBao.png");
    mTypeMap.insert("晴", ":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴", ":/res/type/shaChenBao.png");
    mTypeMap.insert("特大暴雨", ":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined", ":/res/type/undefined.png");
    mTypeMap.insert("雾", ":/res/type/wu.png");
    mTypeMap.insert("小到中雪", ":/res/type/XiaoDaozhongXue.png");
    mTypeMap.insert("小到中雨", ":/res/type/xiaoDaozhongYu.png");
    mTypeMap.insert("小雪", ":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨", ":/res/type/XiaoYu.png");
    mTypeMap.insert("雪", ":/res/type/xue.png");
    mTypeMap.insert("扬沙", ":/res/type/Yangsha.png");
    mTypeMap.insert("阴", ":/res/type/Yin.png");
    mTypeMap.insert("雨", ":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪", ":/res/type/YuJiaxue.png");
    mTypeMap.insert("阵雪", ":/res/type/zhenxue.png");
    mTypeMap.insert("阵雨", ":/res/type/zhenYu.png");
    mTypeMap.insert("中到大雪", ":/res/type/zhongDaoDaXue.png");
    mTypeMap.insert("中到大雨", ":/res/type/zhongDaoDaYu.png");
    mTypeMap.insert("中雪", " :/res/type/zhongXue.png");
    mTypeMap.insert("中雨", ":/res/type/ZhongYu.png");

    mNetAccessManager = new QNetworkAccessManager(this);

    connect(mNetAccessManager, &QNetworkAccessManager::finished, this, &WeatherTray::onReplied);
    
    //getWeatherInfo("101010100");
    
    getWeatherInfo("东莞");

    ui.lblHighCurve->installEventFilter(this);
    ui.lblLowCurve->installEventFilter(this);
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


//void WeatherTray::getWeatherInfo(QString cityCode)
void WeatherTray::getWeatherInfo(QString cityName)
{
    ////
    QString cityCode = WeatherTool::getCityCode(cityName);
    ////
    if (cityCode.isEmpty())
    {
        QMessageBox::warning(this, "天气", "请检查输入是否正确！", QMessageBox::Ok);
        return;
    }

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

    ui.lblHighCurve->update();
    ui.lblLowCurve->update();
}

void WeatherTray::updateUI()
{
    ui.lblDate->setText(QDateTime::fromString(mToday.date,"yyyyMMdd").toString("yyyy/MM/dd") + " " + mDay[1].week);
    ui.lblCity->setText(mToday.city);

    ui.lblTypeIcon->setPixmap(mTypeMap[mToday.type]);
    ui.lblTemp->setText(QString::number(mToday.wendu) + "°");
    ui.lblType->setText(mToday.type);
    ui.lblLowHigh->setText(QString::number(mToday.low) + "°~" + QString::number(mToday.high) + "°");

    ui.lblGanMao->setText("感冒指数：" + mToday.ganmao);

    ui.lblWindFx->setText(mToday.fx);
    ui.lblWindFl->setText(mToday.fl);

    ui.lblPM25->setText(QString::number(mToday.pm25));

    ui.lblShiDu->setText(mToday.shidu);
    ui.lblQuality->setText(mToday.quality);


    for (int i = 0; i < 6; i++)
    {
        mWeekList[i]->setText("周" + mDay[i].week.right(1));
        ui.lblWeek0->setText("昨天");
        ui.lblWeek1->setText("今天");
        ui.lblWeek2->setText("明天");

        QStringList ymdList = mDay[i].date.split("-");
        mDateList[i]->setText(ymdList[1] + "/" + ymdList[2]);

        mTypeList[i]->setText(mDay[i].type);
        mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);

        if (mDay[i].aqi > 0 && mDay[i].aqi <= 50)
        {
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color: rgb(121, 184, 0);padding:8px;");
        }
        else if (mDay[i].aqi > 50 && mDay[i].aqi <= 100)
        {
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color: rgb(255, 187, 23);padding:8px;");
        }
        else if (mDay[i].aqi >100 && mDay[i].aqi <= 150)
        {
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color: rgb(255, 87, 97);padding:8px;");
        }
        else if (mDay[i].aqi >150 && mDay[i].aqi <= 200)
        {
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color: rgb(235, 17, 27);padding:8px;");
        }
        else if (mDay[i].aqi > 200&& mDay[i].aqi <= 250)
        {
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color: rgb(170, 0, 0);padding:8px;");
        }
        else
        {
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color: rgb(110, 0, 0);padding:8px;");
        }

        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);
    }
}

bool WeatherTray::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.lblHighCurve && event->type() == QEvent::Paint)
    {
        paintHighCurve();
    }
    if (watched == ui.lblLowCurve && event->type() == QEvent::Paint)
    {
        paintLowCurve();
    }
    return QWidget::eventFilter(watched, event);
}

void WeatherTray::paintHighCurve()
{
    QPainter painter(ui.lblHighCurve);

    painter.setRenderHint(QPainter::Antialiasing, true);          //抗锯齿

    int pointX[6] = { 0 };
    for (int i = 0; i < 6; i++)
    {
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width() / 2;
    }

    int tempSum = 0;
    int tempAverage = 0;

    for (int i = 0; i < 6; i++)
    {
        tempSum += mDay[i].high;
    }
    tempAverage = tempSum / 6;

    int pointY[6] = { 0 };
    int yCenter = ui.lblHighCurve->height() / 2;

    for (int i = 0; i < 6; i++)
    {
        pointY[i] = yCenter - ((mDay[i].high - tempAverage) * INCREMENT);
    }
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(255, 170, 0));

    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));
    for (int i = 0; i < 6; i++)
    {
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);

        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y, QString::number(mDay[i].high) + "°");
    }

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i], pointY[i], pointX[i + 1], pointY[i + 1]);
    }
}

void WeatherTray::paintLowCurve()
{
    QPainter painter(ui.lblLowCurve);

    painter.setRenderHint(QPainter::Antialiasing, true);          //抗锯齿

    int pointX[6] = { 0 };
    for (int i = 0; i < 6; i++)
    {
        pointX[i] = mWeekList[i]->pos().x() + mWeekList[i]->width() / 2;
    }

    int tempSum = 0;
    int tempAverage = 0;

    for (int i = 0; i < 6; i++)
    {
        tempSum += mDay[i].low;
    }
    tempAverage = tempSum / 6;

    int pointY[6] = { 0 };
    int yCenter = ui.lblLowCurve->height() / 2;

    for (int i = 0; i < 6; i++)
    {
        pointY[i] = yCenter - ((mDay[i].low - tempAverage) * INCREMENT);
    }
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(0, 255, 255));

    painter.setPen(pen);
    painter.setBrush(QColor(0, 255, 255));
    for (int i = 0; i < 6; i++)
    {
        painter.drawEllipse(QPoint(pointX[i], pointY[i]), POINT_RADIUS, POINT_RADIUS);

        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y, QString::number(mDay[i].low) + "°");
    }

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i], pointY[i], pointX[i + 1], pointY[i + 1]);
    }
}

void WeatherTray::on_btnSearch_clicked()
{
    QString cityName = ui.leCity->text();
    getWeatherInfo(cityName);
}
