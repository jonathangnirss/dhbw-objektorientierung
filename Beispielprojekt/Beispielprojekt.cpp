#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include<iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;

//Variablen*******************************************************
	bool left = 0, right = 0, up = 0, down = 0, jump = 0;
	int xq = 0, yq = 0, bq = 50;  // Quader: xq,yq Koordinaten, bq Seitenlänge
//***************************************************************


//Functions*******************************************************
void decrementy(int& y) {
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
}
//****************************************************************



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
		graphics().draw_line(
			10, 20, Gosu::Color::RED,
			200, 100, Gosu::Color::GREEN,
			0.0
		);
		graphics().draw_quad(
			xq, yq, Gosu::Color::RED,
			xq, yq+bq, Gosu::Color::RED,
			xq+bq, yq, Gosu::Color::RED,
			xq+bq, yq+bq, Gosu::Color::RED,
			0.0
		);
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		down = input().down(Gosu::KB_DOWN);
		up = input().down(Gosu::KB_UP);
		left = input().down(Gosu::KB_LEFT);
		right = input().down(Gosu::KB_RIGHT);
		jump = input().down(Gosu::KB_SPACE);
		if (down) { incrementy(yq); std::cout << yq << std::endl; }
		if (up) { decrementy(yq); }
		if (left) { decrementx(xq); std::cout << xq << std::endl;}
		if (right) { incrementx(xq); }
/*		if (jump) {
			for (size_t i = 0; i < 20; i = i + 1) {
				yq = yq - 2.5;
			}
		}*/
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}


