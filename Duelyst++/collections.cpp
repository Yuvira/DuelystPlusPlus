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
	RegisterArtifacts();
	RegisterCards();
	RegisterTiles();
	for (const eCard& key : cardDefinitions | std::ranges::views::keys)
		cardIds.push_back(key);
	std::sort(cardIds.begin(), cardIds.end());
}
Collections::~Collections() {}

#pragma endregion

#pragma region Instance Generators

//Get a new effect instance based on definition with ID
Effect* Collections::GetNewCardEffectInstance(Game* game, eEffect effectId) {
	if (!effectDefinitions.contains(effectId) || effectDefinitions.at(effectId).type != EFFECTTYPE_CARD)
		return nullptr;
	Effect* effect = new Effect(effectDefinitions.at(effectId));
	AddEffectInstance(game, effect);
	return effect;
}

//Get a new aura instance based on definition with ID
Aura* Collections::GetNewAuraInstance(Game* game, eEffect auraId, Player* owner, Card* sourceCard, Effect* sourceEffect) {
	if (!effectDefinitions.contains(auraId) || effectDefinitions.at(auraId).type != EFFECTTYPE_AURA)
		return nullptr;
	Aura* aura = new Aura(effectDefinitions.at(auraId), owner, sourceCard, sourceEffect);
	AddEffectInstance(game, aura);
	return aura;
}

//Get a new effect instance based on definition with ID
SpellEffect* Collections::GetNewSpellEffectInstance(Game* game, eEffect effectId) {
	if (!effectDefinitions.contains(effectId) || effectDefinitions.at(effectId).type != EFFECTTYPE_SPELL)
		return nullptr;
	SpellEffect* effect = new SpellEffect(effectDefinitions.at(effectId));
	AddEffectInstance(game, effect);
	return effect;
}

//Track new instance
void Collections::AddEffectInstance(Game* game, Effect* effect) {
	if (!effectInstances.contains(game))
		effectInstances[game] = { effect };
	else
		effectInstances[game].push_back(effect);
}

//Get a new card instance based on definition with ID
Card* Collections::GetNewCardInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId))
		return nullptr;
	if (cardDefinitions.at(cardId).type == CARDTYPE_UNIT) {
		Unit* unit = new Unit(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, unit);
		return unit;
	}
	else if (cardDefinitions.at(cardId).type == CARDTYPE_SPELL) {
		Spell* spell = new Spell(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, spell);
		return spell;
	}
	else if (cardDefinitions.at(cardId).type == CARDTYPE_ARTIFACT) {
		Artifact* artifact = new Artifact(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, artifact);
		return artifact;
	}
	else if (cardDefinitions.at(cardId).type == CARDTYPE_TILE) {
		Tile* tile = new Tile(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, tile);
		return tile;
	}
	return nullptr;
}

//Get a new unit instance based on definition with ID
PlayableCard* Collections::GetNewPlayableCardInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId))
		return nullptr;
	if (cardDefinitions.at(cardId).type == CARDTYPE_UNIT) {
		Unit* unit = new Unit(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, unit);
		return unit;
	}
	else if (cardDefinitions.at(cardId).type == CARDTYPE_SPELL) {
		Spell* spell = new Spell(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, spell);
		return spell;
	}
	else if (cardDefinitions.at(cardId).type == CARDTYPE_ARTIFACT) {
		Artifact* artifact = new Artifact(cardDefinitions.at(cardId), game, player);
		AddCardInstance(game, artifact);
		return artifact;
	}
	return nullptr;
}

//Get a new unit instance based on definition with ID
Unit* Collections::GetNewUnitInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId) || cardDefinitions.at(cardId).type != CARDTYPE_UNIT)
		return nullptr;
	Unit* unit = new Unit(cardDefinitions.at(cardId), game, player);
	AddCardInstance(game, unit);
	return unit;
}

//Get a new artifact instance based on definition with ID
Artifact* Collections::GetNewArtifactInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId) || cardDefinitions.at(cardId).type != CARDTYPE_ARTIFACT)
		return nullptr;
	Artifact* artifact = new Artifact(cardDefinitions.at(cardId), game, player);
	AddCardInstance(game, artifact);
	return artifact;
}

//Get a new unit instance based on definition with ID
Tile* Collections::GetNewTileInstance(eCard cardId, Game* game, Player* player) {
	if (!cardDefinitions.contains(cardId) || cardDefinitions.at(cardId).type != CARDTYPE_TILE)
		return nullptr;
	Tile* tile = new Tile(cardDefinitions.at(cardId), game, player);
	AddCardInstance(game, tile);
	return tile;
}

//Track new instance
void Collections::AddCardInstance(Game* game, Card* card) {
	if (!cardInstances.contains(game))
		cardInstances[game] = { card };
	else
		cardInstances[game].push_back(card);
}

//Instance deletion
void Collections::DeleteInstancesForGame(Game* game) {
	if (cardInstances.contains(game)) {
		for (Card* card : cardInstances[game])
			delete card;
		cardInstances.erase(game);
	}
	if (effectInstances.contains(game)) {
		for (Effect* effect : effectInstances[game])
			delete effect;
		effectInstances.erase(game);
	}
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
	if (cardDefinitions.at(cardId).detailStatSprite.GetSize() > 0) {
		renderer.Render(cardDefinitions.at(cardId).detailStatSprite, x + 6, y);
		++y;
	}
	++y;

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

	//Starting y position
	int startY = y;

	//Draw details
	renderer.Render(card->ReadSprite(), x, y);
	renderer.Render(card->ReadDetailNameSprite(), x + 6, y);
	++y;
	if (card->ReadDetailStatSprite().GetSize() > 0) {
		renderer.Render(card->ReadDetailStatSprite(), x + 6, y);
		++y;
	}
	++y;

	//Draw effects
	descriptionSprite.CreateFromTextBlock(card->ReadDetails());
	renderer.Render(descriptionSprite, x + 6, y);
	y += descriptionSprite.GetHeight() + 1;

	//Draw tokens
	std::vector<eCard> tokens = card->GetTokens();
	if (tokens.size() > 0) {
		y = max(y, startY + 6);
		startY = y + 2;
		renderer.Render(tokenDividerSprite, x, y);
	}
	for (eCard tokenId : tokens) {
		y += 2;
		RenderCardDetails(renderer, x, y, tokenId);
	}

	//Draw tile
	if (card->IsUnit() && card->GetUnit()->IsOnBoard() && card->GetUnit()->curTile->HasTile()) {
		y = max(y, startY + 6);
		renderer.Render(tileDividerSprite, x, y);
		y += 2;
		card->GetUnit()->curTile->tile->DrawDetails(renderer, y);
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