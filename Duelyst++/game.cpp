//Include
#include "game.h"

//Callback stuff
Callback::Callback(Unit* u, Spell* s, BoardTile* t, eSkill es) {
	unit = u;
	spell = s;
	tile = t;
	skill = es;
}
Callback::~Callback() {}

//Path co-ordinate constructor / destructor
PathCoord::PathCoord(Coord _pos, int _last, int _count) {
	pos = _pos;
	last = _last;
	count = _count;
}
PathCoord::~PathCoord() {}

//Game constructor / destructor
Game::Game() {

	//Board border
	board.createFromFile("resources/board.txt");

	//Event manager context
	em.game = this;

	//Character sprites
	char c[] = { 'Û', 'Þ', 'Ý', 'Ü', 'ß', 'X', '\\', '/', '®', 'é', '¯' };
	for (int a = 0; a < 8; ++a) { chars[a].buffer[0].Char.AsciiChar = c[a]; }

	//Turn indicator
	light.resize(3, 1);
	for (int a = 0; a < 3; ++a) { light.buffer[a].Char.AsciiChar = c[a + 8]; }
	light.setCol(COLOR_LTBLUE);

	//Set opponents
	player[0].enemy = &player[1];
	player[1].enemy = &player[0];

	//Initialize players
	for (int a = 0; a < 2; ++a) {
		player[a].preset(cl, this);
		summon(player[a].deck[0], a * 8, 2, false);
		player[a].deck.erase(player[a].deck.begin());
		player[a].general = unit.back();
		player[a].init(a + 2);
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
	endTurn = false;
	changeTurn(false);
	moveCursor(0, 0);
	activeUnit = nullptr;
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

		//Replace card
		else if (asciiVal == 114 || asciiVal == 82) { player[turn].replace(hPos); }

		//Select card
		else if (asciiVal == 32) {
			if (hPos < player[turn].hand.size()) {
				selectCard();
			}
		}

	}

	//Move mode
	else if (mode == MODE_MOVE) {

		//Move arrow
		if (asciiVal == 119 || asciiVal == 87) { moveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { moveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { moveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { moveSelect(1, 0); } //D

		//Move unit
		else if (asciiVal == 32) {
			for (int a = 0; a < attackable.size(); ++a) {
				if (attackable[a] == selectable[sPos]) {
					attackUnit();
					goto end;
				}
			}
			moveUnit();
			end:;
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
			if (callback.unit || callback.spell) { useEffect(); }
			else { useCard(); }
			if (endTurn) {
				update();
				changeTurn(!turn);
			}
		}

	}

}

//Update loop
void Game::update() {

	//Update units
	bool reloop;
	do { 
		reloop = false;
		for (int a = 0; a < unit.size(); ++a) {
			if (!unit[a]->dead) {
				unit[a]->update(reloop);
			}
		}
	} while (reloop);
	for (int a = 0; a < unit.size(); ++a) {
		if (unit[a]->dead) {
			if (unit[a] == unit[a]->tile->unit) { unit[a]->tile->unit = nullptr; }
			unit[a]->tile = nullptr;
			grave.push_back(unit[a]);
			unit.erase(unit.begin() + a);
			--a;
		}
		else { unit[a]->updateStatSprites(); }
	}

	//Update mana bars
	player[0].updateMana(COLOR_LTBLUE);
	player[1].updateMana(COLOR_RED);

	//Clear tile highlights
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			map.tile[a][b].setCol(COLOR_LTWHITE);
		}
	}

	//Re-highlight hostiles
	hostile.clear();
	for (int b = 0; b < unit.size(); ++b) {
		if (unit[b]->player != &player[turn]) {
			hostile.push_back(unit[b]->tile);
		}
	}
	for (int a = 0; a < hostile.size(); ++a) { hostile[a]->setCol(COLOR_LTRED); }

	//Highlight moveable spaces and create move path
	if (mode == MODE_MOVE) {
		generatePaths();
		for (int a = 0; a < highlighted.size(); ++a) { highlighted[a]->setCol(COLOR_AQUA); }
		map.tile[pos.x][pos.y].setCol(COLOR_LTWHITE);
		for (int a = 0; a < hostile.size(); ++a) {
			if (activeUnit->canAttack(hostile[a]->unit)) {
				hostile[a]->setCol(COLOR_RED);
			}
		}
	}

	//Highlight selectable spaces
	else if (mode == MODE_SELECT) {
		for (int a = 0; a < selectable.size(); ++a) { selectable[a]->setCol(COLOR_GREEN); }
		selectable[sPos]->setCol(COLOR_LTGREEN);
	}

	//Highlight cursor position
	else if (mode == MODE_NONE) {
		highlighted.clear();
		highlightTile(pos.x, pos.y, COLOR_AQUA);
	}

}

//Render objects
void Game::render(Renderer& rm) {

	//Board
	rm.render(board);

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

	//Move mode indicators
	if (mode == MODE_MOVE) {
		for (int a = 0; a < attackable.size(); ++a) {
			if (attackable[a] == selectable[sPos]) {
				COORD c = selectable[sPos]->border.pos;
				drawSword(c.X, c.Y, rm);
				c = activeUnit->tile->border.pos;
				if (selectable[sPos]->unit->canAttack(activeUnit)) { drawSword(c.X, c.Y, rm); }
				goto end;
			}
		}
		drawPath(rm);
		end:;
	}

	//Hand
	for (int a = 0; a < 7; ++a) { rm.render(hand[a].border); }
	for (int a = 0; a < player[turn].hand.size(); ++a) {
		player[turn].hand[a]->sprite.setCol(COLOR_LTWHITE);
		rm.render(player[turn].hand[a]->sprite, (a * 7) + 9, 42);
	}

	//Player UI
	player[0].render(rm, true);
	player[1].render(rm, false);

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

	//Indicate turn is ending
	endTurn = true;

	//Do late callbacks
	while (lateCallback.size() > 0) {
		if (lateCallback[0].spell != nullptr) { lateCallback[0].spell->lateCallback(); }
		lateCallback.erase(lateCallback.begin());
		if (selectable.size() > 0) { return; }
	}

	//Draw and reset replaces
	if (turnCount > 0) { player[turn].draw(); }
	player[turn].replaces = 1;

	//Change turn
	turn = t;
	if (!turn) { ++turnCount; }

	//Refresh mana
	if (turnCount > 1 && player[turn].manaMax < 9) { ++player[turn].manaMax; }
	player[turn].mana = player[turn].manaMax;

	//Set turn indicator
	if (turn) { light.setCol(COLOR_RED); }
	else { light.setCol(COLOR_LTBLUE); }

	//Refresh units
	for (int a = 0; a < unit.size(); ++a) {
		unit[a]->moved = false;
		unit[a]->attacked = false;
	}

	//End old turn, start new turn
	em.sendOnTurnEnd(&player[!t]);
	em.sendOnTurnStart(&player[t]);

	//Turn has ended
	endTurn = false;

}

//Set game context for new token cards
void Game::setContext(Card* c, Player* p) {
	c->game = this;
	c->player = p;
}

//Summon at position
void Game::summon(Card* c, int x, int y, bool actionBar) {
	unit.push_back(dynamic_cast<Unit*>(c));
	unit.back()->setPos(x, y);
	em.sendOnSummon(unit.back(), actionBar);
}

//Use active card
void Game::useCard() {

	//Use mana
	player[turn].mana -= activeCard->cost;

	//Summon unit
	if (activeCard->type == CARD_UNIT) {
		pos = selectable[sPos]->pos;
		hand[hPos + 1].setCol(COLOR_LTWHITE);
		selectable.clear();
		sPos = -1;
		mode = MODE_NONE;
		player[turn].hand.erase(player[turn].hand.begin() + hPos);
		summon(activeCard, pos.x, pos.y, true);
		activeCard = nullptr;
		hPos = -1;
	}

	//Use spell
	else if (activeCard->type == CARD_SPELL) {
		pos = selectable[sPos]->pos;
		hand[hPos + 1].setCol(COLOR_LTWHITE);
		selectable.clear();
		sPos = -1;
		mode = MODE_NONE;
		player[turn].hand.erase(player[turn].hand.begin() + hPos);
		grave.push_back(activeCard);
		dynamic_cast<Spell*>(activeCard)->onUse(&map.tile[pos.x][pos.y]);
		activeCard = nullptr;
		hPos = -1;
	}

}

//Use active effect
void Game::useEffect() {
	BoardTile* t = selectable[sPos];
	pos = t->pos;
	selectable.clear();
	sPos = -1;
	mode = MODE_NONE;
	if (callback.unit != nullptr) { callback.unit->callback(t); }
	else if (callback.spell != nullptr) { callback.spell->callback(t); }
}

//Select unit
void Game::select(BoardTile& t) {
	for (int a = 0; a < highlighted.size(); ++a) { moveable.push_back(highlighted[a]); }
	if (!t.unit->attacked) {
		for (int a = 0; a < hostile.size(); ++a) {
			if (t.unit->canAttack(hostile[a]->unit)) {
				attackable.push_back(hostile[a]);
			}
		}
	}
	if (moveable.size() > 1 || attackable.size() > 0) {
		selectable.insert(selectable.end(), moveable.begin(), moveable.end());
		selectable.insert(selectable.end(), attackable.begin(), attackable.end());
		activeUnit = t.unit;
		sPos = 0;
		mode = MODE_MOVE;
	}
	else { moveable.clear(); }
}

//Select card in hand
void Game::selectCard() {

	//Unit card
	if (player[turn].hand[hPos]->type == CARD_UNIT) {
		if (player[turn].hand[hPos]->cost <= player[turn].mana) {
			highlightSelectable(TARGET_NEAR_ALLY);
			if (selectable.size() > 0) { activeCard = player[turn].hand[hPos]; }
		}
	}

	//Spell card
	if (player[turn].hand[hPos]->type == CARD_SPELL) {
		if (player[turn].hand[hPos]->cost <= player[turn].mana) {
			highlightSelectable(dynamic_cast<Spell*>(player[turn].hand[hPos])->target);
			if (selectable.size() > 0) { activeCard = player[turn].hand[hPos]; }
		}
	}

}

//Move selected unit
void Game::moveUnit() {
	if (selectable[sPos]->unit == nullptr || selectable[sPos] == &map.tile[pos.x][pos.y]) {
		if (selectable[sPos] != &map.tile[pos.x][pos.y]) {
			activeUnit->setPos(selectable[sPos]->pos.x, selectable[sPos]->pos.y);
			activeUnit->moved = true;
			pos = selectable[sPos]->pos;
		}
		moveable.clear();
		attackable.clear();
		selectable.clear();
		activeUnit = nullptr;
		mode = MODE_NONE;
	}
}

//Attack unit
void Game::attackUnit() {
	activeUnit->attack(selectable[sPos]->unit, false);
	pos = selectable[sPos]->pos;
	moveable.clear();
	attackable.clear();
	selectable.clear();
	activeUnit = nullptr;
	mode = MODE_NONE;
}

//Move cursor position
void Game::moveCursor(int x, int y) {
	
	//Move cursor
	pos.x = (pos.x + x + 9) % 9;
	if ((y == -1 && pos.y == 0) || (y == 1 && pos.y == 4)) {
		hPos = max(min(pos.x - 1, 5), 0);
		hand[hPos + 1].setCol(COLOR_AQUA);
		pos = Coord(-1, -1);
		mode = MODE_HAND;
	}
	else { pos.y = (pos.y + y + 5) % 5; }

}

//Move cursor position
void Game::moveCursorHand(int x, int y) {

	//Deselect tile
	hand[hPos + 1].setCol(COLOR_LTWHITE);

	//Move cursor
	hPos = (hPos + x + 6) % 6;
	if (y != -1) {
		pos = Coord(hPos + 1, y);
		hPos = -1;
		mode = MODE_NONE;
		return;
	}

	//Select new tile
	hand[hPos + 1].setCol(COLOR_AQUA);

}

//Move selected tile
void Game::moveSelect(int x, int y) {
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
	end:;
}

//Check if tile is moveable
bool Game::canMove(int x, int y) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		if (map.tile[x][y].unit == nullptr) { return true; }
		else if (map.tile[x][y].unit->player == map.tile[pos.x][pos.y].unit->player) { return true; }
	}
	return false;
}

