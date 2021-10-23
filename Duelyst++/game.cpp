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
	moveCursor(0, 0);
	selected = nullptr;

}
Game::~Game() {}

//Input
void Game::input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { moveCursor(0, 4); } //W
	if (asciiVal == 97 || asciiVal == 65) { moveCursor(8, 0); }  //A
	if (asciiVal == 115 || asciiVal == 83) { moveCursor(0, 1); } //S
	if (asciiVal == 100 || asciiVal == 68) { moveCursor(1, 0); } //D

	//Select (move unit right)
	if (asciiVal == 32) {
		if (map.tile[px][py].unit != nullptr) {
			if (map.tile[px][py].unit->player == &player[turn]) {
				map.tile[px][py].unit->setPos(px + 1, py, map);
				moveCursor(0, 0);
			}
		}
	}

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
void Game::moveCursor(int x, int y) {

	//Clear selected tiles
	for (int a = 0; a < highlighted.size(); ++a) { highlighted[a]->setCol(COLOR_LTWHITE); }
	highlighted.clear();
	
	//Move cursor
	px += x; px %= 9;
	py += y; py %= 5;

	//Select tile at cursor
	highlightTile(px, py, COLOR_AQUA);

	//Highlight moveable spaces
	if (map.tile[px][py].unit != nullptr) {
		if (map.tile[px][py].unit->player == &player[turn]) {
			highlightMoveable(px, py);
		}
	}

}

//Check if tile is moveable
bool Game::moveable(int x, int y) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		if (map.tile[x][y].unit == nullptr) {
			return true;
		}
	}
	return false;
}

//Highlight tile
void Game::highlightTile(int x, int y, eColor col) {
	map.tile[x][y].setCol(col);
	highlighted.push_back(&map.tile[x][y]);
}

//Highlight moveable spaces
void Game::highlightMoveable(int x, int y) {
	bool u = false; bool l = false; bool r = false; bool d = false;
	if (moveable(x, y - 1)) { highlightTile(x, y - 1, COLOR_GRAY); u = true; } //UP
	if (moveable(x - 1, y)) { highlightTile(x - 1, y, COLOR_GRAY); l = true; } //LEFT
	if (moveable(x + 1, y)) { highlightTile(x + 1, y, COLOR_GRAY); r = true; } //RIGHT
	if (moveable(x, y + 1)) { highlightTile(x, y + 1, COLOR_GRAY); d = true; } //DOWN
	if (moveable(x, y - 2) && u) { highlightTile(x, y - 2, COLOR_GRAY); } //UPx2
	if (moveable(x - 2, y) && l) { highlightTile(x - 2, y, COLOR_GRAY); } //LEFTx2
	if (moveable(x + 2, y) && r) { highlightTile(x + 2, y, COLOR_GRAY); } //RIGHTx2
	if (moveable(x, y + 2) && d) { highlightTile(x, y + 2, COLOR_GRAY); } //DOWNx2
	if (moveable(x - 1, y - 1) && (u || l)) { highlightTile(x - 1, y - 1, COLOR_GRAY); } //UP-LEFT
	if (moveable(x + 1, y - 1) && (u || r)) { highlightTile(x + 1, y - 1, COLOR_GRAY); } //UP-RIGHT
	if (moveable(x - 1, y + 1) && (d || l)) { highlightTile(x - 1, y + 1, COLOR_GRAY); } //DOWN-LEFT
	if (moveable(x + 1, y + 1) && (d || r)) { highlightTile(x + 1, y + 1, COLOR_GRAY); } //DOWN-RIGHT
}