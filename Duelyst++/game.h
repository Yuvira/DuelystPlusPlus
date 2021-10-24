//Defines
#ifndef __GAME_H__
#define __GAME_H__

//Include
#include <iostream>
#include <conio.h>
#include "player.h"

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE
};

//Custom co-ord because vector of COORD is invalid??
class Coord {
public:
	Coord(int _x = 0, int _y = 0);
	~Coord();
	int x;
	int y;
};

//Game class
class Game {
public:
	Game();
	~Game();
	void input();
	void update();
	void render(Renderer& rm);
	void moveCursor(int x, int y);
	void moveArrow(int x, int y);
	bool canMove(int x, int y);
	void highlightTile(int x, int y, eColor col);
	void highlightMoveable(int x, int y);
	void drawArrow(int a, int x, int y, Renderer& rm);
	Sprite arrow[5];
	CardList cl;
	Player player[2];
	Map map;
	std::vector<Tile*> highlighted;
	std::vector<Tile*> moveable;
	std::vector<Coord> path;
	Unit* selected;
	eMode mode;
	bool turn;
	int px, py;
};

#endif