#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#include "good.h"
#include "database.h"
#include <exception>//对于所有错误的处理形式都是一样的，所以没有对异常进行分类
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

class Warehouse
{

public:
    Warehouse();
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
    bool delete_goods(string id,string count);

    //查询：
    ///显示当前库存列表，包括商品名及其库存量
    void show_goods();
    //查看仓库中的name商品
    void find_goods(QString id, QString name);
    void readGoodsFromDataBase();
private:
    vector<Goods> goods;
    Database db;
    int stringToNum(const string& str)
    {
        istringstream iss(str);
        int num;
        iss >> num;
        return num;
    } //模板，将字符串类型的变量变为int型

};

#endif // WAREHOUSE_H
