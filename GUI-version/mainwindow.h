#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "warehouse.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_addGoods_clicked();

    void on_delGoods_clicked();

    void on_showGoods_clicked();

    void on_search_clicked();

    void on_empty_clicked();

    void on_ok_clicked();

    void on_exit_clicked();
private:
    Ui::MainWindow *ui;
    char commod;
    Warehouse wre;
    void clear();
};

#endif // MAINWINDOW_H
