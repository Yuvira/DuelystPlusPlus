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
	CardList cl;
	Sprite board;
	Sprite pointer;
	Player player[2];
	Map map;
	bool turn;
	int px, py;
};

#endif