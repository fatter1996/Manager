
#include <QApplication>
#include"stationshowfrom.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    stationshowfrom w;
    w.show();
    return a.exec();
}