//Highlight tile
void Game::highlightTile(int x, int y, eColor col) {
	map.tile[x][y].setCol(col);
	highlighted.push_back(&map.tile[x][y]);
	if (col == COLOR_AQUA && mode == MODE_NONE) {
		if (map.tile[pos.x][pos.y].unit != nullptr) {
			if (!map.tile[pos.x][pos.y].unit->moved && map.tile[pos.x][pos.y].unit->isMoveable()) {
				if (map.tile[pos.x][pos.y].unit->player == &player[turn]) {
					highlightMoveable(pos.x, pos.y);
				}
			}
		}
	}
}

//Highlight moveable spaces
void Game::highlightMoveable(int x, int y) {
	if (map.tile[pos.x][pos.y].unit->isFlying()) {
		for (int a = 0; a < 9; ++a) {
			for (int b = 0; b < 5; ++b) {
				if (a != pos.x || b != pos.y) {
					if (canMove(a, b)) {
						highlightTile(a, b, COLOR_GRAY);
					}
				}
			}
		}
	}
	else {
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
}

//Highlight targetable tiles
void Game::highlightSelectable(eTarget type, Unit* u) {

	//Clear
	selectable.clear();

	//Target type
	switch (type) {

	//Empty tile
	case TARGET_TILE:
		for (int a = 0; a < 9; ++a) {
			for (int b = 0; b < 5; ++b) {
				if (map.tile[a][b].unit == nullptr) {
					selectable.push_back(&map.tile[a][b]);
				}
			}
		}
		break;

	//Any minion
	case TARGET_MINION:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a]->tribe != TRIBE_GENERAL) {
				selectable.push_back(unit[a]->tile);
			}
		}
		break;

	//Enemies
	case TARGET_ENEMY:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a]->player != &player[turn]) {
				selectable.push_back(unit[a]->tile);
			}
		}
		break;

	//Allied minions
	case TARGET_ALLY_MINON:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a]->player == &player[turn]) {
				if (unit[a]->tribe != TRIBE_GENERAL) {
					selectable.push_back(unit[a]->tile);
				}
			}
		}
		break;

	//Enemy minions
	case TARGET_ENEMY_MINION:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a]->player != &player[turn]) {
				if (unit[a]->tribe != TRIBE_GENERAL) {
					selectable.push_back(unit[a]->tile);
				}
			}
		}
		break;

	//Enemey general
	case TARGET_ENEMY_GENERAL:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a] == player[!turn].general) {
				selectable.push_back(unit[a]->tile);
			}
		}
		break;

	//Near allies (summon)
	case TARGET_NEAR_ALLY:
		for (int a = 0; a < unit.size(); ++a) {
			if (unit[a]->player == &player[turn]) {
				for (int b = max(unit[a]->tile->pos.x - 1, 0); b < min(unit[a]->tile->pos.x + 2, 9); ++b) {
					for (int c = max(unit[a]->tile->pos.y - 1, 0); c < min(unit[a]->tile->pos.y + 2, 5); ++c) {
						if (map.tile[b][c].unit == nullptr) {
							if (map.tile[b][c].border.buffer[0].Attributes != COLOR_GREEN) {
								selectable.push_back(&map.tile[b][c]);
							}
						}
					}
				}
			}
		}
		break;

	//Minons near unit
	case TARGET_MINION_NEAR_UNIT:
		if (u != nullptr) {
			for (int a = max(u->tile->pos.x - 1, 0); a < min(u->tile->pos.x + 2, 9); ++a) {
				for (int b = max(u->tile->pos.y - 1, 0); b < min(u->tile->pos.y + 2, 5); ++b) {
					if (map.tile[a][b].unit != nullptr && map.tile[a][b].unit != u && map.tile[a][b].unit->tribe != TRIBE_GENERAL) {
						selectable.push_back(&map.tile[a][b]);
					}
				}
			}
		}
		break;

	}

	//If selectable tiles found, highlight first and switch to select mode
	if (selectable.size() > 0) {
		sPos = 0;
		mode = MODE_SELECT;
	}

}

