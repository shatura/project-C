#include "SierpinskiTriangle.hpp"

SierpinskiTriangle::SierpinskiTriangle(	//объява дпостроя супер дупер треугольника бешенной собаки 
    std::vector<sf::Vector2f> vertexs,
    sf::Color color
)
{
    Triangle triangle(vertexs, color);
    this->triangles.push_back(triangle);
}
void SierpinskiTriangle::calcContour(
    std::vector<sf::Vector2f> midSide,				//пересчет среднего коэфициента
    std::vector<sf::Vector2f> currentContour,		//подсчет текущего контура
    std::vector<std::vector<sf::Vector2f>>& storage //указание на прошлый контур
)
{		//точки расстановки 
    storage.push_back(
        std::vector<sf::Vector2f>{
            midSide[0],
            currentContour[0],
            midSide[1]
        }
    );

    storage.push_back(
        std::vector<sf::Vector2f>{
            midSide[0],
            currentContour[1],
            midSide[2]
        }  
    );

    storage.push_back(
        std::vector<sf::Vector2f>{
            midSide[1],
            midSide[2],
            currentContour[2]
        }  
    );
}
void SierpinskiTriangle::gen(size_t deep) //Генерация треугольника Серпинского до определенной глубины
{
    /**
     * Контуры, исходя из которых генерируется треугольник
     **/
    std::vector<std::vector<sf::Vector2f>> contour;
    contour.push_back(this->triangles[0].getVertex());

    for(int i = 0; i < deep; i++)
    {
        std::vector<sf::Vector2f> midSide = TriangleGeometry::getMidSide(contour[i]);
        this->triangles.push_back(
            Triangle(
                midSide,
                sf::Color::Red,
                true
            )
        );

        this->calcContour(midSide, contour[i], contour);
    }
}

std::vector<Triangle> SierpinskiTriangle::getTriangles()
{
    return this->triangles;			//возврат 
}