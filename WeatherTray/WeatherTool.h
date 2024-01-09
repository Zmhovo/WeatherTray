#ifndef __WEATHERTOOL_H__
#define __WEATHERTOOL_H__

#include <QString>
#include <QMap>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

class WeatherTool
{
public:
	static QString getCityCode(QString cityName)
	{
		if (mCityMap.isEmpty())
		{
			initCityMap();
		}
		QMap<QString, QString>::Iterator it = mCityMap.find(cityName);
		if (it == mCityMap.end())
		{
			it = mCityMap.find(cityName + "ÊÐ");
		}

		if (it != mCityMap.end())
		{
			return it.value();
		}

		return "";
	}
private:
	static QMap<QString, QString> mCityMap;
	static void initCityMap()
	{
		QString filePath = "D:/zmh55/Documents/Study/QT/WeatherTray/WeatherTray/sojson_com_city.json";

		QFile file(filePath);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QByteArray json = file.readAll();
		file.close();

		QJsonParseError err;
		QJsonDocument doc = QJsonDocument::fromJson(json, &err);

		if (err.error != QJsonParseError::NoError)
		{
			return;
		}

		if (!doc.isArray())
		{
			return;
		}

		QJsonArray cities = doc.array();
		for (int i = 0; i < cities.size(); i++)
		{
			QString city = cities[i].toObject().value("name").toString();
			QString code = cities[i].toObject().value("city_code").toString();
			if (code.size() > 0)
			{
				mCityMap.insert(city, code);
			}
		}
	}
};

QMap<QString, QString> WeatherTool::mCityMap = {};
#endif //!__WEATHERTOOL_H__
