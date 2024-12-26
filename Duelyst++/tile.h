//Defines
#ifndef __TILE_H__
#define __TILE_H__

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
	BoardTile* GetRandom();
	BoardTile* GetRandom(BoardTile* ignore);
	BoardTile* GetRandomCorner();
	BoardTile* GetRandomNear(int x, int y);
	BoardTile tiles[9][5];
};

#endif