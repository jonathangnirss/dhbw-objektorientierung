#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include<iostream>
#include <chrono>
#include <thread>
#include <vector>

using std::this_thread::sleep_for;
	int zufallszahl(int min, int max) {
		return (rand() % (max - min) + min);
	}
//Variablen*******************************************************
	bool left = 0, right = 0, up = 0, down = 0, jump = 0;
	std::vector<std::vector<int>> hindernismatrix(4, std::vector<int>(20));

	int xq = 275, yq = 50, bq = 50;  // Abmessungen Spielfigur
	int anz, zfz, score=0;
	int counter = 0;
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
	
	Gosu::Color get_color() { return this->color; }

	void aendere_y(int a) { this->y = this->y + a; }
	void decrementy(int& y) {
		if (y > 0) { y = y - 5; }
	}
	void incrementy(int& y) {
		if (y < 1050 - 2 * bq) { y = y + 5; }
	}
	void decrementx(int& x) {
		if (x > 0) { x = x - 2; }
	}
	void incrementx(int& x) {
		if (x < (650 - 2 * bq)) { x = x + 1; }
	}
};
class Hindernis : public Block
{
private:
	
public:	
	Hindernis(int x, int y, int breite, int hoehe, Gosu::Color color) : Block(x, y, breite, hoehe, color) {}	
};
std::vector<Hindernis> globale_hindernisliste;


class Spielfigur : public Block
{
private:
	std::vector<Hindernis> hindernisliste;
public:
	Spielfigur(int x, int y, int breite, int hoehe, Gosu::Color color) : Block(x, y, breite, hoehe, color) {}
	bool quader_kollision_links(Hindernis h)	//Spielfigur eckt nach links an
	{
		if (((this->get_y() <= h.get_y()) && (h.get_y() < (this->get_y() + this->get_hoehe()))) || ((h.get_y() < this->get_y() && (this->get_y() < (h.get_y() + h.get_hoehe())))))
		{
			return this->get_x() == (h.get_x() + h.get_breite());
		}
		else
		{
			return false;
		}
	}
	bool quader_kollision_rechts(Hindernis h)	//Spielfigur eckt nach rechts an
	{
		if (((this->get_y() <= h.get_y()) && (h.get_y() < (this->get_y() + this->get_hoehe()))) || ((h.get_y() < this->get_y() && (this->get_y() < (h.get_y() + h.get_hoehe())))))
		{
			return (this->get_x() + this->get_breite()) == h.get_x();
		}
		else
		{
			return false;
		}
	}
	bool quader_kollision_oben(Hindernis h)
	{
		if (((this->get_x() <= h.get_x()) && (h.get_x() < (this->get_x() + this->get_breite()))) || ((h.get_x() < this->get_x() && this->get_x() < h.get_x() + h.get_breite())))
		{
			return this->get_y() == (h.get_y() + h.get_hoehe());
		}
		{
			return false;
		}
	}
	bool quader_kollision_unten(Hindernis h)
	{
		if (((this->get_x() <= h.get_x()) && (h.get_x() < (this->get_x() + this->get_breite()))) || ((h.get_x() < this->get_x() && this->get_x() < h.get_x() + h.get_breite())))
		{
			return (this->get_y() + this->get_hoehe()) == h.get_y();
		}
		{
			return false;
		}
	}
	bool fallen() {
		bool hindernis_unten = 0;
		for (Hindernis elem : hindernisliste)
		{
			if (quader_kollision_unten(elem))
			{
				hindernis_unten = 1;
			}
		}
		if (hindernis_unten == 0) {
			return true;
		}
		else { 
			return false; 
		}
	}


	void aktualisiere_hindernisliste() {

	/*	Hindernis h(xh, yh, bh, hh, Gosu::Color::BLUE);		//Erstellen des langen blauen Balkens
		this->hindernisliste.push_back(h); */

		// Koordinaten für Hindernisse aus Hindernissmatrix in Hindernisliste schreiben
		for (size_t s = 0; s < (hindernismatrix.at(0)).size(); s = s + 1) {
			this->hindernisliste.push_back(Hindernis(hindernismatrix.at(0).at(s), hindernismatrix.at(1).at(s), hindernismatrix.at(2).at(s), hindernismatrix.at(3).at(s), Gosu::Color::BLUE));
		}

		globale_hindernisliste = hindernisliste;
	}