//Get final path from list
void Game::createPath() {
	int i = pathList.size() - 1;
	for (int a = 0; a < pathList.back().count + 1; ++a) {
		path.insert(path.begin(), pathList[i].pos);
		i = pathList[i].last;
	}
}

//Add position to path list
bool Game::addToPaths(int x, int y, int l, int c) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		for (int a = 0; a < pathList.size(); ++a) { if (pathList[a].pos.x == x && pathList[a].pos.y == y) { return false; } }
		if (map.tile[pos.x][pos.y].unit->isFlying()) {
			pathList.push_back(PathCoord(Coord(x, y), l, c));
			if (selectable[sPos]->pos.x == x && selectable[sPos]->pos.y == y) { return true; }
			return false;
		}
		for (int a = 0; a < moveable.size(); ++a) {
			if (moveable[a] == &map.tile[x][y]) {
				pathList.push_back(PathCoord(Coord(x, y), l, c));
				if (selectable[sPos] == moveable[a]) { return true; }
				return false;
			}
		}
	}
	return false;
}

//Generate paths to target
void Game::generatePaths() {
	path.clear();
	if (selectable[sPos] != &map.tile[pos.x][pos.y]) {
		for (int a = 0; a < attackable.size(); ++a) { if (attackable[a] == selectable[sPos]) { return; } }
		pathList.push_back(PathCoord(pos, 0, 0));
		int count = 0;
		while (true) {
			for (int a = 0; a < pathList.size(); ++a) {
				if (pathList[a].count == count) {
					if (addToPaths(pathList[a].pos.x, pathList[a].pos.y - 1, a, pathList[a].count + 1)) { goto end; }
					if (addToPaths(pathList[a].pos.x, pathList[a].pos.y + 1, a, pathList[a].count + 1)) { goto end; }
					if (addToPaths(pathList[a].pos.x - 1, pathList[a].pos.y, a, pathList[a].count + 1)) { goto end; }
					if (addToPaths(pathList[a].pos.x + 1, pathList[a].pos.y, a, pathList[a].count + 1)) { goto end; }
				}
			}
			++count;
		}
		end:;
		createPath();
		pathList.clear();
	}
}

