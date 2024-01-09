#include "WeatherTray.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WeatherTray w;
    
    w.show();
    return a.exec();
}
