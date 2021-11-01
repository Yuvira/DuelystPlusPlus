//Defines
#ifndef __COLLECTION_H__
#define __COLLECTION_H__

//Include
#include "game.h"

//Game class
class Collection {
public:
	Collection();
	~Collection();
	void input();
	void update();
	void render(Renderer& rm);
	void renderSidebar(Renderer& rm);
	void moveCursor(int x, int y);
	Sprite board;
	Sprite count[5];
	CardList cl;
	Tile tile[9][7];
	Coord pos;
	bool* modeSwitch;
};

#endif