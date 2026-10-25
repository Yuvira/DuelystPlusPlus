#pragma once

//Include
#include <unordered_map>
#include "cardids.h"
#include "effectids.h"
#include "sprite.h";

//Define
class CardDefinition;
class Card;
class Effect;
class EffectDefinition;
class Renderer;
class Spell;
class Unit;

//Collections class
class Collections {
public:

	//Constructors
	Collections();
	~Collections();

	//Effect instance generators
	Effect* GetNewCardEffectInstance(eEffect effectId);
	Aura* GetNewAuraInstance(eEffect auraId, Player* owner, Card* sourceCard, Effect* sourceEffect);
	SpellEffect* GetNewSpellEffectInstance(eEffect effectId);

	//Card instance generators
	Card* GetNewCardInstance(eCard cardId, Game* game, Player* player);
	PlayableCard* GetNewPlayableCardInstance(eCard cardId, Game* game, Player* player);
	Unit* GetNewUnitInstance(eCard cardId, Game* game, Player* player);
	Artifact* GetNewArtifactInstance(eCard cardId, Game* game, Player* player);
	Tile* GetNewTileInstance(eCard cardId, Game* game, Player* player);

	//Rendering
	void RenderCardSprite(Renderer& renderer, int x, int y, int cardIdx);
	void RenderCardSprite(Renderer& renderer, int x, int y, eCard cardId);
	void RenderCardDetails(Renderer& renderer, int x, int& y, int cardIdx);
	void RenderCardDetails(Renderer& renderer, int x, int& y, eCard cardId);
	void RenderCardDetails(Renderer& renderer, int x, int& y, Card* card);

	//Utils
	eCard GetRandomTokenMinionID();
	int GetCardCount() { return cardIds.size(); }
	int CountCardsWhere(bool (*Predicate)(const CardDefinition& def));
	eCard GetCardIDAtIndex(int idx) {
		if (idx < 0 || idx >= cardIds.size())
			return CARD_NONE;
		return cardIds[idx];
	}


private:

	//Registers
	void RegisterEffects();
	void RegisterSpells();
	void RegisterArtifacts();
	void RegisterTiles();
	void RegisterCards();

	//Definition maps
	std::unordered_map<eCard, CardDefinition> cardDefinitions;
	std::unordered_map<eEffect, EffectDefinition> effectDefinitions;

	//ID list
	std::vector<eCard> cardIds;

	//Rendering sprites
	inline static const Sprite tileDividerSprite{ "컴TILE컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�" };
	inline static const Sprite tokenDividerSprite{ "컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴" };
	inline static Sprite descriptionSprite;

};