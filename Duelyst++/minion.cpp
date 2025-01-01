//Include
#include "game.h"

#pragma region Constructors / Initialization

//Minion constructors
Minion::Minion() : Minion(FACTION_NEUTRAL, TRIBE_NONE, 0, 0, 0, "", "???") {}
Minion::Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name) {
	cardType = CARD_MINION;
	faction = _faction;
	tribe = _tribe;
	targetMode = TargetMode(TARGET_MODE_NEAR_ALLIES, TARGET_FILTER_EMPTY);
	cost = _cost;
	atk = _atk;
	hp = _hp;
	hpMax = _hp;
	name = _name;
	isDead = false;
	hasMoved = false;
	hasAttacked = false;
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	hasForcefield = false;
	if (path == "")
		sprite.Resize(5, 5);
	else
		sprite.CreateFromFile("resources/minions/" + path + ".txt");
	UpdateStatSprites();
	GenerateDetails();
	curTile = nullptr;
	game = nullptr;
	owner = nullptr;
}
Minion::Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name, Effect effect) : Minion(_faction, _tribe, _cost, _atk, _hp, path, _name) {
	if (effect.effect != EFFECT_NONE)
		AddEffect(effect, nullptr);
}
Minion::~Minion() {}

//Generate sidebar details
void Minion::GenerateDetails() {
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
void Minion::Render(Renderer& renderer) {
	hasAttacked ? sprite.SetColor(COLOR_GRAY) : sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite);
	renderer.Render(atkSprite);
	renderer.Render(hpSprite);
	if (hasForcefield) {
		int x = sprite.pos.X;
		int y = sprite.pos.Y;
		renderer.Render(game->chars[7], x + 1, y + 1);
		renderer.Render(game->chars[9], x + 2, y + 1);
		renderer.Render(game->chars[6], x + 3, y + 1);
		renderer.Render(game->chars[8], x + 1, y + 2);
		renderer.Render(game->chars[8], x + 3, y + 2);
		renderer.Render(game->chars[6], x + 1, y + 3);
		renderer.Render(game->chars[9], x + 2, y + 3);
		renderer.Render(game->chars[7], x + 3, y + 3);
	}
}

//Draw card data
void Minion::DrawDetails(Renderer& renderer, int& y) {
	sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite, 66, y);
	renderer.Render(header[0], 72, y); ++y;
	UpdateDetailStats();
	renderer.Render(header[1], 72, y); y += 2;
	renderer.Render(details, 72, y); y += details.height + 1;
	if (token != nullptr) {
		if (y < 7)
			y = 7;
		renderer.Render(divider, 66, y);
		y += 2;
		token->DrawDetails(renderer, y);
	}
}

#pragma endregion

#pragma region Updates

//Check if minion has died
void Minion::Update(bool& shouldLoop) {
	if (hp < 1) {
		game->eventManager.SendOnDeath(this);
		isDead = true;
		shouldLoop = true;
	}
}

//Update attack/health/cost when buffs change
void Minion::UpdateStatBuffs() {
	int costBuff = 0;
	int atkBuff = 0;
	int hpBuff = 0;
	int hpDelta = hp - hpMax;
	for (int i = 0; i < effects.size(); ++i) {
		costBuff += effects[i].costBuff;
		atkBuff += effects[i].atkBuff;
		hpBuff += effects[i].hpBuff;
	}
	cost = max(original->GetMinion()->cost + costBuff, 0);
	atk = max(original->GetMinion()->atk + atkBuff, 0);
	hpMax = original->GetMinion()->hpMax + hpBuff;
	hp = hpMax + hpDelta;
}

