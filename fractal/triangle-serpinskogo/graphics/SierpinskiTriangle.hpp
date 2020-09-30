#include<SFML.hpp>			   				//подключение библиотеки 
#include<SFML/Graphics.hpp>					//вкл графики
#include<vector>							//вкл векторов
#include <iostream>							//вкл работы с символами

#include "Triangle.hpp"
#include "../geometry/TriangleGeometry.hpp"	//повторный вызов себя же для построения 

class SierpinskiTriangle
{
public:
   
    std::vector<Triangle> triangles;					//объявления векторонго треугольника 
    void calcContour(									//Пересчет контуров
        std::vector<sf::Vector2f> midSide,				//пересчет среднего коэфициента 
        std::vector<sf::Vector2f> currentContour,		//подсчет текущего контура
        std::vector<std::vector<sf::Vector2f>>& storage	//указание на прошлый контур 
    );

    SierpinskiTriangle(									//контруктор
        std::vector<sf::Vector2f> vertexs,				//построение
        sf::Color color									//придание цвета
    );

       void gen(size_t deep); //Генерация треугольника Серпинского до максимально возможной глубины

    std::vector<Triangle> getTriangles();
};