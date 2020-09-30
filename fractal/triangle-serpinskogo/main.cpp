#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "graphics/Triangle.hpp"
#include "graphics/SierpinskiTriangle.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sierpinski triangle");	//опрделения размера
    
    SierpinskiTriangle sierpinskiTriangle(
        std::vector<sf::Vector2f> {
            sf::Vector2f(80, 0),	//координаты
            sf::Vector2f(0, 160),
            sf::Vector2f(160, 160)
        },
        sf::Color::Black
    );

    sierpinskiTriangle.gen(3);
    std::vector<Triangle> triangles = sierpinskiTriangle.getTriangles();

    while (window.isOpen())	//цикл открытия файла
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // выйти из построения
            if (event.type == sf::Event::Closed)
                window.close();//закрыть
        }

        window.clear();		//очистить

        for(int i = 0; i < triangles.size(); i++)
        {
            window.draw(triangles[i].draw());
        }
        window.display();	//вывод
    }

    return 0;				//возврат 0
}