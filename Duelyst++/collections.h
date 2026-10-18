#pragma once

//Include
#include <unordered_map>
#include <vector>
#include "cardids.h"
#include "effectids.h"

//Define
class Card;
class Unit;
class Spell;
class Effect;

//Collections class
class Collections {
public:

	//Constructors
	Collections();
	~Collections();

	//Search functions
	Effect* FindEffect(eEffect effect);
	Card* FindCard(eCard card);
	Unit* GetRandomTokenMinion();

	//Getters
	const std::vector<Card*>& GetCards() const;

	//Utils
	int GetCardCount();
	int CountCardsWhere(bool (*Predicate)(Card*));

private:

	//Registers
	void RegisterEffects();
	void RegisterSpells();
	void RegisterCards();

	//Object containers
	std::unordered_map<eEffect, Effect> effects;
	std::vector<Unit> unitList;
	std::vector<Spell> spellList;

	//Reference lists
	std::unordered_map<eCard, Card*> cards;
	std::vector<Card*> cardList;

};