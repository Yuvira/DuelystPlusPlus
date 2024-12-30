//Include
#include "game.h"

#pragma region Constructors / Initialization

//Minion constructors
Minion::Minion() : Minion(FACTION_NEUTRAL, TRIBE_NONE, 0, 0, 0, "", "???") {}
Minion::Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name) {
	cardType = CARD_MINION;
	faction = _faction;
	tribe = _tribe;
	targetMode = TARGET_NEAR_ALLY;
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
	for (int i = 0; i < effects.size(); ++i) {
		renderer.Render(effects[i].sprite, 72, y);
		y += effects[i].sprite.height + 1;
	}
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
		costBuff += effects[i].costBuff * effects[i].sources.size();
		atkBuff += effects[i].atkBuff * effects[i].sources.size();
		hpBuff += effects[i].hpBuff * effects[i].sources.size();
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
	/*
	switch (skill.skill) {
	case SKILL_BLUETIP_SCORPION:
		if (target->tribe != TRIBE_GENERAL)
			damage *= 2;
		break;
	case SKILL_FACESTRIKER:
		if (target->tribe == TRIBE_GENERAL)
			damage *= 2;
		break;
	}
	*/
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

	/*

	//Remove buffs granted to other units/cards
	switch (skill.skill) {
	case SKILL_ARCHON_SPELLBINDER:
		for (int a = 0; a < owner->opponent->hand.size(); ++a) {
			if (owner->opponent->hand[a]->cardType == CARD_SPELL) {
				dynamic_cast<Spell*>(owner->opponent->hand[a])->RemoveBuff(BUFF_ARCHON_SPELLBINDER, false);
			}
		}
		for (int a = 0; a < owner->opponent->deck.size(); ++a) {
			if (owner->opponent->deck[a]->cardType == CARD_SPELL) {
				dynamic_cast<Spell*>(owner->opponent->deck[a])->RemoveBuff(BUFF_ARCHON_SPELLBINDER, false);
			}
		}
		break;
	case SKILL_ARROW_WHISTLER:
		for (int a = 0; a < game->minions.size(); ++a) {
			if (game->minions[a]->owner == owner) {
				if (game->minions[a] != this) {
					if (game->minions[a]->IsRanged()) {
						game->minions[a]->RemoveBuff(BUFF_ARROW_WHISTLER, false);
					}
				}
			}
		}
		break;
	case SKILL_FIRST_SWORD_OF_AKRANE:
		for (int a = 0; a < game->minions.size(); ++a) {
			if (game->minions[a]->owner == owner) {
				if (game->minions[a]->tribe != TRIBE_GENERAL) {
					if (game->minions[a] != this) {
						game->minions[a]->RemoveBuff(BUFF_FIRST_SWORD_OF_AKRANE, false);
					}
				}
			}
		}
		break;
	case SKILL_GOLDEN_JUSTICAR:
		for (int a = 0; a < game->minions.size(); ++a) {
			if (game->minions[a]->owner == owner) {
				if (game->minions[a] != this) {
					if (game->minions[a]->IsProvoking()) {
						game->minions[a]->RemoveEffect(EFFECT_GOLDEN_JUSTICAR, false);
					}
				}
			}
		}
		break;
	case SKILL_GOLEM_METALLURGIST:
		for (int a = 0; a < owner->hand.size(); ++a) {
			if (owner->hand[a]->cardType == CARD_UNIT) {
				dynamic_cast<Minion*>(owner->hand[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, false);
			}
		}
		for (int a = 0; a < owner->deck.size(); ++a) {
			if (owner->deck[a]->cardType == CARD_UNIT) {
				dynamic_cast<Minion*>(owner->deck[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, false);
			}
		}
		break;
	case SKILL_GOLEM_VANQUISHER:
		for (int a = 0; a < game->minions.size(); ++a) {
			if (game->minions[a]->owner == owner) {
				if (game->minions[a] != this) {
					game->minions[a]->RemoveEffect(EFFECT_GOLEM_VANQUISHER, false);
				}
			}
		}
		break;
	case SKILL_GROVE_LION:
		for (int a = 0; a < owner->general->effects.size(); ++a) {
			if (owner->general->effects[a].effect == EFFECT_GROVE_LION) {
				if (owner->general->effects[a].stacks == 1) {
					owner->general->hasForcefield = false;
				}
				owner->general->RemoveEffect(EFFECT_GROVE_LION, false);
				break;
			}
		}
		break;
	}

	//Remove skills and buffs on this
	skill = game->cardList.effectList.Find(SKILL_DISPELLED);
	for (int a = 0; a < effects.size(); ++a) {
		switch (effects[a].effect) {
		case EFFECT_DARKFIRE_SACRIFICE:
		case EFFECT_GOLEM_VANQUISHER:
		case EFFECT_GROVE_LION:
		case EFFECT_LADY_LOCKE_A:
			break;
		default:
			effects.erase(effects.begin() + a);
			--a;
			break;
		}
	}
	for (int a = 0; a < buffs.size(); ++a) {
		switch (buffs[a].buff) {
		case BUFF_FIRST_SWORD_OF_AKRANE:
			break;
		default:
			buffs.erase(buffs.begin() + a);
			--a;
			break;
		}
	}

	*/

	//Dispel effects
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDispelThis)
			effects[i].OnDispelThis(this);

	//Remove misc
	hasCelerityMoved = true;
	hasCelerityAttacked = true;
	hasForcefield = false;

	//Update
	UpdateStatBuffs();

}

