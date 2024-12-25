//Include
#include "tile.h"

//Tile constructor / destructor
Tile::Tile() { 
	border.CreateFromFile("resources/tile.txt");
	SetColor(COLOR_LTWHITE);
} 
Tile::~Tile() {}

BoardTile::BoardTile() : Tile() {
	sprite.Resize(5, 5);
	SetFeature(TILE_NONE);
	minion = nullptr;
}
BoardTile::~BoardTile() {}

//Select tiles
void Tile::SetColor(eColor color) { border.SetColor(color); }

//Set tiles feature
void BoardTile::SetFeature(eFeature newFeature) {

	//Return if already set
	if (feature == newFeature) { return; }

	//Set value
	feature = newFeature;

	//Clear sprites
	sprite.Clear();

	//Empty tiles
	if (feature == TILE_MANA) {
		sprite.buffer[7].Char.AsciiChar = 'Ü';
		sprite.buffer[11].Char.AsciiChar = 'Þ';
		sprite.buffer[13].Char.AsciiChar = 'Ý';
		sprite.buffer[17].Char.AsciiChar = 'ß';
		sprite.SetColor(COLOR_LTBLUE);
	}

}

//Map constructor / destructor
Map::Map() {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 5; ++j) {
			tiles[i][j].border.pos.X = (7 * i) + 1;
			tiles[i][j].border.pos.Y = (7 * j) + 5;
			tiles[i][j].sprite.pos.X = (7 * i) + 2;
			tiles[i][j].sprite.pos.Y = (7 * j) + 6;
			tiles[i][j].pos = Coord(i, j);
		}
	}
	tiles[4][0].SetFeature(TILE_MANA);
	tiles[5][2].SetFeature(TILE_MANA);
	tiles[4][4].SetFeature(TILE_MANA);
}
Map::~Map() {}

//Get random empty tiles
BoardTile* Map::GetRandom() {
	std::vector<BoardTile*> valid;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 5; ++j)
			if (tiles[i][j].minion == nullptr)
				valid.push_back(&tiles[i][j]);
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Get random empty corner
BoardTile* Map::GetRandomCorner() {
	std::vector<BoardTile*> valid;
	if (tiles[0][0].minion == nullptr) { valid.push_back(&tiles[0][0]); }
	else if (tiles[8][0].minion == nullptr) { valid.push_back(&tiles[8][0]); }
	else if (tiles[0][4].minion == nullptr) { valid.push_back(&tiles[0][4]); }
	else if (tiles[8][4].minion == nullptr) { valid.push_back(&tiles[8][4]); }
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Get random empty tiles
BoardTile* Map::GetRandomNear(int x, int y) {
	std::vector<BoardTile*> valid;
	for (int i = max(x - 1, 0); i < min(x + 2, 9); ++i)
		for (int j = max(y - 1, 0); j < min(y + 2, 5); ++j)
			if (tiles[i][j].minion == nullptr)
				valid.push_back(&tiles[i][j]);
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Custom co-ord crap
Coord::Coord() : Coord(0, 0) {}
Coord::Coord(int _x, int _y) {
	x = _x;
	y = _y;
}
Coord::~Coord() {}