#pragma once

#include "ColorRandomizer.h"
#include <vector>


class Board
{
	//struktura reprezentujaca spadajacy klocek z informacja w ktorej jest kolumnie
	struct FallingBlock {
		sf::RectangleShape* block;
		int column;
	};

	//dane o spadajacych klockach
	struct FallingBlocks {
		//spadajace klocki
		std::vector<FallingBlock> blocks;
		//minimalny indeks kolumny spoœród wsyzstkich spadajacych klockow
		int minCol;
		//maksymalny indeks kolumny spoœród wsyzstkich spadajacych klockow
		int maxCol;
	};

	//odwolanie do okienka gry, potrzebne do pobierania rozmiaru
	sf::RenderWindow* _window;
	//wektor 2-wym jako siatka zawierajaca ulozone klocki
	std::vector<std::vector<sf::RectangleShape*>> _data;
	//instancja struktury danych o spadajacych kockach
	FallingBlocks _falling;
	//bazowa predkosc spadania
	sf::Vector2f _defaultFallSpeed;
	//aktualna predkosc spadania (zmieniana np przy przyspieszeniu strzalka w dol)
	sf::Vector2f _fallSpeed;
	//rozmiar klocka podrecznie do oliczen geometrycznych
	sf::Vector2f _blockSize;
	//punkty
	unsigned _score;
	//ilosc rzedow
	size_t _rows;
	//ilosc kolumn
	size_t _cols;
	//aktualny ksztalt spadajacych klockow
	int _blockType;
	//nastepny wylosowany ksztalt
	int _nextBlockType;	
	//obiekt losujacy kolory
	ColorRandomizer _colors;
	//flaga ustawiana do zwrocenia ostatecznej informacji czy przegralismy
	bool _gameOver;
	//ksztalt szarego paska na dole ekranu
	sf::RectangleShape _bottomBar;
	//podglad nastepnego wylosowanego ksztaltu
	sf::RectangleShape _nextShape;
	//pole tekstowwe z info o punktach
	sf::Text _scoreText;
	//zaladowane obrazki pogladowe ladowane do _nextShape
	sf::Texture _images[7];
	
	//inicjalizacja podstawowych info o rozmiarze i alokacja siatki _data
	void init(size_t w, size_t h);
	//przemiesc geometrycznie wszystkie spadajace klocki w dol
	void moveDown(float time);
	//przemiesc w dol i sprawdzaj czy nie spadly na podloze albo juz ulozone klocki
	void fall(float time);
	//sprawdz wszystkie rzedy czy sa pelne i jesli tak to usun rzad
	void checkRows();
	//true jesli rzad o podanym indeksie jest pelny
	bool rowFull(size_t row);
	//usuwanie rzedu o podanym indeksie
	void removeRow(size_t row);
	//wygeneruj kolejny klocek na gorze planszy i wylosuj nastepny
	void processGenerate();
	//stworz elementy odpowiadajace ksztaltowi kwadratu
	void createBlock_O(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi L
	void createBlock_L1(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi L odbicie lustrzane
	void createBlock_L2(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi S
	void createBlock_S1(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi S odbicie lustrzane
	void createBlock_S2(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi linii
	void createBlock_I(sf::Color const& color);
	//stworz elementy odpowiadajace ksztaltowi T
	void createBlock_T(sf::Color const& color);
	//obroc wszystkie elementy wokol srodkowego okreslonego jako ten stojacy na indeksie 'idx' w _falling.blocks
	void rotateFallingRelativeTo(size_t idx);
	//stworz element klocka na kolumnie 'col' i w odleglosci 'topdist' poza gorna krawedzia ekranu, przypisz kolor
	void createBlock(int col, int topdist, sf::Color color);
	//znalezienie najwyzej wysunietego elementu w kolumnie, ale ponizej spadajacych
	sf::RectangleShape* lastInColumn(size_t col);
	//przesun o okreslona ilosc kolumn w lewo prawo spadajace bloki (strzalki lewo/prawo)
	void moveFallingHorizontally(FallingBlocks& falling, int columnOffset);
	//obroc spadajace bloki (strzalka w gore)
	void rotateFalling();
	//przyspiesz spadanie (strzalka w dol)
	void speedUp();
	//uzywane przy inicjalizacji, obroc losowo spadajace bloki
	void randomRotateFalling();
	//wylicz kolumny i rzedy dla klockow spadajacych na podstawie ich pozycji i umiesc w siatce
	void putFallingToGrid();
	//funkcja robiaca wszystko co trzeba ze spadajacymi klockami po dotknieciu podlogi albo innego klocka
	void onFallen();
	//zaokraglenie geometrycznych pozycji spadajacych
	void snapFallingPositions();
	//obroc konkretny klocek w odniesieniu do pozycji klocka centralnego w ksztalcie
	void rotateBlock(FallingBlock& b, sf::Vector2f const& origin);
	//wylicz indeks kolumny na podstawie pozycji
	int columnByPosition(sf::Vector2f const& pos);
	//wylicz indeks kolumny na podstawie pozycji
	int rowByPosition(sf::Vector2f const& pos);
	//policz nowe wartosci minCol maxCol po obrocie spadajacych klockow
	void updateFallingMinMaxColumns(FallingBlocks& falling);
	//odsun od sciany spadajace klocki jesli po obrocie wyszlyby poza sciane
	void moveFallingFromWall(FallingBlocks& falling);
	//wylosuj numer bloku
	int randomBlockType();
	//zachowaj nastepny numer bloku jako aktuany i wylosuj kolejny
	void nextBlockType();
	//sprawdz czy wszystkie klocki z danej kolumny sa powyzej podanego
	bool allFallingOver(sf::RectangleShape* r, int col);
	//zaokraglenie liczby z podana precyzja
	static float roundTo(float value, float precision);
	//zaokraglenie wspolrzednych z podana precyzja
	static sf::Vector2f roundTo(sf::Vector2f v, float precision);
public:
	//konstruktor - inicjalizacja planszy
	Board(sf::RenderWindow* window, size_t w, size_t h, unsigned blockSize);
	//destruktor - dealokacja zasobow
	virtual ~Board();
	//wywolywane przy wykryciu klikniecia klawisza w Application, przetwarzanie reakcji na przycisk
	void onKeyEvent(sf::Event::KeyEvent event);
	//narysowanie calosci planszy na okienku
	void draw(sf::RenderWindow& window);
	//przetwarzanie calej logiki planszy
	bool process();
	//wyczyszczenie, reset planszy
	void clear();
	//ustawienie poczatkowej predkosci spadania
	void setFallSpeed(float value);
	//pobranie informacji o punktach
	unsigned score();
};

