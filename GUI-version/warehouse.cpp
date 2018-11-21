#include "warehouse.h"
#include "ui_warehouse.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
Warehouse::Warehouse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warehouse)
{
    ui->setupUi(this);
    this->goods.clear();
    QPixmap pixmap = QPixmap("../GUI-version/back.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    if(!this->creatDataBase())
    {
        QMessageBox message(QMessageBox::Warning, "waring", "Creat Database failed!", QMessageBox::Ok);
        if(message.exec() == QMessageBox::Ok)
            exit(-1);

    }
    this->readGoodsFromDataBase();
}

Warehouse::~Warehouse()
{
    delete ui;
}


Goods::Goods()
{
    this->name = "";
    this->count = 0;
}

void  Goods::setId(int Id_i)
{
    this->id = Id_i;
}
int  Goods::getId()
{
    return this->id;
}

void Goods::setName(QString name_i)
{
    this->name = name_i;
}
QString Goods::getName()
{
    return this->name;
}
void Goods::setCount(int count_i)
{
    this->count = count_i;
}
int Goods::getCount()
{
    return this->count;
}

void Warehouse::empty()//用来将仓库中的货物清零
{
    this->goods.clear();
    QSqlQuery query;
    query.exec("delete from goods");
    ui->textBrowser->setText("Warehouse is empty!");
}


bool Warehouse::inputCheck(string &id, QString &name, string &count)
{
    QString text = "";
    if(name.length() > 100 || name.length() == 0)
    {
        text = "Name is wrong, please input again!\n";
    }


    if(count.length() >= 10)
    {
        text += "number is wrong, please input again!\n";
    }
    int coun;
    for (coun = 0; coun < count.length(); coun++)
    {
        if (!isdigit(count[coun]))
        {
            text += "Goods number should only include number!\n";
            break;
        }
    }

    for (coun = 0; coun < id.length(); coun++)
    {
        if (!isdigit(id[coun]))
        {
            text += "id should only include number!\n";
            break;
        }
    }

    if(text != "")
    {
        ui->textBrowser->setText(text);
        return false;
    }
    return true;
}
bool Warehouse::add_goods(string id, QString name, string count)
{
    if(!inputCheck(id, name, count))
    {
        return false;
    }
    int id_temp = stringToNum<int>(id);
    int number = stringToNum<int>(count);
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end();it++)
    {
        if (it->getId() == id_temp)
        {
            it->setCount(it->getCount() + number);
            //存到数据库中
            this->writeAnItemToDataBase(id_temp,name,it->getCount() + number);
            ui->textBrowser->setText("Goods have been stored!");
            return true;
        }
    } //若已存在要入库的商品，则只需增加其数量
    if (it ==  goods.end())
    {
        add_to_list(id_temp, name, number);
        return true;
    } //若目前仓库中没有该商品，则将其加入商品列表

}



bool Warehouse::add_to_list(int id, QString name, int count)
//在列表中加入新的商品，该函数在函数add_goods(string name, int count)内部调用
{
    if (this->goods.size() < ALL)//仓库未满直接存储
    {
        Goods good;
        good.setId(id);
        good.setName(name);
        good.setCount(count);

        this->goods.push_back(good);
        //存进数据库
        this->writeAnItemToDataBase(id,name,count);
        ui->textBrowser->setText("Goods have been stored!");
        return true;
    }
    else
    {
        ui->textBrowser->setText("save failed\n warehouse is full!");
        return false;
    } //当储存位置已经满了的时候，显示“仓库已满”
}

bool Warehouse::delete_goods(string id, QString name, string count)
//出货
{
    if(!inputCheck(id, name, count))
    {
        return false;
    }
    int id_temp = stringToNum<int>(id);
    int number = stringToNum<int>(count);
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        if (it->getId() == id_temp)
        {
            if ((it->getCount() - number) < 0)
            {
                ui->textBrowser->setText("Goods is not enough!");
                return false;
            } //出货数量大于库存时，拒绝请求
            else if ((it->getCount() -number) == 0)
            {
                this->goods.erase(it);
                this->writeAnItemToDataBase(id_temp,name,-1);
                 ui->textBrowser->setText("OK!\nAll this Goods have been delete!");
                return true;
            } //出货数量刚好等于库存时，出货，并将该商品从列中移除
            else if ((it->getCount() - number) > 0)
            {
                //存进数据库中
                this->writeAnItemToDataBase(id_temp,name,it->getCount() - number);
                it->setCount(it->getCount() - number);
                 ui->textBrowser->setText("OK!");
                return true;
            }
            return true;
        }
    }
    if (it == goods.end())
    {
         ui->textBrowser->setText("Error\n Goods not exists");
        return false;
    } //若目前仓库中没有该商品，提示未找到

}

void Warehouse::show_goods()
//显示目前仓库中所有商品及其数量
{
    QString text = "Goods           number\n";
    if(goods.size() == 0)
    {
        QMessageBox massagebox(QMessageBox::Warning,"waring", "warehouse is empty", QMessageBox::Ok,NULL);
        massagebox.exec();
        return ;
    }
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        QString count = QString::number(it->getCount());
        text = text+ it->getName()+"           "+count+"\n";
    }
    ui->textBrowser->setText(text);
}
void Warehouse::find_goods(QString name)
//在所有商品中进行查找目标商品
{
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        if (it->getName() == name)
        {
            QString num = QString::number(it->getCount());

           ui->textBrowser->setText( "goods name " +name + "  number is " + num + "\n");
            break;
        }
    }
    if (it == goods.end())
    {
        ui->textBrowser->setText("not have this goods!");
    }
}