//Summon minion on cast resolution
void Minion::Resolve(BoardTile* tile) {

	//Trigger any effects on this card
	Card::Resolve(tile);

	//Summon this
	game->Summon(this, tile, true);

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
bool Minion::HasKeywords(eKeywordFlags keywords) {
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

#pragma region Events

//When a minion is summoned
void Minion::OnSummon(Minion* minion, bool fromActionBar) {

	//Trigger any effects on this card
	Card::OnSummon(minion, fromActionBar);

	//If on board
	if (curTile != nullptr) {

		//When this is summoned
		if (minion == this) {

			//Exhaust
			hasMoved = true;
			hasAttacked = true;
			hasCelerityMoved = true;
			hasCelerityAttacked = true;
			if (HasKeywords(KEYWORD_RUSH)) {
				hasMoved = false;
				hasAttacked = false;
				if (HasKeywords(KEYWORD_CELERITY)) {
					hasCelerityMoved = false;
					hasCelerityAttacked = false;
				}
			}
			if (HasKeywords(KEYWORD_FORCEFIELD)) { hasForcefield = true; }

			/*

			//From action bar (Opening Gambit)
			if (actionBar) {
				switch (skill.skill) {
				case SKILL_AZURE_HERALD:
					owner->general->DealDamage(this, -3);
					break;
				case SKILL_BLAZE_HOUND:
					game->players[0].Draw();
					game->players[1].Draw();
					break;
				case SKILL_BLISTERING_SKORN:
					for (int a = 0; a < game->minions.size(); ++a) { game->minions[a]->DealDamage(this, 1); }
					break;
				case SKILL_CROSSBONES:
					game->HighlightSelectable(TARGET_ENEMY_RANGED);
					if (game->selectable.size() > 0) { game->callback = EffectCallback(this, nullptr, nullptr, SKILL_CROSSBONES); }
					break;
				case SKILL_DANCING_BLADES:
					if (true) {
						int _x = curTile->pos.x;
						&game->players[0] == owner ? ++_x : --_x;
						if (_x > -1 && _x < 9) {
							if (game->map.tiles[_x][curTile->pos.y].minion != nullptr) {
								if (game->map.tiles[_x][curTile->pos.y].minion->tribe != TRIBE_GENERAL) {
									game->map.tiles[_x][curTile->pos.y].minion->DealDamage(this, 3);
								}
							}
						}
					}
					break;
				case SKILL_DEATHBLIGHTER:
					for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
						for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
							if (game->map.tiles[a][b].minion != nullptr) {
								if (game->map.tiles[a][b].minion->owner != owner) {
									if (game->map.tiles[a][b].minion->tribe != TRIBE_GENERAL) {
										game->map.tiles[a][b].minion->DealDamage(this, 3);
									}
								}
							}
						}
					}
					break;
				case SKILL_DUST_WAILER:
					if (&game->players[0] == owner && curTile->pos.x < 8) {
						for (int a = curTile->pos.x + 1; a < 9; ++a) {
							if (game->map.tiles[a][curTile->pos.y].minion != nullptr) {
								game->map.tiles[a][curTile->pos.y].minion->DealDamage(this, 3);
							}
						}
					}
					else if (&game->players[1] == owner && curTile->pos.x > 0) {
						for (int a = curTile->pos.x - 1; a > -1; --a) {
							if (game->map.tiles[a][curTile->pos.y].minion != nullptr) {
								game->map.tiles[a][curTile->pos.y].minion->DealDamage(this, 3);
							}
						}
					}
					break;
				case SKILL_EMERALD_REJUVENATOR:
					owner->general->DealDamage(this, -4);
					owner->opponent->general->DealDamage(this, -4);
					break;
				case SKILL_EPHEMERAL_SHROUD:
					game->HighlightSelectable(TARGET_NEAR_UNIT, this);
					if (game->selectable.size() > 0) { game->callback = EffectCallback(this, nullptr, nullptr, SKILL_EPHEMERAL_SHROUD); }
					break;
				case SKILL_FLAMEBLOOD_WARLOCK:
					owner->general->DealDamage(this, 3);
					owner->opponent->general->DealDamage(this, 3);
					break;
				case SKILL_FROSTBONE_NAGA:
					for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
						for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
							if (game->map.tiles[a][b].minion != nullptr) {
								if (game->map.tiles[a][b].minion != this) {
									game->map.tiles[a][b].minion->DealDamage(this, 2);
								}
							}
						}
					}
					break;
				case SKILL_GHOST_LYNX:
					game->HighlightSelectable(TARGET_MINION_NEAR_UNIT, this);
					if (game->selectable.size() > 0) { game->callback = EffectCallback(this, nullptr, nullptr, SKILL_GHOST_LYNX); }
					break;
				case SKILL_HEALING_MYSTIC:
					game->HighlightSelectable(TARGET_UNIT, this);
					if (game->selectable.size() > 0) { game->callback = EffectCallback(this, nullptr, nullptr, SKILL_HEALING_MYSTIC); }
					break;
				case SKILL_JAX_TRUESIGHT:
					for (int a = 0; a < 9; a += 8) {
						for (int b = 0; b < 5; b += 4) {
							if (game->map.tiles[a][b].minion == nullptr) {
								Minion* u2 = new Minion(*(dynamic_cast<Minion*>(token)));
								game->SetContext(u2, owner);
								game->Summon(u2, a, b, false);
							}
						}
					}
					break;
				case SKILL_KEEPER_OF_THE_VALE:
					for (int a = 0; a < game->grave.size(); ++a) {
						if (game->grave[a]->cardType == CARD_UNIT && !game->grave[a]->isToken && game->grave[a]->owner == owner) {
							game->HighlightSelectable(TARGET_TILE_NEAR_UNIT, this);
							if (game->selectable.size() > 0) { game->callback = EffectCallback(this, nullptr, nullptr, SKILL_KEEPER_OF_THE_VALE); }
							break;
						}
					}
					break;
				case SKILL_LADY_LOCKE:
					owner->general->AddEffect(EFFECT_LADY_LOCKE_A);
					break;
				case SKILL_LIGHTBENDER:
					for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
						for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
							if (game->map.tiles[a][b].minion != nullptr) {
								if (game->map.tiles[a][b].minion != this) {
									game->map.tiles[a][b].minion->Dispel();
								}
							}
						}
					}
					break;
				}
			}

			//Summoned from anywhere (apply global effects)
			switch (skill.skill) {
			case SKILL_ARCHON_SPELLBINDER:
				for (int a = 0; a < owner->opponent->hand.size(); ++a) {
					if (owner->opponent->hand[a]->cardType == CARD_SPELL) {
						dynamic_cast<Spell*>(owner->opponent->hand[a])->AddBuff(BUFF_ARCHON_SPELLBINDER);
					}
				}
				for (int a = 0; a < owner->opponent->deck.size(); ++a) {
					if (owner->opponent->deck[a]->cardType == CARD_SPELL) {
						dynamic_cast<Spell*>(owner->opponent->deck[a])->AddBuff(BUFF_ARCHON_SPELLBINDER);
					}
				}
				break;
			case SKILL_ARROW_WHISTLER:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							if (game->minions[a]->IsRanged()) {
								game->minions[a]->AddBuff(BUFF_ARROW_WHISTLER);
							}
						}
					}
				}
				break;
			case SKILL_FIRST_SWORD_OF_AKRANE:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a]->tribe != TRIBE_GENERAL) {
							if (game->minions[a] != this) {
								game->minions[a]->AddBuff(BUFF_FIRST_SWORD_OF_AKRANE);
							}
						}
					}
				}
				break;
			case SKILL_GOLDEN_JUSTICAR:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							if (game->minions[a]->IsProvoking()) {
								game->minions[a]->AddEffect(EFFECT_GOLDEN_JUSTICAR);
							}
						}
					}
				}
				break;
			case SKILL_GOLEM_METALLURGIST:
				for (int a = 0; a < owner->hand.size(); ++a) {
					if (owner->hand[a]->cardType == CARD_UNIT) {
						Minion* u = dynamic_cast<Minion*>(owner->hand[a]);
						if (u->tribe == TRIBE_GOLEM) {
							u->AddBuff(BUFF_GOLEM_METALLURGIST);
						}
					}
				}
				for (int a = 0; a < owner->deck.size(); ++a) {
					if (owner->deck[a]->cardType == CARD_UNIT) {
						Minion* u = dynamic_cast<Minion*>(owner->deck[a]);
						if (u->tribe == TRIBE_GOLEM) {
							u->AddBuff(BUFF_GOLEM_METALLURGIST);
						}
					}
				}
				break;
			case SKILL_GOLEM_VANQUISHER:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							if (game->minions[a]->tribe == TRIBE_GOLEM) {
								game->minions[a]->AddEffect(EFFECT_GOLEM_VANQUISHER);
							}
						}
					}
				}
				break;
			case SKILL_GROVE_LION:
				owner->general->AddEffect(EFFECT_GROVE_LION);
				owner->general->hasForcefield = true;
				break;
			}

			*/

		}

		/*

		//When something else is summoned
		else {

			//Skills
			switch (skill.skill) {
			case SKILL_ARAKI_HEADHUNTER:
				if (minion->owner == owner) {
					if (actionBar && minion->skill.IsOpeningGambit()) {
						AddBuff(BUFF_ARAKI_HEADHUNTER);
					}
				}
				break;
			case SKILL_ARROW_WHISTLER:
				if (minion->owner == owner) {
					if (minion->IsRanged()) {
						minion->AddBuff(BUFF_ARROW_WHISTLER);
					}
				}
				break;
			case SKILL_CRIMSON_OCULUS:
				if (minion->owner != owner) { AddBuff(BUFF_CRIMSON_OCULUS); }
				break;
			case SKILL_FIRST_SWORD_OF_AKRANE:
				if (minion->owner == owner) {
					minion->AddBuff(BUFF_FIRST_SWORD_OF_AKRANE);
				}
				break;
			case SKILL_GOLDEN_JUSTICAR:
				if (minion->owner == owner) {
					if (minion->IsProvoking()) {
						minion->AddEffect(EFFECT_GOLDEN_JUSTICAR);
					}
				}
				break;
			case SKILL_GOLEM_METALLURGIST:
				if (minion->owner == owner) {
					if (minion->tribe == TRIBE_GOLEM) {
						minion->RemoveBuff(BUFF_GOLEM_METALLURGIST, true);
						for (int b = 0; b < owner->hand.size(); ++b) {
							if (owner->hand[b]->cardType == CARD_UNIT) {
								dynamic_cast<Minion*>(owner->hand[b])->RemoveBuff(BUFF_GOLEM_METALLURGIST, true);
							}
						}
						for (int b = 0; b < owner->deck.size(); ++b) {
							if (owner->deck[b]->cardType == CARD_UNIT) {
								dynamic_cast<Minion*>(owner->deck[b])->RemoveBuff(BUFF_GOLEM_METALLURGIST, true);
							}
						}
					}
				}
				break;
			case SKILL_GOLEM_VANQUISHER:
				if (minion->owner == owner) {
					if (minion->tribe == TRIBE_GOLEM) {
						minion->AddEffect(EFFECT_GOLEM_VANQUISHER);
					}
				}
				break;
			}

			//Effects
			for (int a = 0; a < effects.size(); ++a) {
				switch (effects[a].effect) {
				case EFFECT_DARKFIRE_SACRIFICE:
					if (minion->owner == owner) {
						RemoveEffect(EFFECT_DARKFIRE_SACRIFICE, true);
						minion->RemoveBuff(BUFF_DARKFIRE_SACRIFICE, true);
						for (int b = 0; b < owner->hand.size(); ++b) {
							if (owner->hand[b]->cardType == CARD_UNIT) {
								dynamic_cast<Minion*>(owner->hand[b])->RemoveBuff(BUFF_DARKFIRE_SACRIFICE, true);
							}
						}
						for (int b = 0; b < owner->deck.size(); ++b) {
							if (owner->deck[b]->cardType == CARD_UNIT) {
								dynamic_cast<Minion*>(owner->deck[b])->RemoveBuff(BUFF_DARKFIRE_SACRIFICE, true);
							}
						}
					}
					break;
				case EFFECT_LADY_LOCKE_A:
					if (minion->owner == owner) {
						minion->AddEffect(EFFECT_LADY_LOCKE_B);
						for (int b = 0; b < effects[a].stacks; ++b) { minion->AddBuff(BUFF_LADY_LOCKE); }
					}
					break;
				}

			}

		}

		*/

	}

}

//When a minion dies
void Minion::OnDeath(Minion* minion) {

	//Trigger any effects on this card
	Card::OnDeath(minion);

	/*

	//If on board
	if (curTile != nullptr) {

		//When this minion dies (Dying Wish)
		if (minion == this) {
			switch (skill.skill) {
			case SKILL_ARCHON_SPELLBINDER:
				for (int a = 0; a < owner->opponent->hand.size(); ++a) {
					if (owner->opponent->hand[a]->cardType == CARD_SPELL) {
						dynamic_cast<Spell*>(owner->opponent->hand[a])->RemoveBuff(BUFF_ARCHON_SPELLBINDER, false);
					}
				}
				for (int a = 0; a < owner->opponent->deck.size(); ++a) {
					if (owner->opponent->deck[a]->cardType == CARD_SPELL) {
						dynamic_cast<Spell*>(owner->opponent->deck[a])->RemoveBuff(BUFF_ARCHON_SPELLBINDER, false);
					}
				}
				break;
			case SKILL_ARROW_WHISTLER:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							if (game->minions[a]->IsRanged()) {
								game->minions[a]->RemoveBuff(BUFF_ARROW_WHISTLER, false);
							}
						}
					}
				}
				break;
			case SKILL_AZURE_HORN_SHAMAN:
				for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
					for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
						if (game->map.tiles[a][b].minion != nullptr && game->map.tiles[a][b].minion->owner == owner) {
							if (game->map.tiles[a][b].minion != owner->general) {
								game->map.tiles[a][b].minion->AddBuff(BUFF_AZURE_HORN_SHAMAN);
							}
						}
					}
				}
				break;
			case SKILL_DIOLTAS:
				if (true) {
					BoardTile* t = game->map.GetRandomNear(owner->general->curTile->pos.x, owner->general->curTile->pos.y);
					if (t != nullptr) {
						Minion* u2 = new Minion(*(dynamic_cast<Minion*>(token)));
						game->SetContext(u2, owner);
						game->Summon(u2, t->pos.x, t->pos.y, false);
					}
				}
				break;
			case SKILL_FIRST_SWORD_OF_AKRANE:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a]->tribe != TRIBE_GENERAL) {
							if (game->minions[a] != this) {
								game->minions[a]->RemoveBuff(BUFF_FIRST_SWORD_OF_AKRANE, false);
							}
						}
					}
				}
				break;
			case SKILL_GOLDEN_JUSTICAR:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							if (game->minions[a]->IsProvoking()) {
								game->minions[a]->RemoveEffect(EFFECT_GOLDEN_JUSTICAR, false);
							}
						}
					}
				}
				break;
			case SKILL_GOLEM_METALLURGIST:
				for (int a = 0; a < owner->hand.size(); ++a) {
					if (owner->hand[a]->cardType == CARD_UNIT) {
						dynamic_cast<Minion*>(owner->hand[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, false);
					}
				}
				for (int a = 0; a < owner->deck.size(); ++a) {
					if (owner->deck[a]->cardType == CARD_UNIT) {
						dynamic_cast<Minion*>(owner->deck[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, false);
					}
				}
				break;
			case SKILL_GOLEM_VANQUISHER:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner) {
						if (game->minions[a] != this) {
							game->minions[a]->RemoveEffect(EFFECT_GOLEM_VANQUISHER, false);
						}
					}
				}
				break;
			case SKILL_GROVE_LION:
				for (int a = 0; a < owner->general->effects.size(); ++a) {
					if (owner->general->effects[a].effect == EFFECT_GROVE_LION) {
						if (owner->general->effects[a].stacks == 1) {
							owner->general->hasForcefield = false;
						}
						owner->general->RemoveEffect(EFFECT_GROVE_LION, false);
						break;
					}
				}
				break;
			case SKILL_IRONCLAD:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner != owner) {
						game->minions[a]->Dispel();
					}
				}
				break;
			case SKILL_JAXI:
				if (true) {
					BoardTile* t = game->map.GetRandomCorner();
					if (t != nullptr) {
						Minion* u2 = new Minion(*(dynamic_cast<Minion*>(token)));
						game->SetContext(u2, owner);
						game->Summon(u2, t->pos.x, t->pos.y, false);
					}
				}
				break;
			}
		}

	}

	*/

}

//When a minion attacks
void Minion::OnAttack(Minion* source, Minion* target, bool counter) {

	/*

	//If on board
	if (curTile != nullptr) {

		//If ally attacked
		if (source->owner == owner) {

			//Skills
			switch (skill.skill) {
			case SKILL_DAY_WATCHER:
				if (!counter) {
					if (source->tribe != TRIBE_GENERAL) {
						owner->general->DealDamage(this, -1);
					}
				}
				break;
			}

		}

		//If any attacked
		switch (skill.skill) {
		case SKILL_EXUN:
			if ((source == this || target == this) && !counter) { owner->Draw(); }
			break;
		}

	}

	*/

}

//When a minion is damaged
void Minion::OnDamage(Minion* source, Minion* target, int damage) {

	/*

	//If on board
	if (curTile != nullptr) {

		//If this dealt damage
		if (source == this) {
			switch (skill.skill) {
			case SKILL_CAPTAIN_HANK_HART:
				if (hp > 0) { DealDamage(this, -damage); }
				break;
			case SKILL_ENVYBAER:
				if (true) {
					BoardTile* t = game->map.GetRandomCorner();
					if (t != nullptr) {
						target->SetPosition(t->pos.x, t->pos.y);
						game->eventManager.SendOnMove(target, true);
					}
				}
				break;
			}
		}

		//If this was damaged
		if (target == this) {
			switch (skill.skill) {
			case SKILL_CHAOS_ELEMENTAL:
				if (true) {
					BoardTile* t = game->map.GetRandom();
					if (t != nullptr) {
						SetPosition(t->pos.x, t->pos.y);
						game->eventManager.SendOnMove(this, true);
					}
				}
				break;
			case SKILL_ECLIPSE:
				owner->opponent->general->DealDamage(this, damage);
				break;
			case SKILL_KHYMERA:
				BoardTile* t = game->map.GetRandomNear(curTile->pos.x, curTile->pos.y);
				if (t != nullptr) {
					int i = rand() % game->cardList.minionTokenList.size();
					Minion* u2 = new Minion(*(dynamic_cast<Minion*>(&game->cardList.minionTokenList[i])));
					game->SetContext(u2, owner);
					game->Summon(u2, t->pos.x, t->pos.y, false);
				}
				break;
			}
		}

	}

	//If in hand/deck
	else {
		switch (skill.skill) {
		case SKILL_BLOOD_TAURA:
			if (target == owner->general) {
				RemoveBuff(BUFF_BLOOD_TAURA, true);
				for (int a = 0; a < 25 - owner->general->hp; ++a) { AddBuff(BUFF_BLOOD_TAURA); }
			}
			break;
		case SKILL_CHAKKRAM:
			if (target == owner->general) {
				if (&game->players[game->turn] != owner) {
					AddBuff(BUFF_CHAKKRAM);
				}
			}
			break;
		}
	}

	*/

}

//When a minion is healed
void Minion::OnHeal(Minion* source, Minion* target, int heal) {

	/*

	//If in hand/deck
	if (curTile == nullptr) {
		switch (skill.skill) {
		case SKILL_BLOOD_TAURA:
			if (target == owner->general) {
				RemoveBuff(BUFF_BLOOD_TAURA, true);
				for (int a = 0; a < 25 - owner->general->hp; ++a) { AddBuff(BUFF_BLOOD_TAURA); }
			}
			break;
		}
	}

	*/

}

//When a minion moves or is moved
void Minion::OnMove(Minion* minion, bool byEffect) {

	/*

	//If on board
	if (curTile != nullptr) {

		//If this moved
		if (minion == this) {
			switch (skill.skill) {
			case SKILL_BLACK_LOCUST:
				if (!byEffect) {
					BoardTile* t = game->map.GetRandomNear(curTile->pos.x, curTile->pos.y);
					if (t != nullptr) {
						Minion* u2 = new Minion(*(dynamic_cast<Minion*>(original)));
						game->SetContext(u2, owner);
						game->Summon(u2, t->pos.x, t->pos.y, false);
					}
				}
				break;
			}
		}

	}

	*/

}

//When a spell is cast
void Minion::OnSpellCast(Spell* spell) {

	/*

	//If on board
	if (curTile != nullptr) {
		
		//If ally spell cast
		if (spell->owner == owner) {
			switch (skill.skill) {
			case SKILL_FIRESTARTER:
				if (true) {
					BoardTile* t = game->map.GetRandomNear(curTile->pos.x, curTile->pos.y);
					if (t != nullptr) {
						Minion* u = new Minion(*(dynamic_cast<Minion*>(token)));
						game->SetContext(u, owner);
						game->Summon(u, t->pos.x, t->pos.y, false);
					}
				}
				break;
			}
		}

	}

	*/

}

//When a card is drawn
void Minion::OnDraw(Card* card, bool fromDeck) {

	/*

	//If on board
	if (curTile != nullptr) {

		//Skills
		switch (skill.skill) {
		case SKILL_ARCHON_SPELLBINDER:
			if (!fromDeck) {
				if (card->owner != owner && card->cardType == CARD_SPELL) {
					dynamic_cast<Spell*>(card)->AddBuff(BUFF_ARCHON_SPELLBINDER);
				}
			}
			break;
		case SKILL_DECIMUS:
			if (fromDeck) {
				if (card->owner != owner) {
					owner->opponent->general->DealDamage(this, 2);
				}
			}
			break;
		case SKILL_GOLEM_METALLURGIST:
			if (!fromDeck) {
				if (card->owner == owner && card->cardType == CARD_UNIT) {
					Minion* u = dynamic_cast<Minion*>(card);
					if (u->tribe == TRIBE_GOLEM) {
						dynamic_cast<Minion*>(card)->AddBuff(BUFF_DARKFIRE_SACRIFICE);
					}
				}
			}
			break;
		}

		//Effects
		for (int a = 0; a < effects.size(); ++a) {
			switch (effects[a].effect) {
			case EFFECT_DARKFIRE_SACRIFICE:
				if (!fromDeck) {
					if (card->owner == owner && card->cardType == CARD_UNIT) {
						for (int b = 0; b < effects[a].stacks; ++b) {
							dynamic_cast<Minion*>(card)->AddBuff(BUFF_DARKFIRE_SACRIFICE);
						}
					}
				}
				break;
			}
		}

	}

	*/

}

//When this card is replaced
void Minion::OnReplace(Card* replaced) {

	/*

	//If this is replaced
	if (replaced->cardType == CARD_UNIT) {
		if (dynamic_cast<Minion*>(replaced) == this) {
			switch (skill.skill) {
			case SKILL_ASTRAL_CRUSADER:
				AddBuff(BUFF_ASTRAL_CRUSADER);
				break;
			case SKILL_DREAMGAZER:
				if (true) {
					BoardTile* t = game->map.GetRandomNear(owner->general->curTile->pos.x, owner->general->curTile->pos.y);
					if (t != nullptr) {
						Minion* u = new Minion(*(dynamic_cast<Minion*>(original)));
						game->SetContext(u, owner);
						game->Summon(u, t->pos.x, t->pos.y, false);
						owner->general->DealDamage(this, 2);
					}
				}
				break;
			}
		}
	}

	*/

}

//When a player's turn ends
void Minion::OnTurnEnd(Player* player) {

	//Trigger any effects on this card
	Card::OnTurnEnd(player);

	//Refresh
	hasMoved = false;
	hasAttacked = false;
	if (HasKeywords(KEYWORD_CELERITY)) {
		hasCelerityMoved = false;
		hasCelerityAttacked = false;
	}

	//Reset forcefield
	if (HasKeywords(KEYWORD_FORCEFIELD)) { hasForcefield = true; }

	/*

	//If on board
	if (curTile != nullptr) {

		//When this minion's player ends turn
		if (player == owner) {

			//Skills
			switch (skill.skill) {
			case SKILL_BASTION:
				for (int a = 0; a < game->minions.size(); ++a) {
					if (game->minions[a]->owner == owner && game->minions[a] != this) {
						if (game->minions[a] != owner->general) {
							game->minions[a]->AddBuff(BUFF_BASTION);
						}
					}
				}
				break;
			case SKILL_BONEREAPER:
				for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
					for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
						if (game->map.tiles[a][b].minion != nullptr) {
							if (game->map.tiles[a][b].minion->owner != owner) {
								if (game->map.tiles[a][b].minion->tribe != TRIBE_GENERAL) {
									game->map.tiles[a][b].minion->DealDamage(this, 2);
								}
							}
						}
					}
				}
				break;
			case SKILL_GOLEM_METALLURGIST:
				for (int a = 0; a < owner->hand.size(); ++a) {
					if (owner->hand[a]->cardType == CARD_UNIT) {
						dynamic_cast<Minion*>(owner->hand[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, true);
					}
				}
				for (int a = 0; a < owner->deck.size(); ++a) {
					if (owner->deck[a]->cardType == CARD_UNIT) {
						dynamic_cast<Minion*>(owner->deck[a])->RemoveBuff(BUFF_GOLEM_METALLURGIST, true);
					}
				}
				break;
			case SKILL_LUX_IGNIS:
				for (int a = max(curTile->pos.x - 1, 0); a < min(curTile->pos.x + 2, 9); ++a) {
					for (int b = max(curTile->pos.y - 1, 0); b < min(curTile->pos.y + 2, 5); ++b) {
						if (game->map.tiles[a][b].minion != nullptr) {
							if (game->map.tiles[a][b].minion->owner == owner) {
								if (game->map.tiles[a][b].minion != this) {
									if (game->map.tiles[a][b].minion != owner->general) {
										game->map.tiles[a][b].minion->DealDamage(this, -2);
									}
								}
							}
						}
					}
				}
				break;
			}

			//Effects
			for (int a = 0; a < effects.size(); ++a) {
				switch (effects[a].effect) {
				case EFFECT_LADY_LOCKE_A:
					RemoveEffect(EFFECT_LADY_LOCKE_A, true);
					break;
				}
			}

		}

	}

	//Anywhere
	switch (skill.skill) {
	case SKILL_CHAKKRAM:
		if (player == owner) { RemoveBuff(BUFF_CHAKKRAM, true); }
		break;
	}

	*/

}

//When a player's turn starts
void Minion::OnTurnStart(Player* player) {

	//Trigger any effects on this card
	Card::OnTurnStart(player);

	/*

	//If on board
	if (curTile != nullptr) {

		//If this minion's player started turn
		if (player == owner) {

			//Skills
			switch (skill.skill) {
			case SKILL_DARK_NEMESIS:
				owner->opponent->general->DealDamage(this, 4);
				AddBuff(BUFF_DARK_NEMESIS);
				break;
			case SKILL_GOLEM_METALLURGIST:
				for (int a = 0; a < owner->hand.size(); ++a) {
					if (owner->hand[a]->cardType == CARD_UNIT) {
						Minion* u = dynamic_cast<Minion*>(owner->hand[a]);
						if (u->tribe == TRIBE_GOLEM) {
							u->AddBuff(BUFF_GOLEM_METALLURGIST);
						}
					}
				}
				for (int a = 0; a < owner->deck.size(); ++a) {
					if (owner->deck[a]->cardType == CARD_UNIT) {
						Minion* u = dynamic_cast<Minion*>(owner->deck[a]);
						if (u->tribe == TRIBE_GOLEM) {
							u->AddBuff(BUFF_GOLEM_METALLURGIST);
						}
					}
				}
				break;
			}

		}

	}

	*/

}

//Effect callback
void Minion::Callback(BoardTile* tile) {

	/*

	switch (game->callback.skill) {
	case SKILL_CROSSBONES:
		if (tile->minion != nullptr) { tile->minion->isDead = true; }
		break;
	case SKILL_EPHEMERAL_SHROUD:
		tile->SetFeature(TILE_NONE);
		if (tile->minion != nullptr) { tile->minion->Dispel(); }
		break;
	case SKILL_GHOST_LYNX:
		if (true) {
			BoardTile* t2 = game->map.GetRandom();
			if (t2 != nullptr) { tile->minion->SetPosition(t2->pos.x, t2->pos.y); }
			game->eventManager.SendOnMove(t2->minion, true);
		}
		break;
	case SKILL_HEALING_MYSTIC:
		if (tile->minion != nullptr) { tile->minion->DealDamage(this, -2); }
		break;
	case SKILL_KEEPER_OF_THE_VALE:
		if (tile->minion == nullptr) {
			std::vector<Card*> cv;
			for (int a = 0; a < game->grave.size(); ++a) {
				if (game->grave[a]->cardType == CARD_UNIT && !game->grave[a]->isToken && game->grave[a]->owner == owner) {
					cv.push_back(game->grave[a]);
				}
			}
			if (cv.size() > 0) {
				int i = rand() % cv.size();
				Minion* u2 = new Minion(*(dynamic_cast<Minion*>(cv[i]->original)));
				game->SetContext(u2, owner);
				game->Summon(u2, tile->pos.x, tile->pos.y, false);
			}
		}
		break;
	}
	game->callback = EffectCallback();

	*/

}

#pragma endregion