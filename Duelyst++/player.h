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
	void init(int _mana, Game* g);
	void updateMana(eColor col);
	void render(Renderer& rm, bool left);
	void shuffle();
	void draw();
	int mana;
	int manaMax;
	Sprite crystal[9];
	Unit* general;
	Game* game;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif