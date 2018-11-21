#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QDialog>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <vector>
#define ALL 1500
using namespace std;

namespace Ui {
class Warehouse;
}

class Goods{
public:
    Goods();
    void setId(int Id_i);
    int getId();
    void setName(QString name_i);
    QString getName();
    void setCount(int count_i);
    int getCount();
private:
    int id;
    QString name;
    int count;
};

class Warehouse : public QDialog
{
    Q_OBJECT

public:
    explicit Warehouse(QWidget *parent = nullptr);
    ~Warehouse();
    void empty();//初始化储存商品信息的vector
    bool inputCheck(string &id, QString &name, string &count);//检查用户输入的商品名和数量是否合法
    //进货：
    //进货，对应进货模块，表示当前进货一批数量为count的name商品
    bool add_goods(string id,QString name, string count);

    //更新库存列表，对应新增商品子功能，新增name商品且初始数量为count
    bool add_to_list(int id,QString name, int count);

    //出货：
    //出货，对应出货模块，表示当前出货一批数量为count的name商品
    bool delete_goods(string id,QString name, string count);

    //查询：
    ///显示当前库存列表，包括商品名及其库存量
    void show_goods();
    //查看仓库中的name商品
    void find_goods(QString name);
    void readGoodsFromDataBase();
private slots:
    void on_addGoods_clicked();

    void on_delGoods_clicked();

    void on_showGoods_clicked();

    void on_search_clicked();

    void on_empty_clicked();

    void on_ok_clicked();

    void on_exit_clicked();

private:
    Ui::Warehouse *ui;
    vector<Goods> goods;
    char commod;
    template <class Type>
    Type stringToNum(const string& str)
    {
        istringstream iss(str);
        Type num;
        iss >> num;
        return num;
    } //模板，将字符串类型的变量变为int型
    bool connect(const QString &dbName);
    bool creatDataBase();
    /*
    @number_in:应该是更新后的数据
    */
    bool writeAnItemToDataBase(int id_in, QString name_in, int number_in);
};

#endif // WAREHOUSE_H
