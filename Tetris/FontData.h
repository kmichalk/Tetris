#pragma once

#include "SFML/Graphics.hpp"

//obiekt singleton przechowujacy globalnie dane o czcionce programu
struct FontData {
	//instancja tego obiektu dostepna z kazdego miejsca w programie
	static FontData instance;

	//czcionka przechowywana w obiekcie
	sf::Font font;
	//konstruktor - zaladowanie czcionki z pliku
	FontData();
};