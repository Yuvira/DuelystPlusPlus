//Include
#include "boardtile.h"
#include "card.h"

#pragma region Board Tiles

//Board tile constructor
BoardTile::BoardTile() {
	unit = nullptr;
	tile = nullptr;
	borderColor = COLOR_LTWHITE;
}
BoardTile::~BoardTile() {}

//Utilities
bool BoardTile::IsNear(BoardTile* tile) { return (abs(tile->pos.x - pos.x) < 2 && abs(tile->pos.y - pos.y) < 2); }
bool BoardTile::HasUnit() { return unit != nullptr && !unit->isDead; }
bool BoardTile::HasTile() { return tile != nullptr; }

#pragma endregion

#pragma region Maps

//Map constructor
Map::Map() {
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			tiles[i][j].pos = Coord(i, j);
}
Map::~Map() {}

//Get tile at co-ordinates
BoardTile* Map::GetTile(Coord xy) { return GetTile(xy.x, xy.y); }
BoardTile* Map::GetTile(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return nullptr;
	return &tiles[x][y];
}

//Get all tiles
std::vector<BoardTile*> Map::GetAll() {
	std::vector<BoardTile*> valid;
	for (BoardTile(&column)[height] : tiles)
		for (BoardTile& tile : column)
			valid.push_back(&tile);
	return valid;
}

//Get all empty tiles
std::vector<BoardTile*> Map::GetAllEmpty() { return GetAllEmpty(nullptr); }
std::vector<BoardTile*> Map::GetAllEmpty(BoardTile* ignore) {
	std::vector<BoardTile*> valid;
	for (BoardTile (&column)[height] : tiles)
		for (BoardTile& tile : column)
			if (!tile.HasUnit() && &tile != ignore)
				valid.push_back(&tile);
	return valid;
}

//Get random empty tile
BoardTile* Map::GetRandomEmpty() { return GetRandomEmpty(nullptr); }
BoardTile* Map::GetRandomEmpty(BoardTile* ignore) {
	std::vector<BoardTile*> valid = GetAllEmpty(ignore);
	if (valid.size() > 0)
		return valid[rand() % valid.size()];
	return nullptr;
}

//Get all units on board with optional filter
std::vector<Unit*> Map::GetAllUnits() { return GetAllUnits(FILTER_ALL, FILTER_NEUTRAL, nullptr); }
std::vector<Unit*> Map::GetAllUnits(eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player) {
	std::vector<Unit*> valid;
	for (BoardTile(&column)[height] : tiles)
		for (BoardTile& tile : column)
			if (tile.HasUnit() && FilterUnit(tile.unit, type, allegiance, player))
				valid.push_back(tile.unit);
	return valid;
}

//Get all tiles with units with optional filter
std::vector<BoardTile*> Map::GetAllUnitTiles() { return GetAllUnitTiles(FILTER_ALL, FILTER_NEUTRAL, nullptr); }
std::vector<BoardTile*> Map::GetAllUnitTiles(eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player) {
	std::vector<BoardTile*> valid;
	for (BoardTile(&column)[height] : tiles)
		for (BoardTile& tile : column)
			if (tile.HasUnit() && FilterUnit(tile.unit, type, allegiance, player))
				valid.push_back(&tile);
	return valid;
}

//Get all empty corners
std::vector<BoardTile*> Map::GetEmptyCorners() {
	std::vector<BoardTile*> valid;
	if (!tiles[0][0].HasUnit()) { valid.push_back(&tiles[0][0]); }
	if (!tiles[width - 1][0].HasUnit()) { valid.push_back(&tiles[width - 1][0]); }
	if (!tiles[0][height - 1].HasUnit()) { valid.push_back(&tiles[0][height - 1]); }
	if (!tiles[width - 1][height - 1].HasUnit()) { valid.push_back(&tiles[width - 1][height - 1]); }
	return valid;
}

//Get random empty corner
BoardTile* Map::GetRandomEmptyCorner() {
	std::vector<BoardTile*> valid = GetEmptyCorners();
	if (valid.size() > 0)
		return valid[rand() % valid.size()];
	return nullptr;
}

//Get all tiles near a given tile
std::vector<BoardTile*> Map::GetAllNear(BoardTile* tile) {
	std::vector<BoardTile*> valid;
	if (tile != nullptr) {
		NearbyRange range = NearbyRange(tile->pos, 1, width, height);
		for (int i = range.x0; i <= range.x1; ++i)
			for (int j = range.y0; j <= range.y1; ++j)
				if (&tiles[i][j] != tile)
					valid.push_back(&tiles[i][j]);
	}
	return valid;
}

