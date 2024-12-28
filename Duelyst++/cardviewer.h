//Defines
#ifndef __CARD_VIEWER_H__
#define __CARD_VIEWER_H__

//Include
#include "game.h"

//Game class
class CardViewer {
public:
	CardViewer();
	~CardViewer();
	void Input();
	void Update();
	void RenderCollection(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);
	void MoveCursor(int x, int y);
	Sprite board;
	Sprite count[5];
	Sprite pageNumber;
	Collections collections;
	Tile tiles[9][7];
	Coord pos;
	int page;
	int pageCount;
	bool* modeSwitch;
};

#endif