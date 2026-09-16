#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("abriCSV");
    QCoreApplication::setApplicationName("abriCSV");

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        styleFile.close();
    } else {
        qWarning() << "Failed to open stylesheet:" << styleFile.errorString();
    }

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-VariableFont_wght.ttf");
    QFontDatabase::addApplicationFont(":/fonts/WorkSans-VariableFont_wght.ttf");

    MainWindow w;
    w.show();
    return a.exec();
}
