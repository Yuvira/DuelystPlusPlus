//Include
#include "game.h"

//Game constructor / destructor
Game::Game() {

	//Default units
	player[0].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[1].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[0].unit[0].setPos(0, 2, map);
	player[1].unit[0].setPos(8, 2, map);

	//Misc
	px = 0;
	py = 0;
	map.tile[0][0].select(true);

}
Game::~Game() {}

//Input
void Game::input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { move(0, 4); } //W
	if (asciiVal == 97 || asciiVal == 65) { move(8, 0); }  //A
	if (asciiVal == 115 || asciiVal == 83) { move(0, 1); } //S
	if (asciiVal == 100 || asciiVal == 68) { move(1, 0); } //D

	//Select
	//if (asciiVal == 32) { selection(); }

}

//Update loop
void Game::update() {

}

//Render objects
void Game::render(Renderer& rm) {

	//Tiles
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			rm.render(map.tile[a][b].border);
			rm.render(map.tile[a][b].sprite);
		}
	}

	//Units
	for (int a = 0; a < player[0].unit.size(); ++a) { player[0].unit[a].render(rm); }
	for (int a = 0; a < player[1].unit.size(); ++a) { player[1].unit[a].render(rm); }

}

//Move cursor position
void Game::move(int x, int y) {
	map.tile[px][py].select(false);
	px += x; px %= 9;
	py += y; py %= 5;
	map.tile[px][py].select(true);
}