//Defines
#ifndef __PLAYER_H__
#define __PLAYER_H__

//Include
#include <cstdlib>
#include "cardlist.h"

//Player class
class Player {
public:
	Player();
	~Player();
	void init(CardList& cl);
	void shuffle();
	int mana;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif