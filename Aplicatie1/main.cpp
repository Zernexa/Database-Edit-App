#include "mainwindow.h"
#include "database.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!Database::initialize()) {
        return -1; // Exit app if DB fails to initialize
    }

    MainWindow w;
    w.show();
    return a.exec();
}