//Draw arrow path
void Game::drawPath(Renderer& rm) {
	int a = path.size() - 1;
	if (a > 0) {
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
		if (a > 1) {
			for (int b = 1; b < a; ++b) {
				c = map.tile[path[b].x][path[b].y].border.pos;
				if (path[b].x == path[b - 1].x && path[b].x == path[b + 1].x) { drawArrow(9, c.X, c.Y, rm); }       //Up-down
				else if (path[b].y == path[b - 1].y && path[b].y == path[b + 1].y) { drawArrow(10, c.X, c.Y, rm); } //Left-right
				else if ((path[b].x > path[b - 1].x && path[b].y > path[b + 1].y) || (path[b].x > path[b + 1].x && path[b].y > path[b - 1].y)) { drawArrow(11, c.X, c.Y, rm); } //Up-left
				else if ((path[b].x > path[b - 1].x && path[b].y < path[b + 1].y) || (path[b].x > path[b + 1].x && path[b].y < path[b - 1].y)) { drawArrow(12, c.X, c.Y, rm); } //Down-left
				else if ((path[b].x < path[b - 1].x && path[b].y < path[b + 1].y) || (path[b].x < path[b + 1].x && path[b].y < path[b - 1].y)) { drawArrow(13, c.X, c.Y, rm); } //Down-right
				else if ((path[b].x < path[b - 1].x && path[b].y > path[b + 1].y) || (path[b].x < path[b + 1].x && path[b].y > path[b - 1].y)) { drawArrow(14, c.X, c.Y, rm); } //Up-right
			}
		}
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