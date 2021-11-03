//Include
#include "collection.h"

//Game constructor / destructor
Collection::Collection() {

	//Border
	board.createFromFile("resources/border.txt");

	//Tile positions
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 6; ++b) {
			tile[a][b].border.pos.X = (a * 7) + 1;
			tile[a][b].border.pos.Y = (b * 7) + 6;
		}
	}

	//Variables
	pos = Coord(0, 0);
	page = 0;
	pages = (cl.cList.size() + 53) / 54;
	pageNumber.createFromString(std::to_string(page + 1) + " / " + std::to_string(pages));

}
Collection::~Collection() {}

//Input
void Collection::input() {

	//Get keyPress
	int asciiVal = _getch();

	//Move pointer
	if (asciiVal == 119 || asciiVal == 87) { moveCursor(0, -1); }     //W
	else if (asciiVal == 97 || asciiVal == 65) { moveCursor(-1, 0); } //A
	else if (asciiVal == 115 || asciiVal == 83) { moveCursor(0, 1); } //S
	else if (asciiVal == 100 || asciiVal == 68) { moveCursor(1, 0); } //D

	//Switch mode
	else if (asciiVal == 80 || asciiVal == 112) { *modeSwitch = true; } //P

}

//Update loop
void Collection::update() {

	//Clear tile highlights
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 6; ++b) {
			tile[a][b].setCol(COLOR_LTWHITE);
		}
	}

	//Highlight cursor position
	tile[pos.x][pos.y].setCol(COLOR_AQUA);

}

//Render objects
void Collection::render(Renderer& rm) {

	//Board
	rm.render(board);

	//Tiles
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 6; ++b) {
			rm.render(tile[a][b].border);
		}
	}

	//Cards
	for (int a = page * 54; a < min((page + 1) * 54, cl.cList.size()); ++a) { rm.render(cl.cList[a]->sprite, (((a - (page * 54)) % 9) * 7) + 2, (((a - (page * 54)) / 9) * 7) + 7); }

	//Card counts
	count[0].createFromString("Generals  : " + std::to_string(cl.gList.size()));
	count[1].createFromString("Units     : " + std::to_string(cl.uList.size()));
	count[2].createFromString("Spells    : " + std::to_string(cl.sList.size()));
	count[3].createFromString("Artifacts : 0");
	count[4].createFromString("TOTAL : " + std::to_string(cl.cList.size()));
	for (int a = 0; a < 4; ++a) { rm.render(count[a], 1, a + 1); }
	rm.render(count[4], 18, 1);

	//Sidebar
	renderSidebar(rm);

	//Page number
	rm.render(pageNumber, 66, 47);

}

//Render sidebar
void Collection::renderSidebar(Renderer& rm) {
	int y = 1;
	int i = (pos.y * 9) + pos.x + (page * 54);
	if (i < cl.cList.size()) { cl.cList[i]->drawDetails(rm, y); }
}

//Move cursor position
void Collection::moveCursor(int x, int y) {
	if (pos.x + x == 9) {
		page = (page + 1 + pages) % pages;
		pageNumber.createFromString(std::to_string(page + 1) + " / " + std::to_string(pages));
	}
	else if (pos.x + x == -1) {
		page = (page - 1 + pages) % pages;
		pageNumber.createFromString(std::to_string(page + 1) + " / " + std::to_string(pages));
	}
	pos.x = (pos.x + x + 9) % 9;
	pos.y = (pos.y + y + 6) % 6;
}