#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>
#include <complex>

//Zmienne do ustalenia wymiarów okna programu, static const sprawia ¿e zmienne s¹ nietykalne.
static const int szerokosc = 1920;
static const int wysokosc = 1080;


//G³ówna klasa zawieraj¹ca metody do tworzenia zbioru mandelbrota
class Mandelbrot {
public:

	Mandelbrot();
	void NadpiszObraz(double zoom, double offsetX, double offsetY, sf::Image& image) const;
	int Max_Wywolan = 1;
	static const int  maxWywolan = 1000;
	~Mandelbrot();
private:

	
	std::array<sf::Color, maxWywolan + 1> colors; //kontener array potrzebny do generowania zbioru w trakcie pracy programu
	sf::Color pokoloruj(int wywolania) const; // funkcja odpowiedzialna za nadawanie kolorów zbiorowi przy kolejnych iteracjach funkcji licz¹cej 
	int mandelbrot(double Real, double Imag) const; //funkcja liczaca
	void OdswiezKawawalek(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY) const; //funkcja do umieszczania wygenerowanych wartosci zbioru w odpowiednim miejscu w oknie programu


};
//funkcja do nadania kolorów zbiorowi, uzale¿niona od kolejnych wywo³añ funckji licz¹cej zbiór mandelbrota
sf::Color Mandelbrot::pokoloruj(int wywolania) const
{
	int r; // czerwony
	int g; // zielony
	int b; // niebieski

	if (wywolania < 16) //od 0-16 nadajemy podany nizej kolor
	{
		r = 16 * (16 - wywolania);
		g = 0;
		b = 16*wywolania-1;
	}
	else if (wywolania < 32) // adekwatnie jak wy¿ej
	{
		r = 0;
		g = 32 * (wywolania-16);
		b = 16 * (16 - wywolania)-1;
	}
	else if (wywolania < 64) // adekwatnie jak wy¿ej
	{
		r = 16 * (wywolania-16);
		g = 16 * (64 - wywolania)-1;
		b = 0;
	}
	else
	{
		r = 255 - (wywolania-64)*4;
		g = 0;
		b = 0;
	}
	return sf::Color(r, g, b); //zwracanie odpowiednich kolorów
};


//Struktura która przesy³a wygenerowane kolory do kontenera z punktami zbioru
Mandelbrot::Mandelbrot() {
	for (int i = 0; i <=maxWywolan; ++i) {
		colors[i] = pokoloruj(i);
	}
}

Mandelbrot::~Mandelbrot() {

}

//Definicja funkcji liczacej 
int Mandelbrot::mandelbrot(double Real, double Imag) const  
{
	
	int i = 0; // Iterator 
	double zReal = 0.0, zImag = 0.0; //Czêœæ rzeczywista i zespolona potrzebna do zaimplementowania rekurencyjnego wzoru generacji zbioru Mandelbrota
	while (i < Max_Wywolan && zReal*zReal + zImag * zImag < 4.0) //Warunek który musi byæ spe³niony by punkt zosta³ uznany za punkt zbioru Mandelbrota
	{
		//implementacja rekurencyjnego wzoru generuj¹cego punkty w przestrzeni gaussowskiej 
		double temp = zReal * zReal - zImag * zImag + Real; 
		zImag = 2.0*zReal*zImag + Imag;
		zReal = temp;
		++i;
		
	}
	return i;
};

//Definicja funkcji która odpowiada za poprawne umieszczenie punktów w oknie programu oraz za odpowiednie przybli¿anie i przesuwanie obrazu
void Mandelbrot::OdswiezKawawalek(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY) const
{
	
	double real = 0 * zoom - szerokosc / 2.0 * zoom + offsetX;
	double imagstart = minY * zoom - wysokosc / 2.0 * zoom + offsetY;
	for (int x = 0; x < szerokosc; x++, real += zoom) {
		double imag = imagstart;
		for (int y = minY; y < maxY; y++, imag += zoom) {
			int value = mandelbrot(real, imag);
			image.setPixel(x, y, colors[value]);
		}
	}
}
//Funkcja pomocnicza dla funkcji do ustawiania punktów w oknie programu 
void Mandelbrot::NadpiszObraz(double zoom, double offsetX, double offsetY, sf::Image& image) const
{
	const int STEP = wysokosc/std::thread::hardware_concurrency(); 
	std::vector<std::thread> threads;
	for (int i = 0; i < wysokosc; i += STEP) {
		threads.push_back(std::thread(&Mandelbrot::OdswiezKawawalek, *this, zoom, offsetX, offsetY, std::ref(image), i, std::min(i + STEP, wysokosc)));
	}
	for (auto &t : threads) {
		t.join();
	}
}





