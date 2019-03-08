#pragma once

#include <random>

//pomocnicza klasa utility do losowania
class Random
{
	//obiekt zawierajacy dane o silniku losujacym przechowywany statycznie i inicjalizowany raz na poczatku programu
	class RandomImpl {
	public:
		//narzedzie sluzace do inicjalizacji silnika losowania
		std::random_device rd;
		//rozklad prawdopodobienstwa
		std::uniform_real_distribution<float> distf;
		//silnik losowania
		std::mt19937 engine;

		//konstruktor - inicjalizacja silnika losowania
		RandomImpl();
	};

	//statyczna instancja danych losujacych
	static RandomImpl impl;

public:
	//wylosuj int z przedzialu od 0 do max wartosci inta
	static int nextInt();
	//wylosuj int z przedzialu od 0 do bound
	static int nextInt(int bound);
	//wylosuj int z przedzialu od min do max
	static int nextInt(int min, int max);
	//wylosuj bool (decyzja true/false)
	static bool nextBool();
};