	void update()		//Wird dann 60 mal pro Sekunde aufgerufen
	{
		left = Gosu::Input::down(Gosu::KB_LEFT);		//Einlesen der Tasten
		right = Gosu::Input::down(Gosu::KB_RIGHT);
		up = Gosu::Input::down(Gosu::KB_UP);
		down = Gosu::Input::down(Gosu::KB_DOWN);
		jump = Gosu::Input::down(Gosu::KB_SPACE);

		aktualisiere_hindernisliste();		
		if (left)		//Wenn links gedrückt ist, prüfe für jedes Hindernis in der Hindernis-Liste, ob nach links eine Kollision stattfindet, wenn nein, verringere x.
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_links(elem))
				{
					left = false;
				}
			}
			if (left) 
			{ 
			decrementx(xq);
			std::cout << "x: " << xq << " y: " << yq << std::endl; 
			}
		}
		if (right)
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_rechts(elem))
				{
					right = false;
				}
				if(right) { incrementx(xq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
			}
		}
		if (up)
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_oben(elem))
				{
					up = false;
				}
				if (up) { decrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
			}
		}
		if (down)
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_unten(elem))
				{
					down = false;
				}
				if (down) { incrementy(yq); std::cout << "x: " << xq << " y: " << yq << std::endl; }
			}
		}
		if (fallen()) {
			if (yq < 1050 - 2 * bq) { yq = yq + 1; }
		
		}
		if (jump)
		{
			for (Hindernis elem : hindernisliste)
			{
				if (quader_kollision_unten(elem))
				{
					yq = yq - 150;
				}
			}
			if (this->get_y()+this->get_hoehe()>999)
			{
				yq = yq - 150;
			}
		}
	}
};
//*****************************************************************

//Functions*******************************************************

class GameWindow : public Gosu::Window
{
public:
	GameWindow()
		: Window(600, 1000)
	{
		set_caption("Hell Jumper");
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

		//Zeichne Hindernisliste
		for (Hindernis elem : globale_hindernisliste)
		{
			graphics().draw_quad(
				elem.get_x(), elem.get_y(), elem.get_color(),
				elem.get_x(), elem.get_y() + elem.get_hoehe(), elem.get_color(),
				elem.get_x() + elem.get_breite(), elem.get_y(), elem.get_color(),
				elem.get_x() + elem.get_breite(), elem.get_y() + elem.get_hoehe(), elem.get_color(),
				0.0
			);
		}

/*		for (size_t s = 0; s < (hindernismatrix.at(0)).size(); s = s + 1) {
			graphics().draw_quad(
				hindernismatrix.at(0).at(s), hindernismatrix.at(1).at(s), Gosu::Color::BLUE,
				hindernismatrix.at(0).at(s), hindernismatrix.at(1).at(s) + hindernismatrix.at(3).at(s), Gosu::Color::BLUE,
				hindernismatrix.at(0).at(s) + hindernismatrix.at(2).at(s), hindernismatrix.at(1).at(s), Gosu::Color::BLUE,
				hindernismatrix.at(0).at(s) + hindernismatrix.at(2).at(s), hindernismatrix.at(1).at(s) + hindernismatrix.at(3).at(s), Gosu::Color::BLUE,
				0.0
			); 
		}*/ // Zum Testen direkt aus Matrix zeichnen



	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{	


		
		Spielfigur sf(xq, yq, bq, bq, Gosu::Color::RED);		
		sf.update();

		//Bewegung der Hindernisse*************************************************
		if (counter % 2 == 0) {
			for (size_t s = 0; s < (hindernismatrix.at(0)).size(); s = s + 1) {
				hindernismatrix.at(1).at(s) = hindernismatrix.at(1).at(s) + 1;
			} 
		}
		for (size_t s = 0; s < (hindernismatrix.at(0)).size(); s = s + 1) { // Zurücksetzen der Hindenisse
			if (hindernismatrix.at(1).at(s) > 1000) {
				hindernismatrix.at(1).at(s) = 0 - zufallszahl(0,30); // Hindernissabstand verändern
				hindernismatrix.at(0).at(s) = zufallszahl(0, 575);
			}
		}
		//*********************************************************************
		counter = counter + 1;
		// Couter zurücksetzen und Score hochzählen
		if (counter > 600) { 
			counter = 0;
			score = score + 1;
		}
		//std::cout<< zufallszahl(0,250) << std::endl;
		if (yq > 949) {   // Hier Abbruch einfügen, wenn Spielfigur auf dem Boden angekommen ist.
			std::cout << "Game end. Score:" << score << std::endl;
			exit(1);  
		}

	}
};

// C++ Hauptprogramm
int main()
{	// Hindernissmatrix erzeugen
	for (size_t z = 0; z < hindernismatrix.size(); z = z + 1) {
		for (size_t s = 0; s < (hindernismatrix.at(0)).size(); s = s + 1) {
			if (z == 0){
				hindernismatrix.at(z).at(s) = zufallszahl(0, 550);    // x-Koordinate
			}
				if (z == 1) {
					hindernismatrix.at(z).at(s) = 50*s+zufallszahl(25, 75);  //y-Koordinate
				}
			if (z == 2) {
				hindernismatrix.at(z).at(s) = zufallszahl(50, 300);  // Breite
			}
			if (z == 3) {
				hindernismatrix.at(z).at(s) = 10;  // Höhe
			}
		}
	}
	GameWindow window;
	window.show();

}