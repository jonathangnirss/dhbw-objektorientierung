#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include<iostream>
#include <chrono>
#include <thread>
#include <vector>

using std::this_thread::sleep_for;

//Variablen*******************************************************
	bool left = 0, right = 0, up = 0, down = 0, jump = 0;
	int xq = 0, yq = 0, bq = 50;  // Quader: xq,yq Koordinaten, bq Seitenlänge
//***************************************************************

//Klassen**********************************************************
class Block				//Elternklasse für alle Objekte
{
private:
	int x, y, breite, hoehe;
	Gosu::Color color;
public:
	Block(int x, int y, int breite, int hoehe, Gosu::Color color) : x(x), y(y), breite(breite), hoehe(hoehe), color(color) {}
	int get_x() { return this->x; }
	int get_y() { return this->y; }
	int get_hoehe() { return this->hoehe; }
	int get_breite() { return this->breite; }

	void decrementy(int& y) {
		if (y > 0) { y = y - 5; }
	}
	void incrementy(int& y) {
		if (y < 650 - 2 * bq) { y = y + 5; }
	}
	void decrementx(int& x) {
		if (x > 0) { x = x - 5; }
	}
	void incrementx(int& x) {
		if (x < (850 - 2 * bq)) { x = x + 5; }
	}
};
class Hindernis : public Block
{
public:
	Hindernis(int x, int y, int breite, int hoehe, Gosu::Color color) : Block(x, y, breite, hoehe, color) {}
};
class Spielfigur : public Block
{
private:

public:
	std::vector<Hindernis> hindernisliste;
	Spielfigur(int x, int y, int breite, int hoehe, Gosu::Color color) : Block(x, y, breite, hoehe, color) {}
	bool quader_kollision_links(Hindernis h)	//Spielfigur eckt nach links an
	{
		if (((this->get_y() <= h.get_y()) && (h.get_y() < (this->get_y() + this->get_hoehe()))) || ((h.get_y() < this->get_y() && (this->get_y() < (h.get_y() + h.get_hoehe())))))
		{
			return this->get_x() <= (h.get_x() + h.get_breite());
		}
		else
		{
			return false;
		}
	}
	void update()		//Wird dann 60 mal pro Sekunde aufgerufen
	{
		left = Gosu::Input::down(Gosu::KB_LEFT);		//Einlesen der Tasten
		right = Gosu::Input::down(Gosu::KB_RIGHT);
		up = Gosu::Input::down(Gosu::KB_UP);
		down = Gosu::Input::down(Gosu::KB_DOWN);

		Hindernis h(200, 150, 300, 30, Gosu::Color::BLUE);		//Erstellen des langen blauen Balkens
		this->hindernisliste.push_back(h);

		if (left)		//Wenn links gedrückt ist, prüfe für jedes Hindernis in der Hindernis-Liste, ob nach links eine Kollision stattfindet, wenn nein, verringere x.
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_links(elem))
				{
					left = false;
				}
			}
			if (left) { decrementx(xq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
		}
		if (down) { incrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
		if (up) { decrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
		if (right) { incrementx(xq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
	}
};
//*****************************************************************

//Functions*******************************************************
/*void decrementy(int& y) {
	if (y > 0) { y = y - 5; }
}
void incrementy(int& y) {
	if (y < 650 - 2*bq) { y = y + 5; }
}
void decrementx(int& x) {
	if (x > 0) { x = x - 5; }
}
void incrementx(int& x) {
	if (x < (850 - 2*bq)) { x = x + 5; }
}*/


class GameWindow : public Gosu::Window
{
public:
	GameWindow()
		: Window(800, 600)
	{
		set_caption("Gosu Tutorial mit Git");
	}

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		graphics().draw_quad(
			xq, yq, Gosu::Color::RED,
			xq, yq+bq, Gosu::Color::RED,
			xq+bq, yq, Gosu::Color::RED,
			xq+bq, yq+bq, Gosu::Color::RED,
			0.0
		);
		//Blauer langer Balken
		graphics().draw_quad(
			200, 150, Gosu::Color::BLUE,
			200, 180, Gosu::Color::BLUE,
			500, 150, Gosu::Color::BLUE,
			500, 180, Gosu::Color::BLUE,
			0.0
		);
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		/*down = input().down(Gosu::KB_DOWN);
		up = input().down(Gosu::KB_UP);
		left = input().down(Gosu::KB_LEFT);
		right = input().down(Gosu::KB_RIGHT);
		jump = input().down(Gosu::KB_SPACE);'/
		//if (down) { incrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
		//if (up) { decrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
		//if (left) { decrementx(xq); std::cout << "x: " << xq << " y: " << yq << std::endl;}
		//if (right) { incrementx(xq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
/*		if (jump) {
			for (size_t i = 0; i < 20; i = i + 1) {
				yq = yq - 2.5;
			}
		}*/

		Spielfigur sf(xq, yq, bq, bq, Gosu::Color::RED);		
		sf.update();
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}