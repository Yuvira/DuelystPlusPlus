#pragma once

//Include
#include <vector>
#include "sprite.h"

//Define
class Game;
class Collections;
class Renderer;
class Card;
class Unit;
enum eCard;

//Player class
class Player {
public:

	//Constructor
	Player(Game* game, int mana);
	~Player();

	//Setup
	void InitStartingHand();

	//Rendering
	void UpdateMana(eColor color);
	void Render(Renderer& renderer, bool left);

	//Actions
	void Shuffle();
	void Draw();
	void AddNewToHand(eCard cardId);
	void AddNewToDeck(eCard cardId);
	void Replace(int idx);
	void ModifyReplaceCount(int delta);
	bool RemoveFromDeck(Card* card);

	//Getters
	std::vector<Card*> GetDeckAndHand();

	//Utils
	bool HasCardInHand(Card* match);

	//Properties
	int manaMax;
	int mana;
	int maxReplaces;
	int replaces;
	Game* game;
	Unit* general;
	Player* opponent;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
	Sprite uiReplace;
	Sprite uiCrystal[9];

};