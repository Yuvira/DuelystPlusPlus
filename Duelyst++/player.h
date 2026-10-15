#pragma once

//Include
#include <vector>
#include "sprite.h"

//Define
class Game;
class Collections;
class Renderer;
class Card;
class Minion;

//Player class
class Player {
public:

	//Constructor
	Player();
	~Player();

	//Setup
	void Preset(Collections* collections, Game* game);
	void Init(int mana);

	//Rendering
	void UpdateMana(eColor color);
	void Render(Renderer& renderer, bool left);

	//Actions
	void Shuffle();
	void Draw();
	void AddNewToHand(Card* original);
	void Replace(int idx);
	void ModifyReplaceCount(int delta);

	//Getters
	std::vector<Card*> GetDeckAndHand();

	//Properties
	int mana;
	int manaMax;
	int replaces;
	int maxReplaces;
	Sprite uiReplace;
	Sprite uiCrystal[9];
	Minion* general;
	Game* game;
	Player* opponent;
	std::vector<Card*> deck;
	std::vector<Card*> hand;

};