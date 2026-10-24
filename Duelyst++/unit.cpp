//Include
#include "boardtile.h"
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "effect.h"
#include "event.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "utils.h"

#pragma region Constructors / Initialization

//Unit constructors
Unit::Unit(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	this->tribe = definition.tribe;
	this->baseCost = definition.cost;
	this->cost = definition.cost;
	this->baseAtk = definition.atk;
	this->atk = definition.atk;
	this->baseHpMax = definition.hp;
	this->hpMax = definition.hp;
	this->hp = definition.hp;
	this->isToken = definition.isToken;
	baseMoveRange = 2;
	moveRange = 2;
	keywords = KEYWORD_NONE;
	destroyed = false;
	isDead = false;
	hasMoved = false;
	hasAttacked = false;
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	forcefieldBroken = false;
	curTile = nullptr;
	if (definition.effect != EFFECT_NONE && game != nullptr) {
		Effect* effect = game->collections->GetNewCardEffectInstance(definition.effect);
		if (effect != nullptr)
			effects.push_back(effect);
	}
	UpdateModifiers();
	UpdateAuras();
}
Unit::~Unit() {}

#pragma endregion

#pragma region Rendering

//Render unit on board
void Unit::RenderOnBoard(Renderer& renderer, int x, int y) {

	//Draw sprite
	hasAttacked ? sprite.SetColor(COLOR_GRAY) : sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite, x, y);

	//Update and draw ATK/HP sprites
	atkSprite.CreateFromString(std::to_string(atk), COLOR_GREEN);
	hpSprite.CreateFromString(std::to_string(hp), COLOR_RED);
	renderer.Render(atkSprite, x, y + 4);
	renderer.Render(hpSprite, x + (5 - hpSprite.GetWidth()), y + 4);

	//Draw forcefield if applicable
	if (HasKeyword(KEYWORD_FORCEFIELD) && !forcefieldBroken && game != nullptr)
		game->DrawForcefield(x, y, renderer);

}

//Update stat line and draw details
void Unit::DrawDetails(Renderer& renderer, int& y) {
	if (tribe == TRIBE_GENERAL)
		Utils::GenerateStatSprite(detailStatSprite, atk, hp);
	else
		Utils::GenerateStatSprite(detailStatSprite, cost, atk, hp);
	Card::DrawDetails(renderer, y);
}

#pragma endregion

#pragma region Updates

//Check if unit has died
void Unit::Update(bool& shouldLoop) {
	if (hp < 1 || destroyed) {
		isDead = true;
		game->QueueEvent(new OnDeathEvent(game->GetTurnCount(), this));
		shouldLoop = true;
	}
}

//Update attack/health/cost when buffs change
void Unit::UpdateModifiers() {
	int hpDelta = hp - hpMax;
	cost = PlayableCard::GetStat(STAT_COST, baseCost, true);
	atk = PlayableCard::GetStat(STAT_ATK, baseAtk, true);
	hpMax = PlayableCard::GetStat(STAT_HP, baseHpMax, true);
	moveRange = PlayableCard::GetStat(STAT_MOVE, baseMoveRange, true);
	hp = hpMax + hpDelta;
	keywords = KEYWORD_NONE;
	for (Effect* effect : effects)
		keywords |= effect->ReadKeywords();
	for (Effect* aura : auras)
		keywords |= aura->ReadKeywords();
}

//Get stat
int Unit::GetStat(eStat stat, bool withAuras) {
	switch (stat) {
		case STAT_COST: return PlayableCard::GetStat(STAT_COST, baseCost,      withAuras);
		case STAT_ATK:  return PlayableCard::GetStat(STAT_ATK,  baseAtk,       withAuras);
		case STAT_HP:   return PlayableCard::GetStat(STAT_HP,   baseHpMax,     withAuras);
		case STAT_MOVE: return PlayableCard::GetStat(STAT_MOVE, baseMoveRange, withAuras);
	}
	return -1;
}

#pragma endregion

#pragma region Actions

//Get targets
std::vector<BoardTile*> Unit::GetTargets() {
	if (HasKeyword(KEYWORD_AIRDROP))
		return game->map.GetAllEmpty();
	return game->map.GetEmptyNearAllies(owner);
}

//Initialize states
void Unit::InitState() {
	hasMoved = !HasKeyword(KEYWORD_RUSH);
	hasAttacked = !HasKeyword(KEYWORD_RUSH);
	hasCelerityMoved = !HasAllKeywords(KEYWORD_RUSH | KEYWORD_CELERITY);
	hasCelerityAttacked = !HasAllKeywords(KEYWORD_RUSH | KEYWORD_CELERITY);
	forcefieldBroken = false;
}

//Set sprite position
void Unit::SetTile(BoardTile* tile) {
	OnThisTileUnitChangedEvent tileEvent = OnThisTileUnitChangedEvent();
	if (curTile != nullptr && curTile->unit == this) {
		curTile->unit = nullptr;
		if (curTile->HasTile())
			curTile->tile->TriggerUnitChanged();
	}
	curTile = tile;
	tile->unit = this;
	if (curTile->HasTile())
		curTile->tile->TriggerUnitChanged();
}

//Move on board
void Unit::MoveToTile(BoardTile* tile, bool byEffect) {
	SetTile(tile);
	if (!byEffect) {
		if (!hasCelerityMoved)
			hasCelerityMoved = true;
		else
			hasMoved = true;
	}
	game->QueueEvent(new OnMoveEvent(game->GetTurnCount(), this, byEffect));
}

