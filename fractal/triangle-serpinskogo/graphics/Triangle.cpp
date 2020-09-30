#include "Triangle.hpp"

Triangle::Triangle(						//объявление данных 
    std::vector<sf::Vector2f> vertexs, 
    sf::Color color,
    bool TurnOver
)
{
    this->vertexs = vertexs;				//операции присваивания 
    this->color = color;
    this->position = position;
    this->triangle.setPointCount(3);		//операция присваивания с уточнением количества точек(4-квадрат)

    for(int i = 0; i < vertexs.size(); i++)	//цикл для построения  
    {
        this->triangle.setPoint(i, vertexs[i]);
    }
    this->triangle.setPosition(position);	//установка позициидля отображения 
    if(TurnOver) triangle.setRotation(180.0f);	//переворот
}
std::vector<sf::Vector2f> Triangle::getVertex()//Получение вершин
{
    return this->vertexs;					//возвращение
}
sf::ConvexShape Triangle::draw()			//Отрисовка треугольника
{
    return this->triangle;					//возвращение
}