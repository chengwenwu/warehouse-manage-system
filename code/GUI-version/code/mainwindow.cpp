#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addGoods_clicked()
{
    this->clear();
    ui->id->setEnabled(true);
    ui->godnm->setEnabled(true);
    ui->godnum->setEnabled(true);
    ui->textBrowser->clear();
    this->commod = '1';
}

void MainWindow::on_delGoods_clicked()
{
    this->clear();
    ui->id->setEnabled(true);
    ui->godnm->setEnabled(false);
    ui->godnum->setEnabled(true);
    ui->textBrowser->clear();
    this->commod = '2';
    try {
       wre.show_goods();
    } catch (QString str) {
        this->ui->textBrowser->setText(str);
    }

}

void MainWindow::on_showGoods_clicked()
{
    this->clear();
    ui->id->setEnabled(false);
    ui->godnm->setEnabled(false);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    try {
        wre.show_goods();
    } catch (QString str) {
        this->ui->textBrowser->setText(str);
    }

    this->commod = '3';
}

void MainWindow::on_search_clicked()
{
    this->clear();
    ui->id->setEnabled(true);
    ui->godnm->setEnabled(true);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    this->commod = '4';
}

void MainWindow::on_empty_clicked()
{
    this->clear();
    ui->id->setEnabled(false);
    ui->godnm->setEnabled(false);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    QMessageBox massagebox(QMessageBox::Warning,"waring", "Empty warehouse?", QMessageBox::Yes|QMessageBox::No);
    if (massagebox.exec() == QMessageBox::Yes)
    {
        try {
             wre.empty();
        } catch (QString str) {
            this->ui->textBrowser->setText(str);
        }
    }

}

void MainWindow::on_ok_clicked()
{
    switch(this->commod)//开关实现各功能分块选择
    {
        case '1' ://进货
        {
            try {
                wre.add_goods(ui->id->text().toStdString(), ui->godnm ->text(),ui->godnum->text().toStdString());
                break;
            } catch (QString str) {
                this->ui->textBrowser->setText(str);
                break;
            }
        }
        case '2' ://出货
        {
            try {
                wre.delete_goods(ui->id->text().toStdString(),ui->godnum->text().toStdString());
                break;
            } catch (QString str) {
                this->ui->textBrowser->setText(str);
                break;
            }

        }

        case '4' ://实现查询
        {
            try {
                wre.find_goods(ui->id->text(),ui->godnm ->text());
                break;
            } catch (QString str) {
                this->ui->textBrowser->setText(str);
                break;
            }

        }
        case '5' ://清空仓库
        {
            try {
                wre.empty();
            } catch (QString str) {
                this->ui->textBrowser->setText(str);
            }

        }
    }
}

void MainWindow::on_exit_clicked()
{
    QMessageBox messageBox(QMessageBox::Warning, "waring", "Exit the system?", QMessageBox::Yes|QMessageBox::No);
    if(messageBox.exec()==QMessageBox::Yes)
        qApp->exit(0);
}

void MainWindow::clear()
{
    ui->godnm->clear();
    ui->godnum->clear();
    ui->id->clear();
}