//Get empty tiles near a given tile
std::vector<BoardTile*> Map::GetEmptyNear(BoardTile* tile) {
	std::vector<BoardTile*> valid;
	if (tile != nullptr) {
		NearbyRange range = NearbyRange(tile->pos, 1, width, height);
		for (int i = range.x0; i <= range.x1; ++i)
			for (int j = range.y0; j <= range.y1; ++j)
				if (&tiles[i][j] != tile && !tiles[i][j].HasUnit())
					valid.push_back(&tiles[i][j]);
	}
	return valid;
}

//Get units near a given tile with optional filter
std::vector<Unit*> Map::GetUnitsNear(BoardTile* tile) { return GetUnitsNear(tile, FILTER_ALL, FILTER_NEUTRAL, nullptr); }
std::vector<Unit*> Map::GetUnitsNear(BoardTile* tile, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player) {
	std::vector<Unit*> valid;
	if (tile != nullptr) {
		NearbyRange range = NearbyRange(tile->pos, 1, width, height);
		for (int i = range.x0; i <= range.x1; ++i)
			for (int j = range.y0; j <= range.y1; ++j)
				if (&tiles[i][j] != tile && tiles[i][j].HasUnit() && FilterUnit(tiles[i][j].unit, type, allegiance, player))
					valid.push_back(tiles[i][j].unit);
	}
	return valid;
}

//Get tiles with units near a given tile with optional filter
std::vector<BoardTile*> Map::GetUnitTilesNear(BoardTile* tile) { return GetUnitTilesNear(tile, FILTER_ALL, FILTER_NEUTRAL, nullptr); }
std::vector<BoardTile*> Map::GetUnitTilesNear(BoardTile* tile, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player) {
	std::vector<BoardTile*> valid;
	if (tile != nullptr) {
		NearbyRange range = NearbyRange(tile->pos, 1, width, height);
		for (int i = range.x0; i <= range.x1; ++i)
			for (int j = range.y0; j <= range.y1; ++j)
				if (&tiles[i][j] != tile && tiles[i][j].HasUnit() && FilterUnit(tiles[i][j].unit, type, allegiance, player))
					valid.push_back(&tiles[i][j]);
	}
	return valid;
}

//Get random empty tile near a given tile
BoardTile* Map::GetRandomEmptyNear(BoardTile* tile) {
	std::vector<BoardTile*> valid = GetEmptyNear(tile);
	if (valid.size() > 0)
		return valid[rand() % valid.size()];
	return nullptr;
}

//Get empty tiles within range of a given tile
std::vector<BoardTile*> Map::GetEmptyInRange(BoardTile* tile, int rad, BoardTile* ignore) {
	std::vector<BoardTile*> valid;
	if (tile != nullptr) {
		NearbyRange range = NearbyRange(tile->pos, rad, width, height);
		for (int i = range.x0; i <= range.x1; ++i)
			for (int j = range.y0; j <= range.y1; ++j)
				if (abs(i - tile->pos.x) + abs(j - tile->pos.y) <= rad && &tiles[i][j] != tile && &tiles[i][j] != ignore && !tiles[i][j].HasUnit())
					valid.push_back(&tiles[i][j]);
	}
	return valid;
}

//Get empty tiles near units allied to given player
std::vector<BoardTile*> Map::GetEmptyNearAllies(Player* player) {
	std::vector<BoardTile*> valid;
	for (BoardTile(&column)[height] : tiles)
		for (BoardTile& tile : column)
			if (tile.HasUnit() && tile.unit->owner == player)
				ConcatUnique(valid, GetEmptyNear(&tile));
	return valid;
}

#pragma endregion

//Add unique tiles to list
void Map::ConcatUnique(std::vector<BoardTile*>& base, const std::vector<BoardTile*>& add) {
	for (BoardTile* tile : add)
		if (std::find(base.begin(), base.end(), tile) == base.end())
			base.push_back(tile);
}

//Filter unit by type and allegiance, returns true if unit passes checks
bool Map::FilterUnit(Unit* unit, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player) {
	if (type == FILTER_GENERALS && unit->IsMinion()) return false;
	if (type == FILTER_MINIONS && unit->IsGeneral()) return false;
	if (allegiance == FILTER_ALLY && unit->owner != player) return false;
	if (allegiance == FILTER_ENEMY && unit->owner == player) return false;
	return true;
}