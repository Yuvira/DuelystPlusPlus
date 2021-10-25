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
	void init(CardList& cl, int _mana);
	void updateMana(eColor col);
	void shuffle();
	int mana;
	Sprite crystal[9];
	Card* general;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif