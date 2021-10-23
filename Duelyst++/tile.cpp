//Include
#include "tile.h"

//Tile constructor / destructor
Tile::Tile() { 
	border.createFromFile("resources/tile.txt");
	sprite.resize(5, 5); 
	select(false);
} 
Tile::~Tile() {}

//Set tile feature
void Tile::setFeature(eFeature f) {

	//Return if already set
	if (feature == f) { return; }

	//Set value
	feature = f;

	//Clear sprite
	sprite.clear();

	//Empty tile
	if (f == TILE_MANA) {
		sprite.buffer[7].Char.AsciiChar = '�';
		sprite.buffer[11].Char.AsciiChar = '�';
		sprite.buffer[13].Char.AsciiChar = '�';
		sprite.buffer[17].Char.AsciiChar = '�';
		sprite.buffer[7].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[11].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[13].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[17].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}

}

//Select tile
void Tile::select(bool b) {
	WORD col = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	if (b) { col = FOREGROUND_GREEN | FOREGROUND_BLUE; }
	for (int a = 0; a < border.size; ++a) { border.buffer[a].Attributes = col; }
	selected = b;
}

//Map constructor / destructor
Map::Map() {
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			tile[a][b].border.pos.X = (7 * a) + 1;
			tile[a][b].border.pos.Y = (7 * b) + 1;
			tile[a][b].sprite.pos.X = (7 * a) + 2;
			tile[a][b].sprite.pos.Y = (7 * b) + 2;
		}
	}
	tile[4][0].setFeature(TILE_MANA);
	tile[5][2].setFeature(TILE_MANA);
	tile[4][4].setFeature(TILE_MANA);
}
Map::~Map() {}