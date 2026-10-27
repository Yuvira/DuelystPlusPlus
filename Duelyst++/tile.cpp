//Include
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "game.h"

//Game constructors
Tile::Tile(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	if (definition.effect != EFFECT_NONE && game != nullptr) {
		Effect* effect = game->collections->GetNewCardEffectInstance(game, definition.effect);
		if (effect != nullptr)
			effects.push_back(effect);
	}
}
Tile::~Tile() {}

//Trigger OnAddThis on effects when summoned
void Tile::AddEffects() {
	OnAddThisEvent addEvent = OnAddThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &addEvent);
}

//Trigger OnRemoveThis on effects when leaving the field
void Tile::RemoveEffects() {
	OnRemoveThisEvent removeEvent = OnRemoveThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &removeEvent);
}

//Trigger OnThisTileUnitChangedEvent on effects when the unit on this tile changes
void Tile::TriggerUnitChanged() {
	OnThisTileUnitChangedEvent tileEvent = OnThisTileUnitChangedEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &tileEvent);
}