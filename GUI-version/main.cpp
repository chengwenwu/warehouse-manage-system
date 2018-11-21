#include "mainwindow.h"
#include "warehouse.h"
#include <QApplication>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Warehouse wre;

    wre.show();
    return a.exec();
}
