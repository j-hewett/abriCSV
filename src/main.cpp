#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("abriCSV");
    QCoreApplication::setApplicationName("abriCSV");

    MainWindow w;
    w.show();
    return a.exec();
}
