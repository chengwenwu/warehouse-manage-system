#include "good.h"
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