//Attack enemy
void Unit::Attack(Unit* target, bool counter) {

	//Initialize damage value
	int damage = atk;

	//Damage all adjacent units if this has Frenzy and is targeting an adjacent tile
	if (!counter && HasKeyword(KEYWORD_FRENZY) && curTile->IsNear(target->curTile)) {
		for (Unit* unit : game->map.GetUnitsNear(curTile, FILTER_ALL, FILTER_ENEMY, owner))
			unit->DealDamage(this, damage);
	}

	//Otherwise just damage target
	else
		target->DealDamage(this, damage);

	//Exhaust this if attacking
	if (!counter) {
		if (!hasCelerityAttacked && !hasMoved) {
			hasCelerityMoved = true;
			hasCelerityAttacked = true;
		}
		else {
			hasMoved = true;
			hasAttacked = true;
			hasCelerityMoved = true;
			hasCelerityAttacked = true;
		}
	}

}

//Exhaust this
void Unit::Exhaust() {
	hasMoved = true;
	hasCelerityMoved = true;
	hasAttacked = true;
	hasCelerityAttacked = true;
}

//Refresh state
void Unit::Refresh() {
	hasMoved = false;
	hasAttacked = false;
	hasCelerityMoved = !HasKeyword(KEYWORD_CELERITY);
	hasCelerityAttacked = !HasKeyword(KEYWORD_CELERITY);
	forcefieldBroken = false;
}

//Deal damage to this
int Unit::DealDamage(Card* source, int damage) {

	//Create event and notify
	OnDamageEvent* damageEvent = new OnDamageEvent(game->GetTurnCount(), source, this, damage);
	game->eventManager.NotifyPreEvent(game, damageEvent);

	//Ensure damage is greater than zero
	if (damageEvent->damage > 0) {

		//Negate if target has forcefield
		if (HasKeyword(KEYWORD_FORCEFIELD) && !forcefieldBroken) {
			forcefieldBroken = true;
			damageEvent->damage = 0;
		}

		//Otherwise deal damage
		else {
			hp -= damageEvent->damage;
			if (hp <= 0)
				Destroy(source);
			game->QueueEvent(damageEvent);
		}

	}

	//Return damage value
	return damageEvent->damage;

}

//Heal this
int Unit::Heal(Card* source, int heal) {

	//Create event and notify
	OnHealEvent* healEvent = new OnHealEvent(game->GetTurnCount(), source, this, heal);
	game->eventManager.NotifyPreEvent(game, healEvent);

	//Ensure healing is greater than zero
	if (healEvent->heal > 0) {

		//Cap healing at current health delta
		if (hp + healEvent->heal > hpMax) {
			healEvent->heal = hpMax - hp;
			hp = hpMax;
		}

		//Otherwise heal full amount
		else
			hp += healEvent->heal;

		//Queue event if any healing was done
		if (healEvent->heal > 0)
			game->QueueEvent(healEvent);

	}

	//Return heal value
	return healEvent->heal;

}

//Destroy this
void Unit::Destroy(Card* source) {
	destroyed = true;
}

//Dispel unit
void Unit::Dispel() {

	//Remove effects
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i]->ReadIsRemovable()) {
			RemoveEffect(effects[i]);
			--i;
		}
	}

	//Add dispelled effect
	AddEffect(EFFECT_DISPELLED);

	//Remove misc
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	forcefieldBroken = true;

	//Update
	UpdateModifiers();

}

//Trigger OnAddThis on effects when summoned
void Unit::AddEffects() {
	OnAddThisEvent addEvent = OnAddThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &addEvent);
}

//Trigger OnRemoveThis on effects when leaving the field
void Unit::RemoveEffects() {
	OnRemoveThisEvent removeEvent = OnRemoveThisEvent();
	for (int i = 0; i < effects.size(); ++i)
		effects[i]->TriggerSelfEvent(EffectContext(effects[i], this, game), &removeEvent);
}

#pragma endregion

#pragma region Utils

//Can unit attack target
bool Unit::CanAttack(Unit* target) {
	if (target != nullptr) {
		if (atk > 0) {
			if (HasKeyword(KEYWORD_RANGED))
				return true;
			else if (abs(curTile->pos.x - target->curTile->pos.x) < 2 && abs(curTile->pos.y - target->curTile->pos.y) < 2)
				return true;
		}
	}
	return false;
}

//Can unit move
bool Unit::IsMoveable() {
	if (tribe == TRIBE_STRUCTURE)
		return false;
	return true;
}

//Check if unit has all given keywords
bool Unit::HasAllKeywords(int match) { return (keywords & match) == match; }
bool Unit::HasAnyKeywords(int match) { return (keywords & match) != 0; }

//Is unit being provoked
bool Unit::IsProvoked() {
	for (int i = max(curTile->pos.x - 1, 0); i < min(curTile->pos.x + 2, 9); ++i)
		for (int j = max(curTile->pos.y - 1, 0); j < min(curTile->pos.y + 2, 5); ++j)
			if (game->map.tiles[i][j].unit != nullptr)
				if (game->map.tiles[i][j].unit->owner != owner)
					if (game->map.tiles[i][j].unit->HasKeyword(KEYWORD_PROVOKE))
						return true;
	return false;
}

#pragma endregion