#ifndef __WEATHERDATA_H__
#define __WEATHERDATA_H__

#include <QString>

class Today
{
public:
	Today()
	{
		date = "2024-01-09";
		city = "东莞";

		ganmao = "感冒指数";

		wendu = 0;
		shidu = "0%";
		pm25 = 0;
		quality = "无数据";

		type = "多云";

		fx = "南风";
		fl = "2级";

		high = 30;
		low = 18;
	}

	QString date;
	QString city;

	QString ganmao;

	int wendu;
	QString shidu;
	int pm25;
	QString quality;

	QString type;

	QString fx;
	QString fl;

	int high;
	int low;
};

class Day
{
public:
	Day()
	{
		date = "2024-01-09";
		week = "周二";

		type = "多云";

		fx = "南风";
		fl = "2级";

		high = 30;
		low = 18;

		aqi = 0;
	}

	QString date;
	QString week;

	QString type;

	QString fx;
	QString fl;

	int high;
	int low;
	
	int aqi;
};
#endif // !__WEATHERDATA_H__