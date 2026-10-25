#pragma once

//Include
#include <vector>
#include "sprite.h"

//Define
class Artifact;
class Card;
class Collections;
class Game;
class PlayableCard;
class Renderer;
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
	bool Draw();
	void AddNewToHand(eCard cardId);
	void AddNewToDeck(eCard cardId);
	void Replace(int idx);
	bool RemoveFromHand(int idx);
	bool RemoveFromHand(PlayableCard* card);
	bool RemoveFromDeck(int idx);
	bool RemoveFromDeck(PlayableCard* card);
	void AddArtifact(Artifact* artifact);
	void RemoveArtifact(Artifact* artifact);

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
	std::vector<Artifact*> GetArtifacts();
	bool HasCardInHand(Card* match);

	//Properties
	Game* game;
	Unit* general;
	Player* opponent;
	std::vector<PlayableCard*> deck;
	std::vector<PlayableCard*> hand;
	static constexpr int startHandSize = 5;
	static constexpr int maxHandSize = 6;

private:

	//Properties
	int manaMax;
	int mana;
	int maxReplaces;
	int replaces;
	std::vector<Artifact*> artifacts;
	Sprite uiReplace;
	Sprite uiCrystal[9];
	static constexpr int maxArtifacts = 3;

};