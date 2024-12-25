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
	void Input();
	void Update();
	void RenderCollection(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);
	void MoveCursor(int x, int y);
	Sprite board;
	Sprite count[5];
	Sprite pageNumber;
	CardList cardList;
	Tile tiles[9][7];
	Coord pos;
	int page;
	int pageCount;
	bool* modeSwitch;
};

#endif