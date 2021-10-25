//Defines
#ifndef __GAME_H__
#define __GAME_H__

//Include
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include "player.h"

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE,
	MODE_HAND
};

//Game class
class Game {
public:
	Game();
	~Game();
	void input();
	void update();
	void render(Renderer& rm);
	void changeTurn(bool t);
	void summon(Card* c, bool p, int x, int y);
	void select(BoardTile& t);
	void attackUnit();
	void moveUnit();
	void moveCursor(int x, int y);
	void moveCursorHand(int x, int y);
	void moveArrow(int x, int y);
	bool canMove(int x, int y);
	bool canAttack(BoardTile& t1, BoardTile& t2);
	void highlightTile(int x, int y, eColor col);
	void highlightMoveable(int x, int y);
	void drawPath(Renderer& rm);
	void drawSword(int x, int y, Renderer& rm);
	void drawArrow(int a, int x, int y, Renderer& rm);
	std::vector<Card*> grave;
	std::vector<Unit*> unit;
	Sprite board;
	Sprite chars[8];
	CardList cl;
	Player player[2];
	Map map;
	Tile hand[7];
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<Coord> path;
	BoardTile* attack;
	Unit* activeUnit;
	eMode mode;
	Coord pos;
	int hPos;
	bool turn;
};

#endif