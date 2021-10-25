//Include
#include "game.h"

//Game constructor / destructor
Game::Game() {

	//Arrow sprites
	arrow[0].buffer[0].Char.AsciiChar = 'Û';
	arrow[1].buffer[0].Char.AsciiChar = 'Þ';
	arrow[2].buffer[0].Char.AsciiChar = 'Ý';
	arrow[3].buffer[0].Char.AsciiChar = 'Ü';
	arrow[4].buffer[0].Char.AsciiChar = 'ß';

	//Sword sprites
	sword[0].buffer[0].Char.AsciiChar = 'X';
	sword[1].buffer[0].Char.AsciiChar = '\\';
	sword[2].buffer[0].Char.AsciiChar = '/';

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
	pos = Coord(0, 0);
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			map.tile[a][b].setCol(COLOR_LTWHITE);
		}
	}
	changeTurn(false);
	moveCursor(0, 0);
	selected = nullptr;
	attack = nullptr;
	mode = MODE_NONE;

}
Game::~Game() {}

//Input
void Game::input() {

	//Get keyPress
	int asciiVal = _getch();

	//Standard mode
	if (mode == MODE_NONE) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { moveCursor(0, 4); }      //W
		else if (asciiVal == 97 || asciiVal == 65) { moveCursor(8, 0); }  //A
		else if (asciiVal == 115 || asciiVal == 83) { moveCursor(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { moveCursor(1, 0); } //D

		//Select unit
		else if (asciiVal == 32) {
			if (map.tile[pos.x][pos.y].unit != nullptr) {
				if (map.tile[pos.x][pos.y].unit->player == &player[turn]) {
					select(map.tile[pos.x][pos.y]);
				}
			}
		}

		//Change turn
		else if (asciiVal == 10 || asciiVal == 13) { changeTurn(!turn); }

	}

	//Move mode
	else if (mode == MODE_MOVE) {

		//Move arrow
		if (asciiVal == 119 || asciiVal == 87) { moveArrow(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { moveArrow(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { moveArrow(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { moveArrow(1, 0); } //D

		//Move unit
		else if (asciiVal == 32) {
			if (attack != nullptr) { attackUnit(); }
			else { moveUnit(); }
		}

	}

}

//Update loop
void Game::update() {

	//Update units
	for (int a = 0; a < player[0].unit.size(); ++a) { player[0].unit[a].updateStatSprites(); }
	for (int a = 0; a < player[1].unit.size(); ++a) { player[1].unit[a].updateStatSprites(); }

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

	//Attack indicators
	if (attack != nullptr) {
		COORD c = attack->border.pos;
		drawSword(c.X, c.Y, rm);
		c = map.tile[pos.x][pos.y].border.pos;
		if (canAttack(*attack, map.tile[pos.x][pos.y])) {drawSword(c.X, c.Y, rm);}
	}

	//Arrows
	else if (path.size() > 1) { drawPath(rm); }

}

//Change turn
void Game::changeTurn(bool t) {
	turn = t;
	for (int a = 0; a < hostile.size(); ++a) { hostile[a]->setCol(COLOR_LTWHITE); }
	hostile.clear();
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			if (map.tile[a][b].unit != nullptr) {
				if (map.tile[a][b].unit->player != &player[turn]) {
					map.tile[a][b].setCol(COLOR_LTRED);
					hostile.push_back(&map.tile[a][b]);
				}
			}
		}
	}
	moveCursor(0, 0);
}

//Select unit
void Game::select(Tile& t) {
	for (int a = 0; a < highlighted.size(); ++a) {
		highlighted[a]->setCol(COLOR_AQUA);
		moveable.push_back(highlighted[a]);
	}
	for (int a = 0; a < hostile.size(); ++a) {
		if (canAttack(t, *hostile[a])) {
			hostile[a]->setCol(COLOR_RED);
			attackable.push_back(hostile[a]);
		}
	}
	selected = t.unit;
	t.setCol(COLOR_LTWHITE);
	mode = MODE_MOVE;
	path.push_back(pos);
}

//Move selected unit
void Game::moveUnit() {
	int a = path.size() - 1;
	if (a > 0) {
		selected->setPos(path[a].x, path[a].y, map);
		pos = path[a];
	}
	moveable.clear();
	attackable.clear();
	selected = nullptr;
	mode = MODE_NONE;
	path.clear();
	moveCursor(0, 0);
}

//Attack unit
void Game::attackUnit() {
	selected->attack(*attack->unit);
	if (canAttack(*attack, map.tile[pos.x][pos.y])) { attack->unit->attack(*selected); }
	moveable.clear();
	attackable.clear();
	selected = nullptr;
	attack = nullptr;
	mode = MODE_NONE;
	path.clear();
	moveCursor(0, 0);
}

//Move cursor position
void Game::moveCursor(int x, int y) {

	//Clear selected tiles
	for (int a = 0; a < highlighted.size(); ++a) { highlighted[a]->setCol(COLOR_LTWHITE); }
	highlighted.clear();
	
	//Move cursor
	pos.x += x; pos.x %= 9;
	pos.y += y; pos.y %= 5;

	//Re-highlight enemies in red
	for (int a = 0; a < hostile.size(); ++a) { hostile[a]->setCol(COLOR_LTRED); }

	//Select tile at cursor
	highlightTile(pos.x, pos.y, COLOR_AQUA);

	//Highlight moveable spaces
	if (map.tile[pos.x][pos.y].unit != nullptr) {
		if (map.tile[pos.x][pos.y].unit->player == &player[turn]) {
			highlightMoveable(pos.x, pos.y);
		}
	}

}

//Move movement arrow
void Game::moveArrow(int x, int y) {

	//Get last arrow path position
	Coord p;
	if (attack == nullptr) { p = path[path.size() - 1]; }
	else { p = attack->pos; }

	//Return if back to start
	if (p.x + x == pos.x && p.y + y == pos.y) {
		path.clear();
		path.push_back(pos);
		attack = nullptr;
		return;
	}

	//Check if tile is attackable
	for (int a = 0; a < attackable.size(); ++a) {
		if (&map.tile[p.x + x][p.y + y] == attackable[a]) {
			path.clear();
			path.push_back(pos);
			attack = &map.tile[p.x + x][p.y + y];
		}
	}

	//Check if can move to tile
	for (int a = 0; a < moveable.size(); ++a) {
		if (&map.tile[p.x + x][p.y + y] == moveable[a]) { break; }
		if (a == moveable.size() - 1) { return; }
	}

	//Add position, remove intermediates if past range
	path.push_back(Coord(p.x + x, p.y + y));
	if (attack == nullptr && path.size() == 4) { path.erase(path.begin() + 1, path.begin() + 3); }
	attack = nullptr;

}

//Check if tile is moveable
bool Game::canMove(int x, int y) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		if (map.tile[x][y].unit == nullptr) {
			return true;
		}
	}
	return false;
}

//Check if unit can attack position
bool Game::canAttack(Tile& t1, Tile& t2) {
	if (abs(t1.pos.x - t2.pos.x) < 2 && abs(t1.pos.y - t2.pos.y) < 2) {
		return true;
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
	if (canMove(x, y - 1)) { highlightTile(x, y - 1, COLOR_GRAY); u = true; } //Up
	if (canMove(x - 1, y)) { highlightTile(x - 1, y, COLOR_GRAY); l = true; } //Left
	if (canMove(x, y + 1)) { highlightTile(x, y + 1, COLOR_GRAY); d = true; } //Down
	if (canMove(x + 1, y)) { highlightTile(x + 1, y, COLOR_GRAY); r = true; } //Right
	if (canMove(x, y - 2) && u) { highlightTile(x, y - 2, COLOR_GRAY); } //Up x 2
	if (canMove(x - 2, y) && l) { highlightTile(x - 2, y, COLOR_GRAY); } //Left x 2
	if (canMove(x, y + 2) && d) { highlightTile(x, y + 2, COLOR_GRAY); } //Down x 2
	if (canMove(x + 2, y) && r) { highlightTile(x + 2, y, COLOR_GRAY); } //Right x 2
	if (canMove(x - 1, y - 1) && (u || l)) { highlightTile(x - 1, y - 1, COLOR_GRAY); } //Up-left
	if (canMove(x - 1, y + 1) && (d || l)) { highlightTile(x - 1, y + 1, COLOR_GRAY); } //Down-left
	if (canMove(x + 1, y + 1) && (d || r)) { highlightTile(x + 1, y + 1, COLOR_GRAY); } //Down-right
	if (canMove(x + 1, y - 1) && (u || r)) { highlightTile(x + 1, y - 1, COLOR_GRAY); } //Up-right
}

//Draw arrow path
void Game::drawPath(Renderer& rm) {
	int a = path.size() - 1;
	COORD c = map.tile[path[0].x][path[0].y].border.pos;
	if (path[0].y > path[1].y) { drawArrow(1, c.X, c.Y, rm); }      //Start up
	else if (path[0].x > path[1].x) { drawArrow(2, c.X, c.Y, rm); } //Start left
	else if (path[0].y < path[1].y) { drawArrow(3, c.X, c.Y, rm); } //Start down
	else if (path[0].x < path[1].x) { drawArrow(4, c.X, c.Y, rm); } //Start right
	c = map.tile[path[a].x][path[a].y].border.pos;
	if (path[a].y < path[a - 1].y) { drawArrow(5, c.X, c.Y, rm); }      //End up
	else if (path[a].x < path[a - 1].x) { drawArrow(6, c.X, c.Y, rm); } //End left
	else if (path[a].y > path[a - 1].y) { drawArrow(7, c.X, c.Y, rm); } //End down
	else if (path[a].x > path[a - 1].x) { drawArrow(8, c.X, c.Y, rm); } //End right
	if (path.size() == 3) {
		c = map.tile[path[1].x][path[1].y].border.pos;
		if (path[1].x == path[0].x && path[1].x == path[2].x) { drawArrow(9, c.X, c.Y, rm); }       //Up-down
		else if (path[1].y == path[0].y && path[1].y == path[2].y) { drawArrow(10, c.X, c.Y, rm); } //Left-right
		else if ((path[1].x > path[0].x && path[1].y > path[2].y) || (path[1].x > path[2].x && path[1].y > path[0].y)) { drawArrow(11, c.X, c.Y, rm); } //Up-left
		else if ((path[1].x > path[0].x && path[1].y < path[2].y) || (path[1].x > path[2].x && path[1].y < path[0].y)) { drawArrow(12, c.X, c.Y, rm); } //Down-left
		else if ((path[1].x < path[0].x && path[1].y < path[2].y) || (path[1].x < path[2].x && path[1].y < path[0].y)) { drawArrow(13, c.X, c.Y, rm); } //Down-right
		else if ((path[1].x < path[0].x && path[1].y > path[2].y) || (path[1].x < path[2].x && path[1].y > path[0].y)) { drawArrow(14, c.X, c.Y, rm); } //Up-right
	}
}

//Draw attack swords
void Game::drawSword(int x, int y, Renderer& rm) {
	rm.render(sword[1], x + 2, y + 2);
	rm.render(sword[2], x + 4, y + 2);
	rm.render(sword[0], x + 3, y + 3);
	rm.render(sword[0], x + 2, y + 4);
	rm.render(sword[0], x + 4, y + 4);
}

//Draw arrows
void Game::drawArrow(int a, int x, int y, Renderer& rm) {
	if (a == 1) { //Start up
		rm.render(arrow[0], x + 3, y + 1);
		rm.render(arrow[0], x + 3, y);
	}
	else if (a == 2) { // Start left
		rm.render(arrow[0], x + 1, y + 3);
		rm.render(arrow[0], x, y + 3);
	}
	else if (a == 3) { //Start down
		rm.render(arrow[0], x + 3, y + 5);
		rm.render(arrow[0], x + 3, y + 6);
	}
	else if (a == 4) { //Start right
		rm.render(arrow[0], x + 5, y + 3);
		rm.render(arrow[0], x + 6, y + 3);
	}
	else if (a == 5) { //End up
		rm.render(arrow[0], x + 3, y + 6);
		rm.render(arrow[0], x + 2, y + 5);
		rm.render(arrow[0], x + 3, y + 5);
		rm.render(arrow[0], x + 4, y + 5);
		rm.render(arrow[1], x + 2, y + 4);
		rm.render(arrow[0], x + 3, y + 4);
		rm.render(arrow[2], x + 4, y + 4);
		rm.render(arrow[0], x + 3, y + 3);
	}
	else if (a == 6) { //End left
		rm.render(arrow[0], x + 6, y + 3);
		rm.render(arrow[0], x + 5, y + 2);
		rm.render(arrow[0], x + 5, y + 3);
		rm.render(arrow[0], x + 5, y + 4);
		rm.render(arrow[3], x + 4, y + 2);
		rm.render(arrow[0], x + 4, y + 3);
		rm.render(arrow[4], x + 4, y + 4);
		rm.render(arrow[0], x + 3, y + 3);
	}
	else if (a == 7) { //End down
		rm.render(arrow[0], x + 3, y);
		rm.render(arrow[0], x + 2, y + 1);
		rm.render(arrow[0], x + 3, y + 1);
		rm.render(arrow[0], x + 4, y + 1);
		rm.render(arrow[1], x + 2, y + 2);
		rm.render(arrow[0], x + 3, y + 2);
		rm.render(arrow[2], x + 4, y + 2);
		rm.render(arrow[0], x + 3, y + 3);
	}
	else if (a == 8) { //End right
		rm.render(arrow[0], x, y + 3);
		rm.render(arrow[0], x + 1, y + 2);
		rm.render(arrow[0], x + 1, y + 3);
		rm.render(arrow[0], x + 1, y + 4);
		rm.render(arrow[3], x + 2, y + 2);
		rm.render(arrow[0], x + 2, y + 3);
		rm.render(arrow[4], x + 2, y + 4);
		rm.render(arrow[0], x + 3, y + 3);
	}
	else if (a == 9) { //Up-down
		rm.render(arrow[0], x + 3, y);
		rm.render(arrow[0], x + 3, y + 1);
		rm.render(arrow[0], x + 3, y + 2);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 3, y + 4);
		rm.render(arrow[0], x + 3, y + 5);
		rm.render(arrow[0], x + 3, y + 6);
	}
	else if (a == 10) { //Left-right
		rm.render(arrow[0], x, y + 3);
		rm.render(arrow[0], x + 1, y + 3);
		rm.render(arrow[0], x + 2, y + 3);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 4, y + 3);
		rm.render(arrow[0], x + 5, y + 3);
		rm.render(arrow[0], x + 6, y + 3);
	}
	else if (a == 11) { //Up-left
		rm.render(arrow[0], x, y + 3);
		rm.render(arrow[0], x + 1, y + 3);
		rm.render(arrow[0], x + 2, y + 3);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 3, y + 2);
		rm.render(arrow[0], x + 3, y + 1);
		rm.render(arrow[0], x + 3, y);
	}
	else if (a == 12) { //Down-left
		rm.render(arrow[0], x, y + 3);
		rm.render(arrow[0], x + 1, y + 3);
		rm.render(arrow[0], x + 2, y + 3);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 3, y + 4);
		rm.render(arrow[0], x + 3, y + 5);
		rm.render(arrow[0], x + 3, y + 6);
	}
	else if (a == 13) { //Down-right
		rm.render(arrow[0], x + 3, y + 6);
		rm.render(arrow[0], x + 3, y + 5);
		rm.render(arrow[0], x + 3, y + 4);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 4, y + 3);
		rm.render(arrow[0], x + 5, y + 3);
		rm.render(arrow[0], x + 6, y + 3);
	}
	else if (a == 14) { //Up-right
		rm.render(arrow[0], x + 6, y + 3);
		rm.render(arrow[0], x + 5, y + 3);
		rm.render(arrow[0], x + 4, y + 3);
		rm.render(arrow[0], x + 3, y + 3);
		rm.render(arrow[0], x + 3, y + 2);
		rm.render(arrow[0], x + 3, y + 1);
		rm.render(arrow[0], x + 3, y);
	}
}