//Defines
#ifndef __PLAYER_H__
#define __PLAYER_H__

//Include
#include <cstdlib>
#include "collections.h"

//Player class
class Player {
public:
	Player();
	~Player();
	void Preset(Collections* collections, Game* _game);
	void Init(int _mana);
	void UpdateMana(eColor color);
	void Render(Renderer& renderer, bool left);
	void Shuffle();
	void Draw();
	void AddToHand(Card* card, bool cast);
	void Replace(int idx);
	int mana;
	int manaMax;
	int replaces;
	Sprite uiReplace;
	Sprite uiCrystal[9];
	Minion* general;
	Game* game;
	Player* opponent;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
};

#endif