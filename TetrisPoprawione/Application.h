#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Board.h"
#include "Button.h"

//klasa reprezentujaca aplikacje, glowna klasa gry
class Application {
	//stan aplikacji od ktorego zalezy co jest aktualnie wyswietlane albo przetwarzane w metodzie process
	enum State {
		Init, GameOver, Play
	};
	
	//glowne okienko aplikacji
	sf::RenderWindow _window;
	//obiekt planszy odpowiadajacy za cala logike gry
	Board _board;
	//przycisk play
	Button _playButton;
	//pole tekstowe gorne z napisem Tetris albo Game Over
	sf::Text _headerText;
	//pole tekstowe z informacja o zdobytych punktach po zakonczeniu
	sf::Text _scoreInfoText;
	//stan aktualny
	State _state;
	//obiekt odpowiadajacy za odgrywanie muzyki
	sf::Music _music;

	//wykonywana iteracyjnie metoda przetwarzajaca logike aplikacji
	void process();
	//pobramnie z okna informacji o zdarzeniach tj przyciski, zamkniecie okna i zastosowanie akcji
	void processWindowEvents();
	//przerysowanie wszystkiego
	void redraw();
public:
	Application(std::string const& name);
	~Application();
	//punkt wejscia do programu, zakonczenie tej metody oznacza koniec aplikacji
	void run();
	//pobranie referencji na przechowywane okno
	sf::RenderWindow& window();
};
