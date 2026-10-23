#pragma once

//Definitions
class Aura;
class BoardTile;
class Card;
class Effect;
class Game;
class Map;
class Player;
class Spell;
class SpellEffect;
class Unit;
struct AuraContext;
struct SpellContext;

//Context object for passing state information to event triggers
struct EffectContext {

	//Constructor
	EffectContext() : EffectContext(nullptr, nullptr, nullptr) {}
	EffectContext(Effect* effect, Card* card, Game* game) {
		this->effect = effect;
		this->card = card;
		this->game = game;
	}

	//Conversions
	bool IsAuraContext();
	bool IsSpellContext();
	AuraContext GetAuraContext();
	SpellContext GetSpellContext();

	//Helpers
	bool IsOnBoard();
	bool BothOnBoard(Card* compare);
	bool IsCard(Card* compare);
	bool IsCardOnBoard(Card* compare);
	bool IsInOwnerHand();
	bool SharesOwner(Card* compare);
	bool IsOwnedBy(Player* compare);
	bool IsAllied(Card* compare);
	bool IsOwnerTurn();
	Player* GetOwner();
	Player* GetOpponent();
	Unit* GetUnit();
	Spell* GetSpell();
	Unit* GetAllyGeneral();
	Unit* GetEnemyGeneral();
	BoardTile* GetTile();
	Map* GetMap();

	//Properties
	Effect* effect;
	Card* card;
	Game* game;

};

//
struct AuraContext {

	//Constructor
	AuraContext() : AuraContext(nullptr, nullptr, nullptr) {}
	AuraContext(Aura* aura, Card* card, Game* game) {
		this->aura = aura;
		this->card = card;
		this->game = game;
	}

	//Helpers
	bool CardIsAuraSource();
	bool CardSharesAuraOwner();
	bool CardIsFriendOfAuraSource();

	//Properties
	Aura* aura;
	Card* card;
	Game* game;

};

struct SpellContext {

	//Constructor
	SpellContext() : SpellContext(nullptr, nullptr, nullptr) {}
	SpellContext(SpellEffect* effect, Spell* spell, Game* game) {
		this->effect = effect;
		this->spell = spell;
		this->game = game;
	}

	//Properties
	SpellEffect* effect;
	Spell* spell;
	Game* game;

};