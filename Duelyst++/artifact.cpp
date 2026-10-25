//Include
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "effect.h"
#include "game.h"
#include "renderer.h"
#include "utils.h"

//Game constructors
Artifact::Artifact(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	this->baseCost = definition.cost;
	this->cost = definition.cost;
	this->isToken = definition.isToken;
	durability = 3;
	destroyed = false;
	isActive = false;
	if (definition.effect != EFFECT_NONE && game != nullptr) {
		Effect* effect = game->collections->GetNewCardEffectInstance(definition.effect);
		if (effect != nullptr)
			effects.push_back(effect);
	}
	UpdateModifiers();
	UpdateAuras();
}
Artifact::~Artifact() {}

//Update stat line and draw details
void Artifact::DrawDetails(Renderer& renderer, int& y) {
	Utils::GenerateStatSprite(detailStatSprite, cost);
	Card::DrawDetails(renderer, y);
}

//Update cost when buffs change
void Artifact::UpdateModifiers() { cost = PlayableCard::GetStat(STAT_COST, baseCost, true); }

//Get stat
int Artifact::GetStat(eStat stat, bool withAuras) {
	if (stat != STAT_COST)
		return -1;
	return PlayableCard::GetStat(STAT_COST, baseCost, withAuras);
}

//Get targets
std::vector<BoardTile*> Artifact::GetTargets() { return game->map.GetAll(); }

//Deal 1 damage to this
void Artifact::Damage(Card* source) {
	--durability;
	if (durability == 0)
		Destroy(source);
}

//Set this as destroyed
void Artifact::Destroy(Card* source) { destroyed = true; }

//Trigger OnAddThis when added to board
void Artifact::AddEffects() {
	isActive = true;
	OnAddThisEvent addEvent = OnAddThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &addEvent);
}

//Trigger OnRemoveThis when removed from board
void Artifact::RemoveEffects() {
	OnRemoveThisEvent removeEvent = OnRemoveThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &removeEvent);
	isActive = false;
}