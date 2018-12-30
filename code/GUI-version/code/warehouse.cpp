#include "warehouse.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
Warehouse::Warehouse()
{
    this->goods.clear();
    if(!db.creatDataBase())
    {
        QMessageBox message(QMessageBox::Warning, "waring", "Creat Database failed!", QMessageBox::Ok);
        if(message.exec() == QMessageBox::Ok)
            exit(-1);
    }
    this->readGoodsFromDataBase();
}

Warehouse::~Warehouse()
{
}

void Warehouse::empty()//用来将仓库中的货物清零
{
    QSqlQuery query;
    query.exec("delete from goods");
    this->goods.clear();
    QString str = "Warehouse is empty!";
    throw str;
}


bool Warehouse::inputCheck(string &id, QString &name, string &count)
{
    QString text = "";
    if(id.length() > 100 || id.length() == 0)
    {
        text = "Id is wrong, please input again!\n";
    }
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
        throw text;
    }
    return true;
}
bool Warehouse::add_goods(string id, QString name, string count)
{
    try {
        if(!inputCheck(id, name, count))
        {
            return false;
        }
        int id_temp = stringToNum(id);
        int number = stringToNum(count);
        vector<Goods>::iterator it;
        for (it = goods.begin(); it != goods.end();it++)
        {
            if (it->getId() == id_temp)
            {
                if(it->getName() != name)
                {
                    QString str = "Id and name mismatch\n";
                    throw str;
                }
                //存到数据库中
                db.writeAnItemToDataBase(id_temp,name,it->getCount() + number);
                it->setCount(it->getCount() + number);
                QString str = "Goods have been stored!";
                throw str;
            }
        } //若已存在要入库的商品，则只需增加其数量
        if (it ==  goods.end())
        {
            add_to_list(id_temp, name, number);
            return true;
        } //若目前仓库中没有该商品，则将其加入商品列表

    } catch (QString str1) {
        throw str1;
    }

}


bool Warehouse::add_to_list(int id, QString name, int count)
//在列表中加入新的商品，该函数在函数add_goods(string name, int count)内部调用
{
    try {
        if (this->goods.size() < ALL)//仓库未满直接存储
        {
            Goods good;
            good.setId(id);
            good.setName(name);
            good.setCount(count);

            //存进数据库
            db.writeAnItemToDataBase(id,name,count);
            this->goods.push_back(good);
            QString str = "Goods have been stored!";
            throw str;
        }
        else
        {
            QString str = "save failed\n warehouse is full!";
            throw str;
        } //当储存位置已经满了的时候，显示“仓库已满”
    } catch (QString str1) {
        throw str1;
    }

}

bool Warehouse::delete_goods(string id,string count)
//出货
{
    QString null = "null";
    if(!inputCheck(id, null, count))
    {
        return false;
    }
    int id_temp = stringToNum(id);
    int number = stringToNum(count);
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        if (it->getId() == id_temp)
        {
            if ((it->getCount() - number) < 0)
            {
                QString str = "Goods is not enough!";
                throw str;
            } //出货数量大于库存时，拒绝请求
            else if ((it->getCount() -number) == 0)
            {
                db.writeAnItemToDataBase(id_temp,it->getName(),-1);
                this->goods.erase(it);

                QString str = "OK!\nAll this Goods have been delete!";
                throw str;
                //return true;
            } //出货数量刚好等于库存时，出货，并将该商品从列中移除
            else if ((it->getCount() - number) > 0)
            {
                //存进数据库中
                db.writeAnItemToDataBase(id_temp,it->getName(),it->getCount() - number);
                it->setCount(it->getCount() - number);
                QString str = "OK!";
                throw str;
            }
            return true;
        }
    }
    if (it == goods.end())
    {
        QString str = "Error\n Goods not exists";
         throw str;
    } //若目前仓库中没有该商品，提示未找到

}

void Warehouse::show_goods()
//显示目前仓库中所有商品及其数量
{
    QString text = "Id        Name           number\n";
    if(goods.size() == 0)
    {
        QMessageBox massagebox(QMessageBox::Warning,"waring", "warehouse is empty", QMessageBox::Ok,NULL);
        massagebox.exec();
        return ;
    }
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        QString id =QString::number(it->getId());
        QString count = QString::number(it->getCount());
        text = text+id+"        "+ it->getName()+"           "+count+"\n";
    }
   throw text;
}
void Warehouse::find_goods(QString id, QString name)
//在所有商品中进行查找目标商品
{
    QString text = "Id        Name           number\n";
    vector<Goods>::iterator it;
    for (it = goods.begin(); it != goods.end(); it++)
    {
        if (it->getName() == name || QString::number(it->getId()) == id)
        {
            QString id_r = QString::number(it->getId());
            QString name_r = it->getName();
            QString num_r = QString::number(it->getCount());
            text += (id_r +"        "+name_r+"           "+num_r+"\n");
        }
    }
    throw text;
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
