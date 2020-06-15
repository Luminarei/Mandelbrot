#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <tchar.h>
#include "Mandelbrot.h"


//Klasa z funkcj¹ wirtualna do tworzenia okien menu
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
	virtual int Run(sf::RenderWindow &window); //Funkcja odpowiadajaca za wyœwietlanie okien
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

	//Warunek sprawdzaj¹cy czy odpowiednio zosta³ za³adowany font u¿yty w oknie menu, u¿yto do tego strumieni
	if (!Font.loadFromFile("arial.ttf"))
	{
		std::cerr << "Blad podczas ladowania pliku arial.ttf" << std::endl;
		return (-1);
	}

	// Przygotowanie do zaladowania obrazu w menu
	std::string name = "bm.jpg";
	sf::Texture texture1;
	sf::Texture error1;

	//Wyj¹tek do sprawdzania czy obraz zostal prawid³owo za³adowany 
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
		
	
	
	// Przygotowanie oraz odpowiednie ustawienia obu obrazów
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




	// Ustawienie zmiennych obs³uguj¹cych menu
	if (playing)
	{
		okno_pom = okno_max;
	}
	
	//Pêtla rysowania okna menu
	while (Running)
	{
		while (window.pollEvent(Event))
		{
			// Gdy zamkniemy okno zwracamy dan¹ wartoœæ
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			// Czêœæ dziêki której mo¿emy poruszaæ siê po menu w górê i w dó³
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
		// W tym miejscu odpowiednio w jakim wariancie menu siê znajdujemy ten wariant zostaje pokolorowany na czerwono
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
		// Ostateczne wyœwietlenie wszystkiego co zosta³o przygotowane
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

// Definicja przygotowuj¹ca zbiór do wyœwietlenia
int okno_zbioru::Run(sf::RenderWindow &window)
	{

		sf::Event Event;
		bool Running = true;
		Mandelbrot mb;
		bool ZmianaStanu = true;
		// Zmienne do przesuwania siê w lewo i prawo w oknie zbioru
		double offsetX = 0.0; 
		double offsetY = 0.1; 
		// Zmienna odpowiadaj¹ca za przybli¿anie i oddalanie
		double zoom = 0.004; 
		// Przygotowanie pierwszej iteracji zbioru i odpowiednie jej pokolorowanie
		sf::Image image;
		image.create(szerokosc, wysokosc, sf::Color(0, 0, 0));
		sf::Texture texture;
		sf::Sprite sprite;

		// Pêtla rysowania okna zbioru
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
					case sf::Keyboard::Escape: // Zmkniecie przy u¿yciu escape
						window.close();
						break;
					case sf::Keyboard::Equal: // Przybli¿anie
						zoom *= 0.9;
						break;
					case sf::Keyboard::Dash: // Oddalanie
						zoom /= 0.9;
						break;
					case sf::Keyboard::W: // Przesuwanie w góre 
						offsetY -= 10 * zoom;
						break;
					case sf::Keyboard::S: // Przesuwanie w dó³
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
			// Warunek potrzebny do wywo³ywañ kolejnych iteracji funkcji w mandelbrot.h
			if (ZmianaStanu) 
			{
				mb.NadpiszObraz(zoom, offsetX, offsetY, image);
				texture.loadFromImage(image);
				sprite.setTexture(texture);
				ZmianaStanu = false;
			}
		
			window.clear(); // Czyszczenie okna
			// Wyœwietlanie
			window.draw(sprite); 
			window.display();
		
		}
		return -1;
	}




