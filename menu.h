#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <tchar.h>
#include "Mandelbrot.h"


//Klasa z funkcj� wirtualna do tworzenia okien menu
class Menu
{
public:

	virtual int Run(sf::RenderWindow &window) = 0; 
	sf::Font font;

};

// Klasa okna menu
class okno_menu : public Menu
{
private:
	//Zmienne do iteracji po elementach menu i odpowiedniego dzialania menu
	int okno_max; 
	bool playing;
public:
	virtual int Run(sf::RenderWindow &window); //Funkcja odpowiadajaca za wy�wietlanie okien
};



int okno_menu::Run(sf::RenderWindow &window)
{
	sf::Event Event; 
	bool Running = true;
	sf::Texture Texture;
	int okno_pom = 0;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	int menu = 0;

	//Warunek sprawdzaj�cy czy odpowiednio zosta� za�adowany font u�yty w oknie menu, u�yto do tego strumieni
	if (!Font.loadFromFile("arial.ttf"))
	{
		std::cerr << "Blad podczas ladowania pliku arial.ttf" << std::endl;
		return (-1);
	}

	// Przygotowanie do zaladowania obrazu w menu
	std::string name = "bm.jpg";
	sf::Texture texture1;
	sf::Texture error1;

	//Wyj�tek do sprawdzania czy obraz zostal prawid�owo za�adowany 
	try
	{
		texture1.loadFromFile(name);
		throw std::string();
	}
		
	catch (std::string error)
		{
		if (texture1.loadFromFile(name) == false)
		{
			error1.loadFromFile("imag.jpg");
		}
	}
		
	
	
	// Przygotowanie oraz odpowiednie ustawienia obu obraz�w
	sf::Sprite sprite1;
	sf::Sprite error2;

	sprite1.setTexture(texture1);
	error2.setTexture(error1);
	sprite1.setPosition({ 120.f,130.f });
	error2.setPosition({ 220.f,260.f });

	// Ustalenie wygladu, miejsca i napisu w  pierwszym wariancie  menu
	Menu1.setFont(Font);
	Menu1.setCharacterSize(40);
	Menu1.setString("Wyswietl zbior Mandelbrota");
	Menu1.setPosition({ 750.f, 360.f });

	// Ustalenie wygladu, miejsca i napisu w  drugim wariancie  menu
	Menu2.setFont(Font);
	Menu2.setCharacterSize(40);
	Menu2.setString("Wyjscie");
	Menu2.setPosition({ 750.f, 520.f });




	// Ustawienie zmiennych obs�uguj�cych menu
	if (playing)
	{
		okno_pom = okno_max;
	}
	
	//P�tla rysowania okna menu
	while (Running)
	{
		while (window.pollEvent(Event))
		{
			// Gdy zamkniemy okno zwracamy dan� warto��
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			// Cz�� dzi�ki kt�rej mo�emy porusza� si� po menu w g�r� i w d�
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
					menu = 0;
					break;
				case sf::Keyboard::Down:
					menu = 1;
					break;
				case sf::Keyboard::Return:
					if (menu == 0)
					{
						//wybor generatora zbioru mandelbrota
						playing = true;
						return (1);
					}
					else
					{
						//wyjscie z programu
						return (-1);
					}
					break;
				default:
					break;
				}
				
			}
		}

		//Kiedy osiagniemy wartosc okno max to przestajemy edytowac sprite
		if (okno_pom < okno_max)
		{
			okno_pom++;
		}
		// W tym miejscu odpowiednio w jakim wariancie menu si� znajdujemy ten wariant zostaje pokolorowany na czerwono
		if (menu == 0)
		{
			Menu1.setFillColor(sf::Color::Red);
			Menu2.setFillColor(sf::Color::White);
		}
		else
		{
			Menu1.setFillColor(sf::Color::White);
			Menu2.setFillColor(sf::Color::Red);
		}

		
		window.clear(); //Czyszczenie okna
		window.draw(sprite1); //Rysowanie w oknie
		window.draw(error2); // Rysowanie w oknie
		// Ostateczne wy�wietlenie wszystkiego co zosta�o przygotowane
		if (okno_pom == okno_max)
		{
			if (playing)
			{
				return 1;
			}
			else
			{
				window.draw(Menu1);
			}
			window.draw(Menu2);
				
		
		}
		window.display();
	}

	
	return (-1);
}
// Klasa okna zbioru mandelbrota

class okno_zbioru : public Menu
{
public:
	virtual int Run(sf::RenderWindow &window);
};

// Definicja przygotowuj�ca zbi�r do wy�wietlenia
int okno_zbioru::Run(sf::RenderWindow &window)
	{

		sf::Event Event;
		bool Running = true;
		Mandelbrot mb;
		bool ZmianaStanu = true;
		// Zmienne do przesuwania si� w lewo i prawo w oknie zbioru
		double offsetX = 0.0; 
		double offsetY = 0.1; 
		// Zmienna odpowiadaj�ca za przybli�anie i oddalanie
		double zoom = 0.004; 
		// Przygotowanie pierwszej iteracji zbioru i odpowiednie jej pokolorowanie
		sf::Image image;
		image.create(szerokosc, wysokosc, sf::Color(0, 0, 0));
		sf::Texture texture;
		sf::Sprite sprite;

		// P�tla rysowania okna zbioru
		while (Running)  
		{
			while (window.pollEvent(Event)) 
			{
				switch (Event.type) {
				case sf::Event::Closed: 
					window.close();
					break;
				case sf::Event::KeyPressed:
					ZmianaStanu = true;
					switch (Event.key.code) {
					case sf::Keyboard::Escape: // Zmkniecie przy u�yciu escape
						window.close();
						break;
					case sf::Keyboard::Equal: // Przybli�anie
						zoom *= 0.9;
						break;
					case sf::Keyboard::Dash: // Oddalanie
						zoom /= 0.9;
						break;
					case sf::Keyboard::W: // Przesuwanie w g�re 
						offsetY -= 10 * zoom;
						break;
					case sf::Keyboard::S: // Przesuwanie w d�
						offsetY += 10 * zoom;
						break;
					case sf::Keyboard::A: // Przesuwanie w lewo
						offsetX -= 10 * zoom;
						break;
					case sf::Keyboard::D:
						offsetX += 10 * zoom; // Przesuwanie w prawo
						break;
					case sf::Keyboard::M:
						mb.Max_Wywolan += 1;
						break;

					default:
						ZmianaStanu = false; 
						break;
					}
				default:
					break;
				}
			}
			// Warunek potrzebny do wywo�ywa� kolejnych iteracji funkcji w mandelbrot.h
			if (ZmianaStanu) 
			{
				mb.NadpiszObraz(zoom, offsetX, offsetY, image);
				texture.loadFromImage(image);
				sprite.setTexture(texture);
				ZmianaStanu = false;
			}
		
			window.clear(); // Czyszczenie okna
			// Wy�wietlanie
			window.draw(sprite); 
			window.display();
		
		}
		return -1;
	}




