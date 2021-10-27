//Include
#include "game.h"

//Callback stuff
Callback::Callback(Unit* u, eEffect e) {
	unit = u;
	effect = e;
}
Callback::~Callback() {}

//Game constructor / destructor
Game::Game() {

	//Board border
	board.createFromFile("resources/board.txt");

	//Character sprites
	char c[] = { 'Û', 'Þ', 'Ý', 'Ü', 'ß', 'X', '\\', '/', '®', 'é', '¯' };
	for (int a = 0; a < 8; ++a) { chars[a].buffer[0].Char.AsciiChar = c[a]; }

	//Turn indicator
	light.resize(3, 1);
	for (int a = 0; a < 3; ++a) { light.buffer[a].Char.AsciiChar = c[a + 8]; }
	light.setCol(COLOR_LTBLUE);

	//Initialize players
	for (int a = 0; a < 2; ++a) {
		player[a].preset(cl);
		summon(player[a].deck[0], a, a * 8, 2);
		player[a].deck.erase(player[a].deck.begin());
		player[a].general = unit.back();
		player[a].init(a + 2, this);
	}

	//Hand
	for (int a = 0; a < 7; ++a) {
		hand[a].border.pos.X = (a * 7) + 1;
		hand[a].border.pos.Y = 41;
	}

	//Variables
	pos = Coord(0, 0);
	hPos = -1;
	sPos = -1;
	turnCount = 0;
	changeTurn(false);
	moveCursor(0, 0);
	activeUnit = nullptr;
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
		if (asciiVal == 119 || asciiVal == 87) { moveCursor(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { moveCursor(-1, 0); } //A
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

	//Hand mode
	else if (mode == MODE_HAND) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { moveCursorHand(0, 4); }       //W
		else if (asciiVal == 97 || asciiVal == 65) { moveCursorHand(-1, -1); } //A
		else if (asciiVal == 115 || asciiVal == 83) { moveCursorHand(0, 0); }  //S
		else if (asciiVal == 100 || asciiVal == 68) { moveCursorHand(1, -1); } //D

		//Select card
		else if (asciiVal == 32) {
			if (hPos < player[turn].hand.size()) {
				if (player[turn].hand[hPos]->type == CARD_UNIT) {
					if (player[turn].hand[hPos]->cost <= player[turn].mana) {
						highlightSelectable(TARGET_NEAR_ALLY);
						if (selectable.size() > 0) { activeCard = player[turn].hand[hPos]; }
					}
				}
			}
		}

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

	//Select mode
	else if (mode == MODE_SELECT) {

		//Move selection
		if (asciiVal == 119 || asciiVal == 87) { moveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { moveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { moveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { moveSelect(1, 0); } //D

		//Use card at selection
		else if (asciiVal == 32) {
			if (callback.unit) { useEffect(); }
			else { useCard(); }
		}

	}

}

//Update loop
void Game::update() {

	//Update units
	bool reloop;
	do { 
		reloop = false;
		for (int a = 0; a < unit.size(); ++a) { unit[a]->update(reloop); }
	} while (reloop);
	for (int a = 0; a < unit.size(); ++a) { unit[a]->updateStatSprites(); }

	//Update mana bars
	player[0].updateMana(COLOR_LTBLUE);
	player[1].updateMana(COLOR_RED);

}

//Render objects
void Game::render(Renderer& rm) {

	//Board
	rm.render(board);

	//Player UI
	player[0].render(rm, true);
	player[1].render(rm, false);

	//Turn indicator
	rm.render(light, 31, 2);

	//Tiles
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			rm.render(map.tile[a][b].border);
			rm.render(map.tile[a][b].sprite);
		}
	}

	//Units
	for (int a = 0; a < unit.size(); ++a) { unit[a]->render(rm); }

	//Attack indicators
	if (attack != nullptr) {
		COORD c = attack->border.pos;
		drawSword(c.X, c.Y, rm);
		c = map.tile[pos.x][pos.y].border.pos;
		if (canAttack(*attack, map.tile[pos.x][pos.y])) {drawSword(c.X, c.Y, rm);}
	}

	//Arrows
	else if (path.size() > 1) { drawPath(rm); }

	//Hand
	for (int a = 0; a < 7; ++a) { rm.render(hand[a].border); }
	for (int a = 0; a < player[turn].hand.size(); ++a) { rm.render(player[turn].hand[a]->sprite, (a * 7) + 9, 42); }

	//Sidebar
	renderSidebar(rm);

}

//Render sidebar
void Game::renderSidebar(Renderer& rm) {

	//Vertical position
	int y = 1;

	//Unit on board
	if (mode == MODE_NONE) {
		if (map.tile[pos.x][pos.y].unit != nullptr) {
			map.tile[pos.x][pos.y].unit->drawDetails(rm, y);
		}
	}

	//Unit in hand
	else if (mode == MODE_HAND || mode == MODE_SELECT) {
		if (hPos < player[turn].hand.size()) {
			player[turn].hand[hPos]->drawDetails(rm, y);
		}
	}

}

//Change turn
void Game::changeTurn(bool t) {
	if (turnCount > 0) { player[turn].draw(); }
	turn = t;
	if (!turn) { ++turnCount; }
	if (turnCount > 1 && player[turn].manaMax < 9) { ++player[turn].manaMax; }
	player[turn].mana = player[turn].manaMax;
	if (turn) { light.setCol(COLOR_RED); }
	else { light.setCol(COLOR_LTBLUE); }
	for (int a = 0; a < hostile.size(); ++a) { hostile[a]->setCol(COLOR_LTWHITE); }
	hostile.clear();
	for (int a = 0; a < unit.size(); ++a) {
		if (unit[a]->player != &player[turn]) {
			unit[a]->tile->setCol(COLOR_LTRED);
			hostile.push_back(unit[a]->tile);
		}
	}
	moveCursor(0, 0);
}

//Summon at position
void Game::summon(Card* c, bool p, int x, int y) {
	unit.push_back(new Unit(*(dynamic_cast<Unit*>(c))));
	unit.back()->player = &player[p];
	unit.back()->game = this;
	unit.back()->setPos(x, y);
	for (int a = 0; a < unit.size(); ++a) { unit[a]->onSummon(*unit.back()); }
}

//Use active card
void Game::useCard() {

	//Use mana
	player[turn].mana -= activeCard->cost;

	//Summon unit
	if (activeCard->type == CARD_UNIT) {
		pos = selectable[sPos]->pos;
		hand[hPos + 1].setCol(COLOR_LTWHITE);
		hPos = -1;
		for (int a = 0; a < selectable.size(); ++a) { selectable[a]->setCol(COLOR_LTWHITE); }
		selectable.clear();
		sPos = -1;
		mode = MODE_NONE;
		summon(activeCard, turn, pos.x, pos.y);
		for (int a = 0; a < player[turn].hand.size(); ++a) {
			if (player[turn].hand[a] == activeCard) {
				player[turn].hand.erase(player[turn].hand.begin() + a);
				activeCard = nullptr;
				break;
			}
		}
		if (!callback.unit) { moveCursor(0, 0); }
	}

}

//Use active effect
void Game::useEffect() {
	BoardTile* t = selectable[sPos];
	pos = t->pos;
	for (int a = 0; a < selectable.size(); ++a) { selectable[a]->setCol(COLOR_LTWHITE); }
	selectable.clear();
	sPos = -1;
	mode = MODE_NONE;
	callback.unit->callback(t);
	if (!callback.unit) { moveCursor(0, 0); }
}

//Select unit
void Game::select(BoardTile& t) {
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
	activeUnit = t.unit;
	t.setCol(COLOR_LTWHITE);
	mode = MODE_MOVE;
	path.push_back(pos);
}

//Move selected unit
void Game::moveUnit() {
	if (path.size() > 1) {
		activeUnit->setPos(path.back().x, path.back().y);
		pos = path.back();
	}
	moveable.clear();
	attackable.clear();
	activeUnit = nullptr;
	mode = MODE_NONE;
	path.clear();
	moveCursor(0, 0);
}

//Attack unit
void Game::attackUnit() {
	activeUnit->attack(*attack->unit);
	if (canAttack(*attack, map.tile[pos.x][pos.y])) { attack->unit->attack(*activeUnit); }
	pos = attack->pos;
	moveable.clear();
	attackable.clear();
	activeUnit = nullptr;
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
	pos.x = (pos.x + x + 9) % 9;
	if ((y == -1 && pos.y == 0) || (y == 1 && pos.y == 4)) {
		hPos = max(min(pos.x - 1, 5), 0);
		hand[hPos + 1].setCol(COLOR_AQUA);
		pos = Coord(-1, -1);
		mode = MODE_HAND;
	}
	else { pos.y = (pos.y + y + 5) % 5; }

	//Re-highlight enemies in red
	for (int a = 0; a < hostile.size(); ++a) { hostile[a]->setCol(COLOR_LTRED); }

	//Select tile at cursor
	if (mode == MODE_NONE) { highlightTile(pos.x, pos.y, COLOR_AQUA); }

}

//Move cursor position
void Game::moveCursorHand(int x, int y) {

	//Deselect tile
	hand[hPos + 1].setCol(COLOR_LTWHITE);

	//Move cursor
	hPos = (hPos + x + 6) % 6;
	if (y != -1) {
		pos = Coord(hPos + 1, y);
		highlightTile(pos.x, pos.y, COLOR_AQUA);
		hPos = -1;
		mode = MODE_NONE;
		moveCursor(0, 0);
		return;
	}

	//Select new tile
	hand[hPos + 1].setCol(COLOR_AQUA);

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

//Move selected tile
void Game::moveSelect(int x, int y) {
	selectable[sPos]->setCol(COLOR_GREEN);
	int _x = selectable[sPos]->pos.x + x;
	int _y = selectable[sPos]->pos.y + y;
	if (x != 0) {
		for (int a = 0; x > 0 ? a < 9 : a > -9; a += x) {
			for (int b = 0; b < 3; b > 0 ? b *= -1 : b = (b - 1) * -1) {
				int __x = (_x + a + 9) % 9;
				int __y = (_y + b + 5) % 5;
				for (int c = 0; c < selectable.size(); ++c) {
					if (&map.tile[__x][__y] == selectable[c]) {
						sPos = c;
						goto end;
					}
				}
			}
		}
	}
	else if (y != 0) {
		for (int a = 0; y > 0 ? a < 5 : a > -5; a += y) {
			for (int b = 0; b < 5; b > 0 ? b *= -1 : b = (b - 1) * -1) {
				int __x = (_x + b + 9) % 9;
				int __y = (_y + a + 5) % 5;
				for (int c = 0; c < selectable.size(); ++c) {
					if (&map.tile[__x][__y] == selectable[c]) {
						sPos = c;
						goto end;
					}
				}
			}
		}
	}
	end:
	selectable[sPos]->setCol(COLOR_LTGREEN);
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
bool Game::canAttack(BoardTile& t1, BoardTile& t2) {
	if (abs(t1.pos.x - t2.pos.x) < 2 && abs(t1.pos.y - t2.pos.y) < 2) {
		return true;
	}
	return false;
}

//Highlight tile
void Game::highlightTile(int x, int y, eColor col) {
	map.tile[x][y].setCol(col);
	highlighted.push_back(&map.tile[x][y]);
	if (col == COLOR_AQUA && mode == MODE_NONE) {
		if (map.tile[pos.x][pos.y].unit != nullptr) {
			if (map.tile[pos.x][pos.y].unit->player == &player[turn]) {
				highlightMoveable(pos.x, pos.y);
			}
		}
	}
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

//Highlight targetable tiles
void Game::highlightSelectable(eTarget type, Unit* u) {

	//Near allies (summon)
	if (type == TARGET_NEAR_ALLY) {
		for (int a = 0; a < 9; ++a) {
			for (int b = 0; b < 5; ++b) {
				if (map.tile[a][b].unit != nullptr) {
					if (map.tile[a][b].unit->player == &player[turn]) {
						for (int c = max(a - 1, 0); c < min(a + 2, 9); ++c) {
							for (int d = max(b - 1, 0); d < min(b + 2, 5); ++d) {
								if (map.tile[c][d].unit == nullptr) {
									if (map.tile[c][d].border.buffer[0].Attributes != COLOR_GREEN) {
										map.tile[c][d].setCol(COLOR_GREEN);
										selectable.push_back(&map.tile[c][d]);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//Near unit
	else if (type == TARGET_MINION_NEAR_UNIT && u) {
		for (int a = max(u->tile->pos.x - 1, 0); a < min(u->tile->pos.x + 2, 9); ++a) {
			for (int b = max(u->tile->pos.y - 1, 0); b < min(u->tile->pos.y + 2, 5); ++b) {
				if (map.tile[a][b].unit != nullptr && map.tile[a][b].unit != u && map.tile[a][b].unit->tribe != TRIBE_GENERAL) {
					if (map.tile[a][b].border.buffer[0].Attributes != COLOR_GREEN) {
						map.tile[a][b].setCol(COLOR_GREEN);
						selectable.push_back(&map.tile[a][b]);
					}
				}
			}
		}
	}

	//If selectable tiles found, highlight first and switch to select mode
	if (selectable.size() > 0) {
		sPos = 0;
		selectable[0]->setCol(COLOR_LTGREEN);
		mode = MODE_SELECT;
	}

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
	rm.render(chars[6], x + 2, y + 2);
	rm.render(chars[7], x + 4, y + 2);
	rm.render(chars[5], x + 3, y + 3);
	rm.render(chars[5], x + 2, y + 4);
	rm.render(chars[5], x + 4, y + 4);
}

//Draw arrows
void Game::drawArrow(int a, int x, int y, Renderer& rm) {
	if (a == 1) { //Start up
		rm.render(chars[0], x + 3, y + 1);
		rm.render(chars[0], x + 3, y);
	}
	else if (a == 2) { // Start left
		rm.render(chars[0], x + 1, y + 3);
		rm.render(chars[0], x, y + 3);
	}
	else if (a == 3) { //Start down
		rm.render(chars[0], x + 3, y + 5);
		rm.render(chars[0], x + 3, y + 6);
	}
	else if (a == 4) { //Start right
		rm.render(chars[0], x + 5, y + 3);
		rm.render(chars[0], x + 6, y + 3);
	}
	else if (a == 5) { //End up
		rm.render(chars[0], x + 3, y + 6);
		rm.render(chars[0], x + 2, y + 5);
		rm.render(chars[0], x + 3, y + 5);
		rm.render(chars[0], x + 4, y + 5);
		rm.render(chars[1], x + 2, y + 4);
		rm.render(chars[0], x + 3, y + 4);
		rm.render(chars[2], x + 4, y + 4);
		rm.render(chars[0], x + 3, y + 3);
	}
	else if (a == 6) { //End left
		rm.render(chars[0], x + 6, y + 3);
		rm.render(chars[0], x + 5, y + 2);
		rm.render(chars[0], x + 5, y + 3);
		rm.render(chars[0], x + 5, y + 4);
		rm.render(chars[3], x + 4, y + 2);
		rm.render(chars[0], x + 4, y + 3);
		rm.render(chars[4], x + 4, y + 4);
		rm.render(chars[0], x + 3, y + 3);
	}
	else if (a == 7) { //End down
		rm.render(chars[0], x + 3, y);
		rm.render(chars[0], x + 2, y + 1);
		rm.render(chars[0], x + 3, y + 1);
		rm.render(chars[0], x + 4, y + 1);
		rm.render(chars[1], x + 2, y + 2);
		rm.render(chars[0], x + 3, y + 2);
		rm.render(chars[2], x + 4, y + 2);
		rm.render(chars[0], x + 3, y + 3);
	}
	else if (a == 8) { //End right
		rm.render(chars[0], x, y + 3);
		rm.render(chars[0], x + 1, y + 2);
		rm.render(chars[0], x + 1, y + 3);
		rm.render(chars[0], x + 1, y + 4);
		rm.render(chars[3], x + 2, y + 2);
		rm.render(chars[0], x + 2, y + 3);
		rm.render(chars[4], x + 2, y + 4);
		rm.render(chars[0], x + 3, y + 3);
	}
	else if (a == 9) { //Up-down
		rm.render(chars[0], x + 3, y);
		rm.render(chars[0], x + 3, y + 1);
		rm.render(chars[0], x + 3, y + 2);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 3, y + 4);
		rm.render(chars[0], x + 3, y + 5);
		rm.render(chars[0], x + 3, y + 6);
	}
	else if (a == 10) { //Left-right
		rm.render(chars[0], x, y + 3);
		rm.render(chars[0], x + 1, y + 3);
		rm.render(chars[0], x + 2, y + 3);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 4, y + 3);
		rm.render(chars[0], x + 5, y + 3);
		rm.render(chars[0], x + 6, y + 3);
	}
	else if (a == 11) { //Up-left
		rm.render(chars[0], x, y + 3);
		rm.render(chars[0], x + 1, y + 3);
		rm.render(chars[0], x + 2, y + 3);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 3, y + 2);
		rm.render(chars[0], x + 3, y + 1);
		rm.render(chars[0], x + 3, y);
	}
	else if (a == 12) { //Down-left
		rm.render(chars[0], x, y + 3);
		rm.render(chars[0], x + 1, y + 3);
		rm.render(chars[0], x + 2, y + 3);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 3, y + 4);
		rm.render(chars[0], x + 3, y + 5);
		rm.render(chars[0], x + 3, y + 6);
	}
	else if (a == 13) { //Down-right
		rm.render(chars[0], x + 3, y + 6);
		rm.render(chars[0], x + 3, y + 5);
		rm.render(chars[0], x + 3, y + 4);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 4, y + 3);
		rm.render(chars[0], x + 5, y + 3);
		rm.render(chars[0], x + 6, y + 3);
	}
	else if (a == 14) { //Up-right
		rm.render(chars[0], x + 6, y + 3);
		rm.render(chars[0], x + 5, y + 3);
		rm.render(chars[0], x + 4, y + 3);
		rm.render(chars[0], x + 3, y + 3);
		rm.render(chars[0], x + 3, y + 2);
		rm.render(chars[0], x + 3, y + 1);
		rm.render(chars[0], x + 3, y);
	}
}