#pragma once

//Include
#include <ranges>
#include "card.h"

//Collections class
class Collections {
public:
	Collections();
	~Collections();
	Effect* FindEffect(eEffect effect);
	Card* FindCard(eCard card);
	Minion* GetRandomTokenMinion();
	std::unordered_map<eEffect, Effect> effects;
	std::unordered_map<eCard, Card*> cards;
	std::vector<Card*> cardList;
	std::vector<Minion> minionList;
	std::vector<Spell> spellList;
	int cardCount;
	int generalCount;
	int minionCount;
	int spellCount;
};