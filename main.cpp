#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName(QString("LemonxTech"));
    QCoreApplication::setApplicationName(QString("LemonxNote"));
    MainWindow w;
    w.show();
    return a.exec();
}
