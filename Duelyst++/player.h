#pragma once

//Include
#include <vector>
#include "sprite.h"

//Define
class Game;
class Collections;
class Renderer;
class Card;
class PlayableCard;
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
	void Render(Renderer& renderer, bool left);
	void UpdateMana(eColor color);

	//Actions
	void Shuffle();
	void Draw();
	void AddNewToHand(eCard cardId);
	void AddNewToDeck(eCard cardId);
	void Replace(int idx);
	bool RemoveFromHand(int idx);
	bool RemoveFromHand(PlayableCard* card);
	bool RemoveFromDeck(int idx);
	bool RemoveFromDeck(PlayableCard* card);

	//Resource management
	void AddMana(int add);
	void AddMaxMana(int add);
	bool SpendMana(int spend);
	void AddMaxReplaces(int add);
	void Refresh();

	//Readers
	const int& ReadMana() const { return mana; }

	//Utils
	std::vector<PlayableCard*> GetDeckAndHand();
	bool HasCardInHand(Card* match);

	//Properties
	Game* game;
	Unit* general;
	Player* opponent;
	std::vector<PlayableCard*> deck;
	std::vector<PlayableCard*> hand;

private:

	//Properties
	int manaMax;
	int mana;
	int maxReplaces;
	int replaces;
	Sprite uiReplace;
	Sprite uiCrystal[9];

};