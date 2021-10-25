//Defines
#ifndef __TILE_H__
#define __TILE_H__

//Include
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
	void setFeature(eFeature f);
	void setCol(WORD col);
	eFeature feature;
	Unit* unit;
	Sprite border;
	Sprite sprite;
	Coord pos;
};

//Map (tile container)
class Map {
public:
	Map();
	~Map();
	Tile tile[9][5];
};

#endif