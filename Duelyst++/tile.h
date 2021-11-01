//Defines
#ifndef __TILE_H__
#define __TILE_H__

//Include
#include <vector>
#include "renderer.h"

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

//Custom co-ord because vector of COORD is invalid??
class Coord {
public:
	Coord(int _x = 0, int _y = 0);
	~Coord();
	int x;
	int y;
};

//Tile class
class Tile {
public:
	Tile();
	~Tile();
	void setCol(eColor col);
	Sprite border;
};

class BoardTile : public Tile {
public:
	BoardTile();
	~BoardTile();
	void setFeature(eFeature f);
	eFeature feature;
	Unit* unit;
	Sprite sprite;
	Coord pos;
};

//Map (tile container)
class Map {
public:
	Map();
	~Map();
	BoardTile* getRandom();
	BoardTile* getRandomNear(int x, int y);
	BoardTile tile[9][5];
};

#endif