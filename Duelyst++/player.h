//Defines
#ifndef __PLAYER_H__
#define __PLAYER_H__

//Include
#include "cardlist.h"

//Player class
class Player {
public:
	Player();
	~Player();
	int mana;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif