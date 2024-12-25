//Include
#include "collection.h"

//Game constructor / destructor
Collection::Collection() {

	//Border
	board.CreateFromFile("resources/border.txt");

	//Tile positions
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 6; ++j) {
			tiles[i][j].border.pos.X = (i * 7) + 1;
			tiles[i][j].border.pos.Y = (j * 7) + 6;
		}
	}

	//Variables
	pos = Coord(0, 0);
	page = 0;
	pageCount = (cardList.cardList.size() + 53) / 54;
	pageNumber.CreateFromString(std::to_string(page + 1) + " / " + std::to_string(pageCount));
	modeSwitch = nullptr;

}
Collection::~Collection() {}

//Input
void Collection::Input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { MoveCursor(0, -1); }     //W
	else if (asciiVal == 97 || asciiVal == 65) { MoveCursor(-1, 0); } //A
	else if (asciiVal == 115 || asciiVal == 83) { MoveCursor(0, 1); } //S
	else if (asciiVal == 100 || asciiVal == 68) { MoveCursor(1, 0); } //D

	//Switch mode
	else if (asciiVal == 80 || asciiVal == 112) { *modeSwitch = true; } //P

}

//Update loop
void Collection::Update() {

	//Clear tiles highlights
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 6; ++j)
			tiles[i][j].SetColor(COLOR_LTWHITE);

	//Highlight cursor position
	tiles[pos.x][pos.y].SetColor(COLOR_AQUA);

}

//Render objects
void Collection::RenderCollection(Renderer& renderer) {

	//Board
	renderer.Render(board);

	//Tiles
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 6; ++j)
			renderer.Render(tiles[i][j].border);

	//Cards
	for (int a = page * 54; a < min((page + 1) * 54, cardList.cardList.size()); ++a)
		renderer.Render(cardList.cardList[a]->sprite, (((a - (page * 54)) % 9) * 7) + 2, (((a - (page * 54)) / 9) * 7) + 7);

	//Card counts
	count[0].CreateFromString("Generals  : " + std::to_string(cardList.generalList.size()));
	count[1].CreateFromString("Units     : " + std::to_string(cardList.minionList.size()));
	count[2].CreateFromString("Spells    : " + std::to_string(cardList.spellList.size()));
	count[3].CreateFromString("Artifacts : 0");
	count[4].CreateFromString("TOTAL : " + std::to_string(cardList.cardList.size()));
	for (int i = 0; i < 4; ++i)
		renderer.Render(count[i], 1, i + 1);
	renderer.Render(count[4], 18, 1);

	//Sidebar
	RenderSidebar(renderer);

	//Page number
	renderer.Render(pageNumber, 66, 47);

}

//Render sidebar
void Collection::RenderSidebar(Renderer& renderer) {
	int y = 1;
	int i = (pos.y * 9) + pos.x + (page * 54);
	if (i < cardList.cardList.size())
		cardList.cardList[i]->DrawDetails(renderer, y);
}

//Move cursor position
void Collection::MoveCursor(int x, int y) {
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