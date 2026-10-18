//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "event.h"
#include "game.h"
#include "player.h"

#pragma region Constructor

//Card list constructor
Collections::Collections() {

	//Registries
	RegisterEffects();
	RegisterSpells();
	RegisterCards();

	//Generate card map
	for (int i = 0; i < unitList.size(); ++i) {
		cards[unitList[i].cardId] = &unitList[i];
		cards[unitList[i].cardId]->original = cards[unitList[i].cardId];
		cardList.push_back(cards[unitList[i].cardId]);
	}
	for (int i = 0; i < spellList.size(); ++i) {
		cards[spellList[i].cardId] = &spellList[i];
		cards[spellList[i].cardId]->original = cards[spellList[i].cardId];
		cardList.push_back(cards[spellList[i].cardId]);
	}

	//Set original references
	for (int i = 0; i < cardList.size(); ++i) { cardList[i]->original = cardList[i]; }

	//Assign tokens
	cards[CARD_DIOLTAS]->effects[0]->SetToken(cards[CARD_TOMBSTONE]);
	cards[CARD_FIRESTARTER]->effects[0]->SetToken(cards[CARD_SPELLSPARK]);
	cards[CARD_JAX_TRUESIGHT]->effects[0]->SetToken(cards[CARD_MINI_JAX]);
	cards[CARD_JAXI]->effects[0]->SetToken(cards[CARD_MINI_JAX]);

}
Collections::~Collections() {}

#pragma endregion

#pragma region Search Functions

//Find effect by enum
Effect* Collections::FindEffect(eEffect effect) {
	if (effects.contains(effect))
		return &effects[effect];
	return nullptr;
}

//Find card by name
Card* Collections::FindCard(eCard card) {
	if (cards.contains(card))
		return cards[card];
	return nullptr;
}

//Get random token minion
Unit* Collections::GetRandomTokenMinion() {
	std::vector<Unit*> valid;
	for (int i = 0; i < unitList.size(); ++i)
		if (unitList[i].isToken)
			valid.push_back(&unitList[i]);
	if (valid.size() > 0)
		return valid[rand() % valid.size()];
	return nullptr;
}

#pragma endregion

#pragma region Utils

//Getters
const std::vector<Card*>& Collections::GetCards() const { return cardList; }

//Get card count
int Collections::GetCardCount() { return cardList.size(); }

//Count cards matching predicate
int Collections::CountCardsWhere(bool (*Predicate)(Card*)) {
	if (!Predicate)
		return cardList.size();
	int count = 0;
	for (Card* card : cardList)
		if (Predicate(card))
			++count;
	return count;
}

#pragma endregion