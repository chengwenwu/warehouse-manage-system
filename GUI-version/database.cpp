#include "database.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
bool Database::connect(const QString &dbName)
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

 bool Database::creatDataBase()
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

 bool Database::writeAnItemToDataBase(int id_in, QString name_in, int number_in)
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
        return true;
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
