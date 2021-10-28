//Include
#include "tile.h"

//Tile constructor / destructor
Tile::Tile() { 
	border.createFromFile("resources/tile.txt");
	setCol(COLOR_LTWHITE);
} 
Tile::~Tile() {}

BoardTile::BoardTile() : Tile() {
	sprite.resize(5, 5);
	setFeature(TILE_NONE);
	unit = nullptr;
}
BoardTile::~BoardTile() {}

//Select tile
void Tile::setCol(eColor col) { border.setCol(col); }

//Set tile feature
void BoardTile::setFeature(eFeature f) {

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
		sprite.setCol(COLOR_LTBLUE);
	}

}

//Map constructor / destructor
Map::Map() {
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			tile[a][b].border.pos.X = (7 * a) + 1;
			tile[a][b].border.pos.Y = (7 * b) + 5;
			tile[a][b].sprite.pos.X = (7 * a) + 2;
			tile[a][b].sprite.pos.Y = (7 * b) + 6;
			tile[a][b].pos = Coord(a, b);
		}
	}
	tile[4][0].setFeature(TILE_MANA);
	tile[5][2].setFeature(TILE_MANA);
	tile[4][4].setFeature(TILE_MANA);
}
Map::~Map() {}

//Get random empty tile
BoardTile* Map::getRandom() {
	std::vector<BoardTile*> v;
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			if (tile[a][b].unit == nullptr) {
				v.push_back(&tile[a][b]);
			}
		}
	}
	if (v.size() > 0) {
		int i = rand() % v.size();
		return v[i];
	}
	return nullptr;
}

//Custom co-ord crap
Coord::Coord(int _x, int _y) {
	x = _x;
	y = _y;
}
Coord::~Coord() {}