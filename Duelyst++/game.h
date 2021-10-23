//Defines
#ifndef __GAME_H__
#define __GAME_H__

//Include
#include <iostream>
#include <conio.h>
#include "player.h"

//Game class
class Game {
public:
	Game();
	~Game();
	void input();
	void update();
	void render(Renderer& rm);
	void moveCursor(int x, int y);
	bool moveable(int x, int y);
	void highlightTile(int x, int y, eColor col);
	void highlightMoveable(int x, int y);
	CardList cl;
	Player player[2];
	Map map;
	std::vector<Tile*> highlighted;
	Unit* selected;
	bool turn;
	int px, py;
};

#endif