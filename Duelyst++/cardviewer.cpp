//Include
#include <conio.h>
#include "card.h"
#include "carddefinition.h"
#include "cardviewer.h"
#include "collections.h"
#include "game.h"
#include "player.h"
#include "renderer.h"

#pragma region Constructor

//Game constructor
CardViewer::CardViewer(Game* game, Collections* collections, bool* modeSwitch) {

	//Collections reference
	this->game = game;
	this->collections = collections;
	this->modeSwitch = modeSwitch;

	//Border
	board.CreateFromFile("resources/border.txt");
	tileBorder.CreateFromFile("resources/tile.txt");

	//Variables
	pos = Coord(0, 0);
	page = 0;
	pageCount = (this->collections->GetCardCount() + 53) / 54;
	pageNumber.CreateFromString(std::to_string(page + 1) + " / " + std::to_string(pageCount));
	modeSwitch = nullptr;

}
CardViewer::~CardViewer() {}

#pragma endregion

#pragma region Rendering

//Render objects
void CardViewer::RenderCollection(Renderer& renderer) {

	//Board
	renderer.Render(board, 0, 0);

	//Tiles
	tileBorder.SetColor(COLOR_LTWHITE);
	for (int x = 0; x < pageWidth; ++x)
		for (int y = 0; y < pageHeight; ++y)
			renderer.Render(tileBorder, TileX(x), TileY(y));
	tileBorder.SetColor(COLOR_AQUA);
	renderer.Render(tileBorder, TileX(pos.x), TileY(pos.y));

	//Cards
	int x, y;
	for (int idx = StartIdx(page); idx < min(StartIdx(page + 1), collections->GetCardCount()); ++idx) {
		x = ((idx - StartIdx(page)) % pageWidth);
		y = ((idx - StartIdx(page)) / pageWidth);
		collections->RenderCardSprite(renderer, SpriteX(x), SpriteY(y), idx);
	}

	//Card counts
	count[0].CreateFromString("Generals  : " + std::to_string(collections->CountCardsWhere([](const CardDefinition& def) { return def.tribe == TRIBE_GENERAL; })));
	count[1].CreateFromString("Minions   : " + std::to_string(collections->CountCardsWhere([](const CardDefinition& def) { return def.type == CARDTYPE_UNIT && def.tribe != TRIBE_GENERAL; })));
	count[2].CreateFromString("Spells    : " + std::to_string(collections->CountCardsWhere([](const CardDefinition& def) { return def.type == CARDTYPE_SPELL; })));
	count[3].CreateFromString("Artifacts : 0");
	count[4].CreateFromString("TOTAL : " + std::to_string(collections->GetCardCount()));
	for (int i = 0; i < 4; ++i)
		renderer.Render(count[i], 1, i + 1);
	renderer.Render(count[4], 18, 1);

	//Sidebar
	RenderSidebar(renderer);

	//Page number
	renderer.Render(pageNumber, 66, 47);

}

//Render sidebar
void CardViewer::RenderSidebar(Renderer& renderer) {
	int y = 1;
	int i = (pos.y * 9) + pos.x + (page * 54);
	if (i < collections->GetCardCount())
		collections->RenderCardDetails(renderer, 66, y, i);
}

#pragma endregion

#pragma region Input

//Input
void CardViewer::Input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { MoveCursor(0, -1); }       //W
	else if (asciiVal == 97 || asciiVal == 65) { MoveCursor(-1, 0); }   //A
	else if (asciiVal == 115 || asciiVal == 83) { MoveCursor(0, 1); }   //S
	else if (asciiVal == 100 || asciiVal == 68) { MoveCursor(1, 0); }   //D

	//Add current card to decks
	else if (asciiVal == 32) {                                          //Space
		int i = (pos.y * 9) + pos.x + (page * 54);
		if (i < collections->GetCardCount()) {
			game->GetTurnPlayer()->AddNewToDeck(collections->GetCardIDAtIndex(i));
			game->GetNonTurnPlayer()->AddNewToDeck(collections->GetCardIDAtIndex(i));
		}
	}

	//Re-init game
	else if (asciiVal == 10 || asciiVal == 13) {                        //Enter
		game->GetTurnPlayer()->InitStartingHand();
		game->GetNonTurnPlayer()->InitStartingHand();
	}

	//Switch mode
	else if (asciiVal == 80 || asciiVal == 112) { *modeSwitch = true; } //P

}

//Move cursor position
void CardViewer::MoveCursor(int x, int y) {
	if (pos.x + x == 9) {
		page = (page + 1 + pageCount) % pageCount;
		pageNumber.CreateFromString(std::to_string(page + 1) + " / " + std::to_string(pageCount));
	}
	else if (pos.x + x == -1) {
		page = (page - 1 + pageCount) % pageCount;
		pageNumber.CreateFromString(std::to_string(page + 1) + " / " + std::to_string(pageCount));
	}
	pos.x = (pos.x + x + 9) % 9;
	pos.y = (pos.y + y + 6) % 6;
}

#pragma endregion