//Include
#include "card.h"
#include "carddefinition.h"
#include "effect.h"
#include "renderer.h"
#include "utils.h"

#pragma region Constructors / Initialization

//Game constructors
Spell::Spell(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	this->targetMode = definition.targetMode;
	UpdateModifiers();
}
Spell::~Spell() {}

#pragma endregion

#pragma region Rendering

//Update stat line and draw details
void Spell::DrawDetails(Renderer& renderer, int& y) {
	Utils::GenerateStatSprite(detailStatSprite, cost);
	Card::DrawDetails(renderer, y);
}

#pragma endregion

#pragma region Updates

//Update cost when buffs change
void Spell::UpdateModifiers() { cost = GetStat(STAT_COST, baseCost, true); }

#pragma endregion

#pragma region Setters

//Target mode
void Spell::SetTargetMode(TargetMode targetMode) { this->targetMode = targetMode; }

#pragma endregion