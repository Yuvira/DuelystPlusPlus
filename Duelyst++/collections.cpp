//Include
#include <ranges>
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "effect.h"
#include "effectdefinition.h"
#include "event.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "utils.h"

#pragma region Constructor

//Card list constructor
Collections::Collections() {
	RegisterEffects();
	RegisterSpells();
	RegisterCards();
	for (const eCard& key : cardDefinitions | std::ranges::views::keys)
		cardIds.push_back(key);
	std::sort(cardIds.begin(), cardIds.end());
}
Collections::~Collections() {}

#pragma endregion

#pragma region Instance Generators

//Get a new effect instance based on definition with ID
Effect* Collections::GetNewEffectInstance(eEffect effectId) {
	if (!effectDefinitions.contains(effectId))
		return nullptr;
	return new Effect(effectDefinitions.at(effectId));
}

//Get a new card instance based on definition with ID
Card* Collections::GetNewCardInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId))
		return nullptr;
	if (cardDefinitions.at(cardId).type == CARDTYPE_UNIT)
		return new Unit(cardDefinitions.at(cardId), game, player);
	else if (cardDefinitions.at(cardId).type == CARDTYPE_SPELL)
		return new Spell(cardDefinitions.at(cardId), game, player);
	return nullptr;
}

//Get a new unit instance based on definition with ID
Unit* Collections::GetNewUnitInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId) || cardDefinitions.at(cardId).type != CARDTYPE_UNIT)
		return nullptr;
	return new Unit(cardDefinitions.at(cardId), game, player);
}

#pragma endregion

#pragma region Rendering

//Draw sprite for card definition at index in list
void Collections::RenderCardSprite(Renderer& renderer, int x, int y, int cardIdx) {
	if (cardIdx < 0 || cardIdx >= cardIds.size())
		return;
	RenderCardSprite(renderer, x, y, cardIds[cardIdx]);
}

//Draw sprite for card definition with ID
void Collections::RenderCardSprite(Renderer& renderer, int x, int y, eCard cardId) {
	if (!cardDefinitions.contains(cardId))
		return;
	renderer.Render(cardDefinitions.at(cardId).sprite, x, y);
}

//Draw details for card definition at index in list
void Collections::RenderCardDetails(Renderer& renderer, int x, int& y, int cardIdx) {
	if (cardIdx < 0 || cardIdx >= cardIds.size())
		return;
	RenderCardDetails(renderer, x, y, cardIds[cardIdx]);
}

//Draw details for card definition with ID
void Collections::RenderCardDetails(Renderer& renderer, int x, int& y, eCard cardId) {

	//Draw details
	if (!cardDefinitions.contains(cardId))
		return;
	renderer.Render(cardDefinitions.at(cardId).sprite, x, y);
	renderer.Render(cardDefinitions.at(cardId).detailNameSprite, x + 6, y);
	++y;
	renderer.Render(cardDefinitions.at(cardId).detailStatSprite, x + 6, y);
	y += 2;

	//Draw effect
	eEffect effectId = cardDefinitions.at(cardId).effect;
	if (effectId == EFFECT_NONE || !effectDefinitions.contains(effectId))
		return;
	std::vector<std::string> details;
	Utils::WrapText(effectDefinitions.at(effectId).description, details, Utils::MAX_DESCRIPTION_WIDTH);
	effectDefinitions.at(effectId).modifiers.WriteValues(details);
	descriptionSprite.CreateFromTextBlock(details);
	renderer.Render(descriptionSprite, x + 6, y);
	y += descriptionSprite.GetHeight() + 1;

	//Draw tokens
	eCard tokenId = effectDefinitions.at(effectId).token;
	if (tokenId == CARD_NONE)
		return;
	y = max(y, 7);
	renderer.Render(tokenDividerSprite, x, y);
	y += 2;
	RenderCardDetails(renderer, x, y, tokenId);

}

//Draw details for card instance
void Collections::RenderCardDetails(Renderer& renderer, int x, int& y, Card* card) {

	//Draw details
	renderer.Render(card->ReadSprite(), x, y);
	renderer.Render(card->ReadDetailNameSprite(), x + 6, y);
	++y;
	renderer.Render(card->ReadDetailStatSprite(), x + 6, y);
	y += 2;

	//Draw effects
	descriptionSprite.CreateFromTextBlock(card->ReadDetails());
	renderer.Render(descriptionSprite, x + 6, y);
	y += descriptionSprite.GetHeight() + 1;

	//Draw tokens
	std::vector<eCard> tokens = card->GetTokens();
	if (tokens.size() > 0) {
		y = max(y, 7);
		renderer.Render(tokenDividerSprite, x, y);
	}
	for (eCard tokenId : tokens) {
		y += 2;
		RenderCardDetails(renderer, x, y, tokenId);
	}

}

#pragma endregion

#pragma region Utils

//Get random token minion
eCard Collections::GetRandomTokenMinionID() {
	std::vector<eCard> valid;
	for (const CardDefinition& def : cardDefinitions | std::ranges::views::values)
		if (def.isToken)
			valid.push_back(def.cardId);
	if (valid.size() > 0)
		return valid[rand() % valid.size()];
	return CARD_NONE;
}

//Count cards matching predicate
int Collections::CountCardsWhere(bool (*Predicate)(const CardDefinition& def)) {
	int count = 0;
	for (const CardDefinition& def : cardDefinitions | std::ranges::views::values)
		if (Predicate(def))
			++count;
	return count;
}

#pragma endregion