#pragma once

//Include
#include "card.h"
#include "cardids.h"
#include "effectids.h"
#include "utils.h"

//Card type enum
enum eCardType {
	CARDTYPE_UNIT,
	CARDTYPE_SPELL,
	CARDTYPE_TILE
};

//Card Definition
struct CardDefinition {

	//Constructors
	CardDefinition(eCardType type, eCard cardId, eRarity rarity, eFaction faction, std::string path, std::string name, eEffect effect) {
		this->type = type;
		this->cardId = cardId;
		this->rarity = rarity;
		this->faction = faction;
		this->name = name;
		this->effect = effect;
		tribe = TRIBE_NONE;
		cost = cost;
		atk = 0;
		hp = 0;
		isToken = false;
		sprite.Resize(5, 5);
	}
	CardDefinition(eCard cardId, eRarity rarity, eFaction faction, eTribe tribe, std::string path, std::string name, int cost, int atk, int hp, eEffect effect)
		: CardDefinition(CARDTYPE_UNIT, cardId, rarity, faction, path, name, effect) {
		this->cost = cost;
		this->tribe = tribe;
		this->atk = atk;
		this->hp = hp;
		sprite.CreateFromFile("resources/minions/" + path + ".txt");
		GenerateDetailName(" - " + TribeToString(tribe));
		if (tribe == TRIBE_GENERAL)
			Utils::GenerateStatSprite(detailStatSprite, atk, hp);
		else
			Utils::GenerateStatSprite(detailStatSprite, cost, atk, hp);
	}
	CardDefinition(eCard cardId, eRarity rarity, eFaction faction, std::string path, std::string name, int cost, eEffect effect)
		: CardDefinition(CARDTYPE_SPELL, cardId, rarity, faction, path, name, effect) {
		this->cost = cost;
		sprite.CreateFromFile("resources/spells/" + path + ".txt");
		GenerateDetailName(" - SPELL");
		Utils::GenerateStatSprite(detailStatSprite, cost);
	}
	CardDefinition(eCard cardId, eRarity rarity, eFaction faction, std::string path, std::string name, eEffect effect)
		: CardDefinition(CARDTYPE_TILE, cardId, rarity, faction, path, name, effect) {
		sprite.CreateFromFile("resources/tiles/" + path + ".txt");
		GenerateDetailName(" - TILE");
	}
	
	//Shortcut for generating detail name
	void GenerateDetailName(const std::string& typeString) {
		detailNameSprite.CreateFromString(Utils::ToUpper(name) + typeString);
		detailNameSprite.SetColor(COLOR_GRAY, name.size(), detailNameSprite.GetSize());
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