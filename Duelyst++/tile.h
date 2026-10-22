#pragma once

//Include
#include <vector>
#include "sprite.h"

//Definition
class Unit;

//Tile features
enum eFeature {
	TILE_NONE,
	TILE_MANA,
	TILE_SAND,
	TILE_HALLOW,
	TILE_FLOURISH,
	TILE_CREEP,
};

//Custom co-ordinate because you can't vector COORD
class Coord {
public:
	Coord();
	Coord(int x, int y);
	~Coord();
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

//Tile class
class Tile {
public:
	Tile();
	~Tile();
	void SetColor(eColor color);
	Sprite border;
};

class BoardTile : public Tile {
public:
	BoardTile();
	~BoardTile();
	void SetFeature(eFeature newFeature);
	void Dispel();
	bool IsNear(BoardTile* tile);
	bool HasUnit() { return unit != nullptr; }
	eFeature feature;
	Unit* unit;
	Sprite sprite;
	Coord pos;
};

//Map (tiles container)
class Map {

public:

	//Constructor
	Map();
	~Map();

	//Get from position
	BoardTile* GetTile(int x, int y);

	//Get all
	std::vector<BoardTile*> GetAllEmpty(BoardTile* ignore);
	BoardTile* GetRandomEmpty();
	BoardTile* GetRandomEmpty(BoardTile* ignore);

	//Get corners
	std::vector<BoardTile*> GetEmptyCorners();
	BoardTile* GetRandomEmptyCorner();

	//Get near
	std::vector<BoardTile*> GetAllNear(BoardTile* tile);
	std::vector<BoardTile*> GetEmptyNear(BoardTile* tile);
	std::vector<Unit*> GetUnitsNear(BoardTile* tile);
	BoardTile* GetRandomEmptyNear(BoardTile* tile);

	//Get in range
	std::vector<BoardTile*> GetEmptyInRange(BoardTile* tile, int rad, BoardTile* ignore);

	//Tile objects
	BoardTile tiles[9][5];

private:

	//Size properties
	inline static const int width = 9;
	inline static const int height = 5;

};