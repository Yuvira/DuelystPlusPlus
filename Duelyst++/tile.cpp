//Include
#include "tile.h"

//Tile constructor / destructor
Tile::Tile() { sprite.resize(5, 5); } 
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
		sprite.buffer[7].Char.AsciiChar = 'Ü';
		sprite.buffer[11].Char.AsciiChar = 'Þ';
		sprite.buffer[13].Char.AsciiChar = 'Ý';
		sprite.buffer[17].Char.AsciiChar = 'ß';
		sprite.buffer[7].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[11].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[13].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		sprite.buffer[17].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}

}

//Map constructor / destructor
Map::Map() {
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			tile[a][b].sprite.pos.X = (6 * a) + 2;
			tile[a][b].sprite.pos.Y = (6 * b) + 2;
		}
	}
	tile[4][0].setFeature(TILE_MANA);
	tile[5][2].setFeature(TILE_MANA);
	tile[4][4].setFeature(TILE_MANA);
}
Map::~Map() {}