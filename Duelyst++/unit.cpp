//Include
#include <algorithm>
#include "card.h"
#include "game.h"
#include "tile.h"
#include "player.h"
#include "effect.h"
#include "event.h"
#include "renderer.h"

#pragma region Constructors / Initialization

//Unit constructors
Unit::Unit()
	: Unit(CARD_NONE, FACTION_NEUTRAL, TRIBE_NONE, 0, 0, 0, "", "???", false, nullptr) {}
Unit::Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name)
	: Unit(cardId, faction, tribe, cost, atk, hp, path, name, false, nullptr) {}
Unit::Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken)
	: Unit(cardId, faction, tribe, cost, atk, hp, path, name, isToken, nullptr) {}
Unit::Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, Effect* effect)
	: Unit(cardId, faction, tribe, cost, atk, hp, path, name, false, effect) {}
Unit::Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken, Effect* effect) {
	this->cardId = cardId;
	this->faction = faction;
	this->tribe = tribe;
	baseCost = cost;
	this->cost = cost;
	baseAtk = atk;
	this->atk = atk;
	this->hp = hp;
	baseHpMax = hp;
	hpMax = hp;
	baseMoveRange = 2;
	moveRange = 2;
	this->isToken = isToken;
	this->name = name;
	destroyed = false;
	isDead = false;
	hasMoved = false;
	hasAttacked = false;
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	forcefieldBroken = false;
	if (path == "")
		sprite.Resize(5, 5);
	else
		sprite.CreateFromFile("resources/minions/" + path + ".txt");
	UpdateStatSprites();
	GenerateDetails();
	curTile = nullptr;
	game = nullptr;
	owner = nullptr;
	if (effect != nullptr && effect->GetID() != EFFECT_NONE)
		AddEffect(*effect, this);
}
Unit::~Unit() {}

//Generate sidebar details
void Unit::GenerateDetails() {
	std::string str;
	str.resize(name.size());
	std::transform(name.begin(), name.end(), str.begin(), ::toupper);
	switch (tribe) {
	case TRIBE_NONE:
		str += " - MINION";
		break;
	case TRIBE_GENERAL:
		str += " - GENERAL";
		break;
	case TRIBE_ARCANYST:
		str += " - ARCANYST";
		break;
	case TRIBE_PET:
		str += " - PET";
		break;
	case TRIBE_GOLEM:
		str += " - GOLEM";
		break;
	case TRIBE_MECH:
		str += " - MECH";
		break;
	case TRIBE_DERVISH:
		str += " - DERVISH";
		break;
	case TRIBE_VESPYR:
		str += " - VESPYR";
		break;
	case TRIBE_STRUCTURE:
		str += " - STRUCTURE";
		break;
	case TRIBE_WARMASTER:
		str += " - WARMASTER";
		break;
	}
	header[0].CreateFromString(str);
	for (int i = name.size(); i < header[0].buffer.size(); ++i)
		header[0].buffer[i].Attributes = COLOR_GRAY;
	UpdateDetailStats();
}

#pragma endregion

#pragma region Rendering

//Render sprites
void Unit::Render(Renderer& renderer) {
	hasAttacked ? sprite.SetColor(COLOR_GRAY) : sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite);
	renderer.Render(atkSprite);
	renderer.Render(hpSprite);
	if (HasKeywords(KEYWORD_FORCEFIELD) && !forcefieldBroken && game != nullptr)
		game->DrawForcefield(sprite.pos.X, sprite.pos.Y, renderer);
}

//Draw card data
void Unit::DrawDetails(Renderer& renderer, int& y) {
	sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite, 66, y);
	renderer.Render(header[0], 72, y); ++y;
	UpdateDetailStats();
	renderer.Render(header[1], 72, y); y += 2;
	UpdateDetails();
	renderer.Render(details, 72, y); y += details.height + 1;
	bool tokens = false;
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i]->GetToken() != nullptr) {
			if (!tokens) {
				if (y < 7)
					y = 7;
				renderer.Render(divider, 66, y);
				tokens = true;
			}
			y += 2;
			effects[i]->GetToken()->DrawDetails(renderer, y);
		}
	}
}

//Update HP & ATK sprites
void Unit::UpdateStatSprites() {
	std::string str = std::to_string(atk);
	atkSprite.Resize(str.length(), 1);
	for (int i = 0; i < str.length(); ++i)
		atkSprite.buffer[i].Char.AsciiChar = str[i];
	atkSprite.SetColor(COLOR_GREEN);
	int width = hpSprite.buffer.size();
	str = std::to_string(hp);
	hpSprite.Resize(str.length(), 1);
	for (int i = 0; i < str.length(); ++i)
		hpSprite.buffer[i].Char.AsciiChar = str[i];
	hpSprite.SetColor(COLOR_RED);
	hpSprite.pos.X += width - str.length();
}

//Update card stats
void Unit::UpdateDetailStats() {
	eColor color = COLOR_LTBLUE;
	std::string str = "";
	if (tribe != TRIBE_GENERAL)
		str = "COST:" + std::to_string(cost) + " ATK:" + std::to_string(atk) + " HP:" + std::to_string(hp);
	else {
		str = "ATK:" + std::to_string(atk) + " HP:" + std::to_string(hp);
		color = COLOR_GREEN;
	}
	header[1].CreateFromString(str);
	for (int i = 0; i < header[1].buffer.size(); ++i) {
		if (str[i] != ':' && str[i] != ' ')
			header[1].buffer[i].Attributes = color;
		if (str[i] == ' ') {
			if (color == COLOR_LTBLUE)
				color = COLOR_GREEN;
			else
				color = COLOR_RED;
		}
	}
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
	cost = GetStat(STAT_COST, baseCost);
	atk = GetStat(STAT_ATK, baseAtk);
	hpMax = GetStat(STAT_HP, baseHpMax);
	moveRange = GetStat(STAT_MOVE, baseMoveRange);
	hp = hpMax + hpDelta;
	keywords = KEYWORD_NONE;
	for (Effect* effect : effects)
		keywords |= effect->GetKeywords();
	for (Effect* aura : auras)
		keywords |= aura->GetKeywords();
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
void Unit::SetPosition(int x, int y) {
	if (curTile != nullptr) { curTile->unit = nullptr; }
	int spriteX = (x * 7) + 2;
	int spriteY = (y * 7) + 6;
	sprite.pos.X = spriteX;
	sprite.pos.Y = spriteY;
	atkSprite.pos.X = spriteX;
	atkSprite.pos.Y = spriteY + 4;
	hpSprite.pos.X = (spriteX + 4) - (hpSprite.buffer.size() - 1);
	hpSprite.pos.Y = spriteY + 4;
	curTile = &game->map.tiles[x][y];
	curTile->unit = this;
	if (curTile->feature == TILE_MANA) {
		curTile->SetFeature(TILE_NONE);
		if (owner->mana < 9) { ++owner->mana; }
	}
}

//Move on board
void Unit::MoveToTile(BoardTile* tile, bool byEffect) { MoveToPosition(tile->pos.x, tile->pos.y, byEffect); }
void Unit::MoveToPosition(int x, int y, bool byEffect) {
	SetPosition(x, y);
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