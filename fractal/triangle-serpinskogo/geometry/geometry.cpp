#include<SFML.hpp>			//подключение библиотеки 
#include<SFML/Graphics.hpp>		//вкл графики
#include<vector>			//вкл векторов
#include <iostream>			//вкл работы с символами

class TriangleGeometry
	public:
	std::vector /*объявление контейнера для хранения инфы*/ <sf::Vector2f> /*занесения в контейнер векторов*/ 
	getMidSide /*получение средней длины*/ (std::vector<sf::Vector2f> vertexs);/*получение вешины вектора*/
	
	build () //конструктор
	{
		std::vector  <sf::Vector2f> getMidSide (std::vector<sf::Vector2f> vertexs);
		{
			std::vector<sf::Vector2f> midSide; /*получение вершин*/

			midSide.push_back(
				sf::Vector2f((vertexs[0].x + vertexs[1].x) / 2, (vertexs[0].y + vertexs[1].y) / 2));

			midSide.push_back(
				sf::Vector2f((vertexs[0].x + vertexs[2].x) / 2, (vertexs[0].y + vertexs[2].y) / 2));

			midSide.push_back(
				sf::Vector2f((vertexs[1].x + vertexs[2].x) / 2, (vertexs[1].y + vertexs[2].y) / 2));

			/*midSide.push_back(
			 sf::Vector2f((vertexs[2].x + vertexs[2].x) / 2, (vertexs[2].y + vertexs[2].y) / 2)); // 4 вершина для реализации ковра
			 */
			 
			return midSide;
		}
	}