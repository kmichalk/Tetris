#pragma once

#include "SFML/Graphics.hpp"

class Application;

//klasa reprezentuj�ca przycisk
class Button: public sf::RectangleShape
{
	//tekst na przycisktu
	sf::Text _text;
	//flaga czy by� klikni�ty przy poprzednim sprawdzeniu
	bool _pressed;
	//askaxnik na aplikacje w ktorej jest dodany
	Application* _app;
public:
	//konstruktor - inicjalizacja
	Button(sf::Vector2f pos, sf::Vector2f size, std::string const& text);
	//przetworzenie informacji o myszy i sprawdzenie czy button zosta� klikni�ty
	bool clicked();
	//rysowanie przycisku na oknie
	void draw(sf::RenderTarget& target) const;
	//ustawienie wskaznika na aplikacje
	void setParentApp(Application* app);
};

