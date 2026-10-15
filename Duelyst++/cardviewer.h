#pragma once

//Include
#include "tile.h"

//Define
class Collections;
class Renderer;

//Codex view
class CardViewer {

//Methods
public:
	CardViewer(Collections* collections, bool* modeSwitch);
	~CardViewer();
	void Input();
	void Update();
	void RenderCollection(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);
	void MoveCursor(int x, int y);

//Properties
private:
	Sprite board;
	Sprite count[5];
	Sprite pageNumber;
	Collections* collections;
	Tile tiles[9][7];
	Coord pos;
	int page;
	int pageCount;
	bool* modeSwitch;

};