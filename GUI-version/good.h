#ifndef GOOD_H
#define GOOD_H
#include <QString>
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
#endif // GOOD_H
