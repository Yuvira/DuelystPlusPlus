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
	if (definition.effect != EFFECT_NONE && game != nullptr) {
		SpellEffect* effect = game->collections->GetNewSpellEffectInstance(definition.effect);
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
void Spell::UpdateModifiers() { cost = GetStat(STAT_COST, baseCost, true); }

//Get targets
std::vector<BoardTile*> Spell::GetTargets() {
	if (spellEffect)
		return spellEffect->GetTargets(game, this);
	return std::vector<BoardTile*>{};
}