#pragma once

#include "SFML/Graphics.hpp"

//klasa losujaca kolor. gwarantuje ze nie zostanie wylosowany kolor jesli ostatni albo przedostatni byl taki sam
class ColorRandomizer
{
	//zbior mozliwych do losowania kolorow
	std::vector<sf::Color> _colors;
	//indeks ostatnio wylosowanego
	int _lastIdx;
	//indeks przedostatnio wylosowanego
	int _prelastIdx;
public:
	//konstruktor - ustawienie mozliwych kolorow
	ColorRandomizer(std::vector<sf::Color> const& colors);
	//wylosowanie koloru
	sf::Color get();
};

