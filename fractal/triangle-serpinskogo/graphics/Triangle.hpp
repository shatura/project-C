#pragma once					//одноразовый
#include<SFML.hpp>			    //подключение библиотеки 
#include<SFML/Graphics.hpp>		//вкл графики
#include<vector>				//вкл векторов
#include <iostream>				//вкл работы с символами

class Triangle
{
private:
      
    std::vector<sf::Vector2f> vertexs; 		//контейнер для вершины треугольника
	/*sf-работа с векторами*/
    sf::Color color; 				  		//цвет
    sf::Vector2f position;			 		//позиция на отображение
    sf::Built triangle;		   		   		//построение
public:
    Triangle(								//контруктор 
        std::vector<sf::Vector2f> vertexs,	//вызов векторов
        sf::Color color,					//вызов краски
        bool TurnOver = false				//вызов булевой переменной для переворота треугольников
    );

   vector<sf::Vector2f> getVertex();	//вершина
   sf::Built draw();					//построение
};