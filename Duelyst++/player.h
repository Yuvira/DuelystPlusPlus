//Defines
#ifndef __PLAYER_H__
#define __PLAYER_H__

//Include
#include <cstdlib>
#include "card.h"

//Player class
class Player {
public:
	Player();
	~Player();
	void preset(CardList& cl);
	void init(int _mana);
	void updateMana(eColor col);
	void shuffle();
	int mana;
	int manaMax;
	Sprite crystal[9];
	Unit* general;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif