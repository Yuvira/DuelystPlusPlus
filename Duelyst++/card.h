//Defines
#ifndef __CARD_H__
#define __CARD_H__

//Include
#include "tile.h"

//Card types
enum eCard {
	CARD_UNIT,
	CARD_SPELL,
	CARD_ARTIFACT
};

enum eRarity {
	RARITY_NONE,
	RARITY_COMMON,
	RARITY_RARE,
	RARITY_EPIC,
	RARITY_LEGENDARY
};

//Card class
class Card {
public:
	Card();
	virtual ~Card();
	eCard type;
	Sprite sprite;
	Sprite desc;
};

#endif