//Update HP & ATK sprites
void Minion::UpdateStatSprites() {
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
void Minion::UpdateDetailStats() {
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

#pragma region Actions

//Set sprite position
void Minion::SetPosition(int x, int y) {
	if (curTile != nullptr) { curTile->minion = nullptr; }
	int spriteX = (x * 7) + 2;
	int spriteY = (y * 7) + 6;
	sprite.pos.X = spriteX;
	sprite.pos.Y = spriteY;
	atkSprite.pos.X = spriteX;
	atkSprite.pos.Y = spriteY + 4;
	hpSprite.pos.X = (spriteX + 4) - (hpSprite.buffer.size() - 1);
	hpSprite.pos.Y = spriteY + 4;
	curTile = &game->map.tiles[x][y];
	curTile->minion = this;
	if (curTile->feature == TILE_MANA) {
		curTile->SetFeature(TILE_NONE);
		if (owner->mana < 9) { ++owner->mana; }
	}
}

//Attack enemy
void Minion::Attack(Minion* target, bool counter) {
	int damage = atk;
	target->DealDamage(this, damage);
	game->eventManager.SendOnAttack(this, target, counter);
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
		if (target->CanAttack(this))
			target->Attack(this, true);
	}
}

//Deal damage to this
int Minion::DealDamage(Minion* source, int damage) {
	if (damage < 0) {
		if (hp - damage > hpMax) {
			damage = -(hpMax - hp);
			hp = hpMax;
		}
		else
			hp -= damage;
		game->eventManager.SendOnHeal(source, this, -damage);
	}
	else {
		if (hasForcefield) {
			hasForcefield = false;
			damage = 0;
		}
		else {
			hp -= damage;
			game->eventManager.SendOnDamage(source, this, damage);
		}
	}
	return damage;
}

//Dispel minion
void Minion::Dispel() {

	//Dispel effects
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDispelThis)
			effects[i].OnDispelThis(EffectContext(&effects[i], this, game));

	//Remove effects
	for (int i = 0; i < effects.size(); ++i)
		if (!effects[i].IsContinuous())
			effects.erase(effects.begin() + i);

	//Add dispelled effect
	AddEffect(game->collections->FindEffect(EFFECT_DISPELLED), nullptr);

	//Remove misc
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	hasForcefield = false;

	//Update
	UpdateStatBuffs();

}

#pragma endregion

#pragma region Utils

//Can minion attack target
bool Minion::CanAttack(Minion* target) {
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

//Can minion move
bool Minion::IsMoveable() {
	if (tribe == TRIBE_STRUCTURE)
		return false;
	return true;
}

//Minion range
int Minion::MoveRange() {
	int range = 2;
	for (int i = 0; i < effects.size(); ++i) {
		switch (effects[i].effect) {
		case EFFECT_GOLDEN_JUSTICAR:
			range += 2;
		}
	}
	return range;
}

//Check if minion has given keywords
bool Minion::HasKeywords(int keywords) {
	int flags = KEYWORD_NONE;
	for (int i = 0; i < effects.size(); ++i)
		flags |= effects[i].keywords;
	return (flags & keywords) == keywords;
}

//Is minion being provoked
bool Minion::IsProvoked() {
	for (int i = max(curTile->pos.x - 1, 0); i < min(curTile->pos.x + 2, 9); ++i)
		for (int j = max(curTile->pos.y - 1, 0); j < min(curTile->pos.y + 2, 5); ++j)
			if (game->map.tiles[i][j].minion != nullptr)
				if (game->map.tiles[i][j].minion->owner != owner)
					if (game->map.tiles[i][j].minion->HasKeywords(KEYWORD_PROVOKE))
						return true;
	return false;
}

#pragma endregion

#pragma region Action & Event Overrides

//Summon minion on cast resolution
void Minion::Resolve(BoardTile* tile) {

	//Trigger any effects on this card
	Card::Resolve(tile);

	//Summon this
	game->Summon(this, tile, true);

	//Exhaust
	hasMoved = !HasKeywords(KEYWORD_RUSH);
	hasAttacked = !HasKeywords(KEYWORD_RUSH);
	hasCelerityMoved = !HasKeywords(KEYWORD_RUSH) || !HasKeywords(KEYWORD_CELERITY);
	hasCelerityAttacked = !HasKeywords(KEYWORD_RUSH) || !HasKeywords(KEYWORD_CELERITY);
	hasForcefield = HasKeywords(KEYWORD_FORCEFIELD);

}

//When a player's turn ends
void Minion::OnTurnEnd(Player* player) {

	//Trigger any effects on this card
	Card::OnTurnEnd(player);

	//Refresh
	hasMoved = false;
	hasAttacked = false;
	hasCelerityMoved = !HasKeywords(KEYWORD_CELERITY);
	hasCelerityAttacked = !HasKeywords(KEYWORD_CELERITY);
	hasForcefield = HasKeywords(KEYWORD_FORCEFIELD);

}

#pragma endregion