void Warehouse::on_addGoods_clicked()
{
    ui->godnm->clear();
    ui->godnum->clear();
    ui->godnm->setEnabled(true);
    ui->godnum->setEnabled(true);
    ui->textBrowser->clear();
    this->commod = '1';
}

void Warehouse::on_delGoods_clicked()
{

    ui->godnm->clear();
    ui->godnum->clear();
    ui->godnm->setEnabled(true);
    ui->godnum->setEnabled(true);
    ui->textBrowser->clear();
    this->commod = '2';
    this->show_goods();
}

void Warehouse::on_showGoods_clicked()
{
    ui->godnm->clear();
    ui->godnum->clear();
    ui->godnm->setEnabled(false);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    show_goods();
    this->commod = '3';
}

void Warehouse::on_search_clicked()
{
    ui->godnm->clear();
    ui->godnum->clear();
    ui->godnm->setEnabled(true);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    this->commod = '4';
}

void Warehouse::on_empty_clicked()
{
    ui->godnm->clear();
    ui->godnum->clear();
    ui->godnm->setEnabled(false);
    ui->godnum->setEnabled(false);
    ui->textBrowser->clear();
    QMessageBox massagebox(QMessageBox::Warning,"waring", "Empty warehouse?", QMessageBox::Yes|QMessageBox::No);
    if (massagebox.exec() == QMessageBox::Yes)
            this->empty();
}

void Warehouse::on_ok_clicked()
{
    switch(this->commod)//开关实现各功能分块选择
    {
        case '1' ://进货
        {
            add_goods(ui->id->text().toStdString(), ui->godnm ->text(),ui->godnum->text().toStdString());
            break;
        }
        case '2' ://出货
        {

            delete_goods(ui->id->text().toStdString(),ui->godnm ->text(),ui->godnum->text().toStdString());
            break;
        }

        case '4' ://实现查询
        {
            find_goods(ui->godnm ->text());
            break;
        }
        case '5' ://清空仓库
        {
            empty();
        }
    }
}

void Warehouse::on_exit_clicked()
{
    QMessageBox messageBox(QMessageBox::Warning, "waring", "Exit the system?", QMessageBox::Yes|QMessageBox::No);
    if(messageBox.exec()==QMessageBox::Yes)
        qApp->exit(0);
}

bool Warehouse::connect(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("host");
//    db.setDatabaseName("dbname");
//    db.setUserName("username");
//    db.setPassword("password");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"), db.lastError().text());
        return false;
    }
    return true;
}

 bool Warehouse::creatDataBase()
 {
     if (connect("demo.db"))
     {
         QSqlQuery query;
           //判断表是否已经存在
         QString sql = QString("select * from sqlite_master where name='%1'").arg("goods");
         query.exec(sql);
         if(query.next())
             return true;
         query.finish();
         if (!query.exec("CREATE TABLE goods("
                         "id INT,"
                         "name VARCHAR,"
                         "number INT)")) {
             QMessageBox::critical(0, QObject::tr("Database Error"),
                                   query.lastError().text());
             return false;
         }
         return true;

      }
     return false;
 }

 bool Warehouse::writeAnItemToDataBase(int id_in, QString name_in, int number_in)
 {

     QSqlQuery query;
    if(number_in == -1)
    {
        query.prepare("delete from goods where id = :id_in");
        query.bindValue(":id_in", id_in);

        if(!query.exec())
        {
            QMessageBox::critical(0, QObject::tr("Database Error -1"),
                                  query.lastError().text());
            return false;
        }
        return true;
    }

    query.prepare("select * from goods where id = :id_in");
    query.bindValue(":id_in", id_in);
    query.exec();
    if(query.next())
    {
        query.finish();
        query.prepare("update goods set number = :number_in where id = :id_in");
        query.bindValue(":id_in",id_in);
        query.bindValue(":number_in", number_in);
        if(!query.exec())
        {
            QMessageBox::critical(0, QObject::tr("Database Error-2"),
                                  query.lastError().text());
            return false;
        }
    }
    else
    {
        query.finish();
        query.prepare("insert into goods(id, name, number) values(:id_in,:name_in,:number_in)");
        query.bindValue(":id_in",id_in);
        query.bindValue(":name_in",name_in);
        query.bindValue(":number_in", number_in);
        if(!query.exec())
        {
            QMessageBox::critical(0, QObject::tr("Database Error-3"),
                                  query.lastError().text());
            return false;
        }
    }
    return true;


 }

 void Warehouse::readGoodsFromDataBase()
 {
     QSqlQuery query;
     query.exec("SELECT id, name, number FROM goods");
         while (query.next()) {
             int id_in = query.value(0).toInt();
             QString name_in = query.value(1).toString();
             int number_in = query.value(2).toInt();
             Goods good;
             good.setId(id_in);
             good.setName(name_in);
             good.setCount(number_in);

             this->goods.push_back(good);
         }

 }
