#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
class Database{
public:
    bool connect(const QString &dbName);
    bool creatDataBase();
    /*
    @number_in:应该是更新后的数据
    */
    bool writeAnItemToDataBase(int id_in, QString name_in, int number_in);
private:

};
#endif // DATABASE_H
