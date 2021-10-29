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
	void preset(CardList& cl, Game* g);
	void init(int _mana);
	void updateMana(eColor col);
	void render(Renderer& rm, bool left);
	void shuffle();
	void draw();
	void replace(int i);
	void onSummon(Unit* u);
	void onDeath(Unit* u);
	void onAttack(Unit* u1, Unit* u2);
	void onDamage(Unit* u1, Unit* u2);
	void onTurnEnd(Player* p);
	void onTurnStart(Player* p);
	int mana;
	int manaMax;
	int replaces;
	Sprite uiReplace;
	Sprite uiCrystal[9];
	Unit* general;
	Game* game;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif