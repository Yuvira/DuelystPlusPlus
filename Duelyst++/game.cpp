//Include
#include "game.h"

//Game constructor / destructor
Game::Game() {

	//Generate board
	board.createFromFile("resources/board2.txt");
	board.pos.X = 1;
	board.pos.Y = 1;

	//Default units
	player[0].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[1].unit.push_back(*(dynamic_cast<Unit*>(cl.clist[0])));
	player[0].unit[0].setPos(0, 2, map);
	player[1].unit[0].setPos(8, 2, map);

	//Misc
	px = 0;
	py = 0;
	pointer.buffer[0].Char.AsciiChar = '';

}
Game::~Game() {}

//Input
void Game::input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { py += 4; py %= 5; } //W
	if (asciiVal == 97 || asciiVal == 65) { px += 8; px %= 9; } //A
	if (asciiVal == 115 || asciiVal == 83) { ++py; py %= 5; }    //S
	if (asciiVal == 100 || asciiVal == 68) { ++px; px %= 9; }    //D

	//Select
	//if (asciiVal == 32) { selection(); }

}

//Update loop
void Game::update() {

	//Update pointer pos
	pointer.pos.X = (px * 6) + 4;
	pointer.pos.Y = (py * 6) + 7;

}

//Render objects
void Game::render(Renderer& rm) {

	//Board
	rm.render(board);

	//Tiles
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			rm.render(map.tile[a][b].sprite);
		}
	}

	//Units
	for (int a = 0; a < player[0].unit.size(); ++a) { player[0].unit[a].render(rm); }
	for (int a = 0; a < player[1].unit.size(); ++a) { player[1].unit[a].render(rm); }

	//Pointer
	rm.render(pointer);

}