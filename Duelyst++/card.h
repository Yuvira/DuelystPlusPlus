//Defines
#ifndef __CARD_H__
#define __CARD_H__

//Include
#include <algorithm>
#include <vector>
#include "tile.h"

//Card types
enum eCard {
	CARD_UNIT,
	CARD_SPELL,
	CARD_ARTIFACT
};

//Rarities
enum eRarity {
	RARITY_NONE,
	RARITY_COMMON,
	RARITY_RARE,
	RARITY_EPIC,
	RARITY_LEGENDARY
};

//Targeting types
enum eTarget {
	TARGET_ANY,
	TARGET_UNIT,
	TARGET_ALLY,
	TARGET_ENEMY,
	TARGET_NEAR_UNIT,
	TARGET_NEAR_ALLY,
};

//Card class
class Card {
public:
	Card();
	~Card();
	virtual void drawCard(Renderer& rm, int& y);
	int cost;
	eCard type;
	std::string name;
	Sprite sprite;
	int descriptionSize;
	std::vector<Sprite> description;
};

#endif