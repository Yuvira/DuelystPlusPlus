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
	MODE_MOVE
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
	void select(Tile& t);
	void attackUnit();
	void moveUnit();
	void moveCursor(int x, int y);
	void moveArrow(int x, int y);
	bool canMove(int x, int y);
	bool canAttack(Tile& t1, Tile& t2);
	void highlightTile(int x, int y, eColor col);
	void highlightMoveable(int x, int y);
	void drawPath(Renderer& rm);
	void drawSword(int x, int y, Renderer& rm);
	void drawArrow(int a, int x, int y, Renderer& rm);
	Sprite arrow[5];
	Sprite sword[3];
	CardList cl;
	Player player[2];
	Map map;
	std::vector<Tile*> highlighted;
	std::vector<Tile*> moveable;
	std::vector<Tile*> hostile;
	std::vector<Tile*> attackable;
	std::vector<Coord> path;
	Tile* attack;
	Unit* selected;
	eMode mode;
	Coord pos;
	bool turn;
};

#endif