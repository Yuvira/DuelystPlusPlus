#pragma once

//Include
#include "boardtile.h"

//Define
class Collections;
class Game;
class Renderer;

//Codex view
class CardViewer {

public:

	//Constructor
	CardViewer(Game* game, Collections* collections, bool* modeSwitch);
	~CardViewer();

	//Rendering
	void RenderCollection(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);

	//Input
	void Input();
	void MoveCursor(int x, int y);

	//Render position evals
	constexpr int TileX(int x) { return (x * 7) + 1; }
	constexpr int TileY(int y) { return (y * 7) + 6; }
	constexpr int SpriteX(int x) { return (x * 7) + 2; }
	constexpr int SpriteY(int y) { return (y + 1) * 7; }
	constexpr int StartIdx(int page) { return page * cardsPerPage; }

private:

	//Properties
	Coord pos;
	int page;
	int pageCount;
	Game* game;
	Collections* collections;
	bool* modeSwitch;
	Sprite board;
	Sprite tileBorder;
	Sprite count[5];
	Sprite pageNumber;

	//Constants
	static constexpr int pageWidth = 9;
	static constexpr int pageHeight = 6;
	static constexpr int cardsPerPage = pageWidth * pageHeight;

};