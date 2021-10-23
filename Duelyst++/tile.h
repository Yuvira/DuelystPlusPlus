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

//Tile class
class Tile {
public:
	Tile();
	~Tile();
	void setFeature(eFeature f);
	void select(bool b);
	bool selected;
	eFeature feature;
	Unit* unit;
	Sprite border;
	Sprite sprite;
};

//Map (tile container)
class Map {
public:
	Map();
	~Map();
	Tile tile[9][5];
};

#endif