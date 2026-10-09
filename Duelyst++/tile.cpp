//Include
#include "tile.h"

#pragma region Coords

//Custom co-ord
Coord::Coord() : Coord(0, 0) {}
Coord::Coord(int _x, int _y) {
	x = _x;
	y = _y;
}
Coord::~Coord() {}

#pragma endregion

#pragma region Tiles

//Tile constructor
Tile::Tile() { 
	border.CreateFromFile("resources/tile.txt");
	SetColor(COLOR_LTWHITE);
} 
Tile::~Tile() {}

//Select tiles
void Tile::SetColor(eColor color) { border.SetColor(color); }

#pragma endregion

#pragma region Board Tiles

//Board tile constructor
BoardTile::BoardTile() : Tile() {
	sprite.Resize(5, 5);
	SetFeature(TILE_NONE);
	minion = nullptr;
}
BoardTile::~BoardTile() {}

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

//Check if a tile is adjacent to this one
bool BoardTile::IsNear(BoardTile* tile) { return (abs(tile->pos.x - pos.x) < 2 && abs(tile->pos.y - pos.y) < 2); }

#pragma endregion

#pragma region Maps

//Map constructor
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

//Get tile at co-ordinates
BoardTile* Map::GetTile(int x, int y) {
	if (x < 0 || x > 8 || y < 0 || y > 4)
		return nullptr;
	return &tiles[x][y];
}

//Get random empty tile
BoardTile* Map::GetRandomEmpty() { return GetRandomEmpty(nullptr, nullptr); }
BoardTile* Map::GetRandomEmpty(BoardTile* ignore) { return GetRandomEmpty(ignore, nullptr); }
BoardTile* Map::GetRandomEmpty(BoardTile* ignore1, BoardTile* ignore2) {
	std::vector<BoardTile*> valid;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 5; ++j)
			if (tiles[i][j].minion == nullptr && &tiles[i][j] != ignore1 && &tiles[i][j] != ignore2)
				valid.push_back(&tiles[i][j]);
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Get random empty corner
BoardTile* Map::GetRandomEmptyCorner() {
	std::vector<BoardTile*> valid;
	if (tiles[0][0].minion == nullptr) { valid.push_back(&tiles[0][0]); }
	if (tiles[8][0].minion == nullptr) { valid.push_back(&tiles[8][0]); }
	if (tiles[0][4].minion == nullptr) { valid.push_back(&tiles[0][4]); }
	if (tiles[8][4].minion == nullptr) { valid.push_back(&tiles[8][4]); }
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Get random empty tile near a given tile
BoardTile* Map::GetRandomEmptyNear(BoardTile* tile) {
	if (tile == nullptr)
		return nullptr;
	std::vector<BoardTile*> valid;
	for (int i = max(tile->pos.x - 1, 0); i < min(tile->pos.x + 2, 9); ++i)
		for (int j = max(tile->pos.y - 1, 0); j < min(tile->pos.y + 2, 5); ++j)
			if (&tiles[i][j] != tile && tiles[i][j].minion == nullptr)
				valid.push_back(&tiles[i][j]);
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

//Get all empty corners
std::vector<BoardTile*> Map::GetEmptyCorners() {
	std::vector<BoardTile*> valid;
	if (tiles[0][0].minion == nullptr) { valid.push_back(&tiles[0][0]); }
	if (tiles[8][0].minion == nullptr) { valid.push_back(&tiles[8][0]); }
	if (tiles[0][4].minion == nullptr) { valid.push_back(&tiles[0][4]); }
	if (tiles[8][4].minion == nullptr) { valid.push_back(&tiles[8][4]); }
	return valid;
}

//Get all tiles near a given tile
std::vector<BoardTile*> Map::GetAllNear(BoardTile* tile) {
	std::vector<BoardTile*> valid;
	if (tile != nullptr)
		for (int i = max(tile->pos.x - 1, 0); i < min(tile->pos.x + 2, 9); ++i)
			for (int j = max(tile->pos.y - 1, 0); j < min(tile->pos.y + 2, 5); ++j)
				if (&tiles[i][j] != tile)
					valid.push_back(&tiles[i][j]);
	return valid;
}

#pragma endregion