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
	void move(int x, int y);
	CardList cl;
	Player player[2];
	Map map;
	std::vector<Tile*> select;
	bool turn;
	int px, py;
};

#endif