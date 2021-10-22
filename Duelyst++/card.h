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