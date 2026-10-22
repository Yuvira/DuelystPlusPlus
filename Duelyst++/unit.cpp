//Include
#include "card.h"
#include "carddefinition.h"
#include "effect.h"
#include "event.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "tile.h"
#include "utils.h"

#pragma region Constructors / Initialization

//Unit constructors
Unit::Unit(const CardDefinition& definition, Game* game, Player* owner) : Card(definition, game, owner) {
	tribe = definition.tribe;
	baseAtk = definition.atk;
	atk = definition.atk;
	baseHpMax = definition.hp;
	hpMax = definition.hp;
	hp = definition.hp;
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
	UpdateModifiers();
}
Unit::~Unit() {}

#pragma endregion

#pragma region Rendering

//Render unit on board
void Unit::RenderOnBoard(Renderer& renderer) {
	if (curTile != nullptr)
		RenderOnBoard(renderer, (curTile->pos.x * 7) + 2, (curTile->pos.y * 7) + 6);
}
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
	if (HasKeywords(KEYWORD_FORCEFIELD) && !forcefieldBroken && game != nullptr)
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
		game->QueueEvent(new OnDeathEvent(game->GetTurnCount(), this));
		isDead = true;
		shouldLoop = true;
	}
}

//Update attack/health/cost when buffs change
void Unit::UpdateModifiers() {
	int hpDelta = hp - hpMax;
	cost = GetStat(STAT_COST, baseCost, true);
	atk = GetStat(STAT_ATK, baseAtk, true);
	hpMax = GetStat(STAT_HP, baseHpMax, true);
	moveRange = GetStat(STAT_MOVE, baseMoveRange, true);
	hp = hpMax + hpDelta;
	keywords = KEYWORD_NONE;
	for (Effect* effect : effects)
		keywords |= effect->ReadKeywords();
	for (Effect* aura : auras)
		keywords |= aura->ReadKeywords();
}

#pragma endregion

#pragma region Actions

//Initialize states
void Unit::InitState() {
	hasMoved = !HasKeywords(KEYWORD_RUSH);
	hasAttacked = !HasKeywords(KEYWORD_RUSH);
	hasCelerityMoved = !HasKeywords(KEYWORD_RUSH) || !HasKeywords(KEYWORD_CELERITY);
	hasCelerityAttacked = !HasKeywords(KEYWORD_RUSH) || !HasKeywords(KEYWORD_CELERITY);
	forcefieldBroken = false;
}

//Set sprite position
void Unit::SetTile(BoardTile* tile) {
	if (curTile != nullptr)
		curTile->unit = nullptr;
	curTile = tile;
	tile->unit = this;
	if (tile->feature == TILE_MANA) {
		tile->SetFeature(TILE_NONE);
		owner->AddMana(1);
	}
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
	if (!counter && HasKeywords(KEYWORD_FRENZY) && curTile->IsNear(target->curTile)) {
		for (BoardTile* tile : game->map.GetAllNear(curTile))
			if (tile->unit != nullptr && tile->unit->owner != owner)
				tile->unit->DealDamage(this, damage);
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
	hasCelerityMoved = !HasKeywords(KEYWORD_CELERITY);
	hasCelerityAttacked = !HasKeywords(KEYWORD_CELERITY);
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
		if (HasKeywords(KEYWORD_FORCEFIELD) && !forcefieldBroken) {
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
		RemoveEffect(effects[i]);
		--i;
	}

	//Add dispelled effect
	AddEffect(EFFECT_DISPELLED, nullptr);

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

//Get targeting mode
TargetMode Unit::GetTargetMode() {
	if (HasKeywords(KEYWORD_AIRDROP))
		return TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) { return tile->unit == nullptr; });
	return TargetMode(TARGET_MODE_NEAR_ALLIES, [](BoardTile* tile) { return tile->unit == nullptr; });
}

//Can unit attack target
bool Unit::CanAttack(Unit* target) {
	if (target != nullptr) {
		if (atk > 0) {
			if (HasKeywords(KEYWORD_RANGED))
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
bool Unit::HasKeywords(int match) { return (keywords & match) == match; }

//Is unit being provoked
bool Unit::IsProvoked() {
	for (int i = max(curTile->pos.x - 1, 0); i < min(curTile->pos.x + 2, 9); ++i)
		for (int j = max(curTile->pos.y - 1, 0); j < min(curTile->pos.y + 2, 5); ++j)
			if (game->map.tiles[i][j].unit != nullptr)
				if (game->map.tiles[i][j].unit->owner != owner)
					if (game->map.tiles[i][j].unit->HasKeywords(KEYWORD_PROVOKE))
						return true;
	return false;
}

#pragma endregion