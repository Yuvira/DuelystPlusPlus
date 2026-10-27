//Include
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "effect.h"
#include "game.h"
#include "renderer.h"
#include "utils.h"

//Game constructors
Spell::Spell(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	this->baseCost = definition.cost;
	this->cost = definition.cost;
	this->isToken = definition.isToken;
	if (definition.effect != EFFECT_NONE && game != nullptr) {
		SpellEffect* effect = game->collections->GetNewSpellEffectInstance(game, definition.effect);
		if (effect != nullptr) {
			spellEffect = effect;
			effects.push_back(effect);
		}
	}
	UpdateModifiers();
	UpdateAuras();
}
Spell::~Spell() {}

//Update stat line and draw details
void Spell::DrawDetails(Renderer& renderer, int& y) {
	Utils::GenerateStatSprite(detailStatSprite, cost);
	Card::DrawDetails(renderer, y);
}

//Update cost when buffs change
void Spell::UpdateModifiers() { cost = PlayableCard::GetStat(STAT_COST, baseCost, true); }

//Get stat
int Spell::GetStat(eStat stat, bool withAuras) {
	if (stat != STAT_COST)
		return -1;
	return PlayableCard::GetStat(STAT_COST, baseCost, withAuras);
}

//Get targets
std::vector<BoardTile*> Spell::GetTargets() {
	if (spellEffect)
		return spellEffect->GetTargets(game, this);
	return std::vector<BoardTile*>{};
}