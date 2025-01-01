//Include
#include "game.h"

#pragma region Constructors / Initialization

//Game constructors
Spell::Spell() : Spell(FACTION_NEUTRAL, TargetMode(TARGET_MODE_ALL, TARGET_FILTER_NONE), 0, "", "???") {}
Spell::Spell(eFaction _faction, TargetMode _targetMode, int _cost, std::string path, std::string _name) {
	cardType = CARD_SPELL;
	faction = _faction;
	targetMode = _targetMode;
	cost = _cost;
	name = _name;
	if (path == "")
		sprite.Resize(5, 5);
	else
		sprite.CreateFromFile("resources/spells/" + path + ".txt");
	GenerateDetails();
	game = nullptr;
	owner = nullptr;
}
Spell::Spell(eFaction _faction, TargetMode _targetMode, int _cost, std::string path, std::string _name, Effect effect) : Spell(_faction, _targetMode, _cost, path, _name) {
	if (effect.effect != EFFECT_NONE)
		AddEffect(effect, nullptr);
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
	if (token != nullptr) {
		if (y < 7) { y = 7; }
		renderer.Render(divider, 66, y);
		y += 2;
		token->DrawDetails(renderer, y);
	}
}

#pragma endregion

#pragma region Updates

//Update cost when buffs change
void Spell::UpdateStatBuffs() {
	int costBuff = 0;
	for (int i = 0; i < effects.size(); ++i)
		costBuff += effects[i].costBuff;
	cost = max(original->cost + costBuff, 0);
}

//Update card stats
void Spell::UpdateDetailStats() {
	std::string s = "COST:" + std::to_string(cost);
	header[1].CreateFromString(s);
	for (int i = 0; i < header[1].buffer.size(); ++i)
		if (s[i] != ':')
			header[1].buffer[i].Attributes = COLOR_LTBLUE;
}

#pragma endregion

#pragma region Action & Event Overrides

//When a card is cast
void Spell::Resolve(BoardTile* tile) {

	//Trigger any effects on this card
	Card::Resolve(tile);

	//Cast this
	game->grave.push_back(this);
	game->spellHistory.push_back(this);

}

#pragma endregion