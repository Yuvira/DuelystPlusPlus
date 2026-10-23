#pragma once

//Include
#include "card.h"
#include "cardids.h"
#include "effectids.h"
#include "utils.h"

//Card type enum
enum eCardType {
	CARDTYPE_UNIT,
	CARDTYPE_SPELL
};

//Card Definition
struct CardDefinition {

	//Constructors
	CardDefinition(eCardType type, eCard cardId, eRarity rarity, eFaction faction, std::string path, std::string name, int cost, eEffect effect) {
		this->type = type;
		this->cardId = cardId;
		this->rarity = rarity;
		this->faction = faction;
		tribe = TRIBE_NONE;
		this->name = name;
		this->cost = cost;
		atk = 0;
		hp = 0;
		this->effect = effect;
		if (path == "") sprite.Resize(5, 5);
		else if (type == CARDTYPE_UNIT) sprite.CreateFromFile("resources/minions/" + path + ".txt");
		else if (type == CARDTYPE_SPELL) sprite.CreateFromFile("resources/spells/" + path + ".txt");
		isToken = false;
	}
	CardDefinition(eCard cardId, eRarity rarity, eFaction faction, eTribe tribe, std::string path, std::string name, int cost, int atk, int hp, eEffect effect)
		: CardDefinition(CARDTYPE_UNIT, cardId, rarity, faction, path, name, cost, effect) {
		this->tribe = tribe;
		this->atk = atk;
		this->hp = hp;
		detailNameSprite.CreateFromString(Utils::ToUpper(name) + " - " + TribeToString(tribe));
		detailNameSprite.SetColor(COLOR_GRAY, name.size(), detailNameSprite.GetSize());
		if (tribe == TRIBE_GENERAL)
			Utils::GenerateStatSprite(detailStatSprite, atk, hp);
		else
			Utils::GenerateStatSprite(detailStatSprite, cost, atk, hp);
	}
	CardDefinition(eCard cardId, eRarity rarity, eFaction faction, std::string path, std::string name, int cost, eEffect effect)
		: CardDefinition(CARDTYPE_SPELL, cardId, rarity, faction, path, name, cost, effect) {
		detailNameSprite.CreateFromString(Utils::ToUpper(name) + " - SPELL");
		detailNameSprite.SetColor(COLOR_GRAY, name.size(), detailNameSprite.GetSize());
		Utils::GenerateStatSprite(detailStatSprite, cost);
	}

	//Properties
	eCardType type;
	eCard cardId;
	eRarity rarity;
	eFaction faction;
	eTribe tribe;
	std::string name;
	int cost;
	int atk;
	int hp;
	eEffect effect;
	bool isToken;
	Sprite sprite;
	Sprite detailNameSprite;
	Sprite detailStatSprite;

};