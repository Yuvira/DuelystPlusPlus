//Include
#include "game.h"

//Game constructor / destructor
Game::Game() {

	//Default units
	player[0].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[1].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[0].unit[0].setPos(0, 2, map);
	player[1].unit[0].setPos(8, 2, map);
	player[0].unit[0].player = &player[0];
	player[1].unit[0].player = &player[1];
	player[0].unit[0].game = this;
	player[1].unit[0].game = this;

	//Misc
	px = 0;
	py = 0;
	turn = false;
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			map.tile[a][b].setCol(COLOR_LTWHITE);
		}
	}
	move(0, 0);

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

	//Clear selected tiles
	for (int a = 0; a < select.size(); ++a) { select[a]->setCol(COLOR_LTWHITE); }
	select.clear();
	
	//Move cursor
	px += x; px %= 9;
	py += y; py %= 5;

	//Select tile at cursor
	map.tile[px][py].setCol(COLOR_AQUA);
	select.push_back(&map.tile[px][py]);

}