#pragma once

//Include
#include <vector>
#include "sprite.h"

//Definition
class Player;
class Tile;
class Unit;

//Unit filters
enum eUnitFilterType {
	FILTER_ALL,
	FILTER_GENERALS,
	FILTER_MINIONS,
};
enum eUnitFilterAllegiance {
	FILTER_NEUTRAL,
	FILTER_ALLY,
	FILTER_ENEMY
};

#pragma region Helpers

//Custom co-ordinate because you can't vector COORD
struct Coord {
	Coord() : Coord(0, 0) {}
	Coord(int x, int y) {
		this->x = x;
		this->y = y;
	}
	~Coord() {}
	int x;
	int y;
};

//Helper for getting tiles in range
struct NearbyRange {
	NearbyRange(Coord pos, int rad, int width, int height) {
		x0 = max(pos.x - rad, 0);
		x1 = min(pos.x + rad, width - 1);
		y0 = max(pos.y - rad, 0);
		y1 = min(pos.y + rad, height - 1);
	}
	int x0, x1, y0, y1;
};

#pragma endregion

//Board tile object
class BoardTile {
public:
	BoardTile();
	~BoardTile();
	bool IsNear(BoardTile* tile);
	bool HasUnit();
	bool HasTile();
	Coord pos;
	Unit* unit;
	Tile* tile;
	eColor borderColor;
};

//Map (tiles container)
class Map {

public:

	//Constructor
	Map();
	~Map();

	//Get from position
	BoardTile* GetTile(Coord xy);
	BoardTile* GetTile(int x, int y);

	//Get all
	std::vector<BoardTile*> GetAll();
	std::vector<BoardTile*> GetAllEmpty();
	std::vector<BoardTile*> GetAllEmpty(BoardTile* ignore);
	std::vector<Unit*> GetAllUnits();
	std::vector<Unit*> GetAllUnits(eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player);
	std::vector<BoardTile*> GetAllUnitTiles();
	std::vector<BoardTile*> GetAllUnitTiles(eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player);
	BoardTile* GetRandomEmpty();
	BoardTile* GetRandomEmpty(BoardTile* ignore);

	//Get corners
	std::vector<BoardTile*> GetEmptyCorners();
	BoardTile* GetRandomEmptyCorner();

	//Get near
	std::vector<BoardTile*> GetAllNear(BoardTile* tile);
	std::vector<BoardTile*> GetEmptyNear(BoardTile* tile);
	std::vector<Unit*> GetUnitsNear(BoardTile* tile);
	std::vector<Unit*> GetUnitsNear(BoardTile* tile, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player);
	std::vector<BoardTile*> GetUnitTilesNear(BoardTile* tile);
	std::vector<BoardTile*> GetUnitTilesNear(BoardTile* tile, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player);
	BoardTile* GetRandomEmptyNear(BoardTile* tile);

	//Get in range
	std::vector<BoardTile*> GetEmptyInRange(BoardTile* tile, int rad, BoardTile* ignore);

	//Get near allies
	std::vector<BoardTile*> GetEmptyNearAllies(Player* player);

	//Utils
	void ConcatUnique(std::vector<BoardTile*>& base, const std::vector<BoardTile*>& add);
	bool FilterUnit(Unit* unit, eUnitFilterType type, eUnitFilterAllegiance allegiance, Player* player);

	//Property getters
	int GetWidth() { return width; }
	int GetHeight() { return height; }

	//Tile objects
	BoardTile tiles[9][5];

private:

	//Size properties
	static constexpr int width = 9;
	static constexpr int height = 5;

};