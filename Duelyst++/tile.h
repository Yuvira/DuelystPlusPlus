#pragma once

//Include
#include <vector>
#include "renderer.h"

//Definition
class Minion;

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
	Coord(int _x, int _y);
	~Coord();
	int x;
	int y;
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
	eFeature feature;
	Minion* minion;
	Sprite sprite;
	Coord pos;
};

//Map (tiles container)
class Map {
public:
	Map();
	~Map();
	BoardTile* GetTile(int x, int y);
	BoardTile* GetRandom();
	BoardTile* GetRandom(BoardTile* ignore);
	BoardTile* GetRandomCorner();
	BoardTile* GetRandomNear(BoardTile* tile, bool empty);
	std::vector<BoardTile*> GetNear(BoardTile* tile);
	BoardTile tiles[9][5];
};