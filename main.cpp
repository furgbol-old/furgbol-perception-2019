#include <QApplication>
#include "system.h"
#include "mainwindow.h"
#include "config.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow ui;
    ui.show();

    return a.exec();
}
