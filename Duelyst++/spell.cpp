//Include
#include <algorithm>
#include "card.h"
#include "effect.h"
#include "renderer.h"

#pragma region Constructors / Initialization

//Game constructors
Spell::Spell()
	: Spell(CARD_NONE, FACTION_NEUTRAL, 0, "", "???") {}
Spell::Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name)
	: Spell(cardId, faction, cost, path, name, false, nullptr) {}
Spell::Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, bool isToken)
	: Spell(cardId, faction, cost, path, name, isToken, nullptr) {}
Spell::Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, Effect* effect)
	: Spell(cardId, faction, cost, path, name, false, effect) {}
Spell::Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, bool isToken, Effect* effect) {
	this->cardId = cardId;
	this->faction = faction;
	this->cost = cost;
	this->isToken = isToken;
	this->name = name;
	if (path == "")
		sprite.Resize(5, 5);
	else
		sprite.CreateFromFile("resources/spells/" + path + ".txt");
	GenerateDetails();
	game = nullptr;
	owner = nullptr;
	if (effect != nullptr && effect->GetID() != EFFECT_NONE)
		AddEffect(*effect, nullptr, this);
}
Spell::~Spell() {}

//Generate sidebar details
void Spell::GenerateDetails() {
	std::string s;
	s.resize(name.size());
	std::transform(name.begin(), name.end(), s.begin(), ::toupper);
	s += " - SPELL";
	header[0].CreateFromString(s);
	for (int i = name.size(); i < header[0].buffer.size(); ++i)
		header[0].buffer[i].Attributes = COLOR_GRAY;
	UpdateDetailStats();
}

#pragma endregion

#pragma region Rendering

//Draw card details
void Spell::DrawDetails(Renderer& renderer, int& y) {
	sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite, 66, y);
	renderer.Render(header[0], 72, y); ++y;
	UpdateDetailStats();
	renderer.Render(header[1], 72, y); y += 2;
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

#pragma endregion

#pragma region Updates

//Update cost when buffs change
void Spell::UpdateStatBuffs() { cost = GetStat(STAT_COST, baseCost); }

//Update card stats
void Spell::UpdateDetailStats() {
	std::string s = "COST:" + std::to_string(cost);
	header[1].CreateFromString(s);
	for (int i = 0; i < header[1].buffer.size(); ++i)
		if (s[i] != ':')
			header[1].buffer[i].Attributes = COLOR_LTBLUE;
}

#pragma endregion

#pragma region Setters

//Target mode
void Spell::SetTargetMode(TargetMode targetMode) { this->targetMode = targetMode; }

#pragma endregion