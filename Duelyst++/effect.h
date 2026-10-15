#pragma once

//Include
#include <string>
#include "effectids.h"

//Definitions
class Game;
class BoardTile;
class Player;
class Card;
class Effect;
struct Event;

#pragma region Effect Context

//Self reference container for passing to lambdas
struct EffectContext {
	EffectContext(Effect* effect, Card* card, Game* game) {
		this->effect = effect;
		this->card = card;
		this->game = game;
	}
	bool IsOnBoard();
	bool BothOnBoard(Card* compare);
	bool IsCard(Card* compare);
	bool IsCardOnBoard(Card* compare);
	bool SharesOwner(Card* compare);
	bool IsOwnedBy(Player* compare);
	bool IsAllied(Card* compare);
	bool IsOwnerTurn();
	Effect* effect;
	Card* card;
	Game* game;
};

#pragma endregion

//Effect class
class Effect {

//Methods
public:

	//Constructors
	Effect();
	Effect(eEffect effectId, int keywords, std::string description);
	~Effect();

	//Triggers
	void TriggerPreEvent(EffectContext context, Event* event);
	void TriggerPostEvent(EffectContext context, Event* event);

	//Legacy triggers
	void TriggerOnAddThis(EffectContext context);
	void TriggerOnRemoveThis(EffectContext context);
	void TriggerOnPreCastThis(EffectContext context, BoardTile* tile);
	void TriggerOnResolveThis(EffectContext context, BoardTile* tile);

	//Getters & Setters
	Card* GetToken();
	Card* GetRef();
	const Effect* const& GetSource() const;
	const eEffect& GetID() const;
	const int& GetKeywords() const;
	const std::string& GetDescription() const;
	const int& GetCostBuff() const;
	const int& GetFixedCost() const;
	const int& GetAtkBuff() const;
	const int& GetHPBuff() const;
	const int& GetMoveBuff() const;
	void SetCostBuff(int costBuff);
	void SetFixedCost(int costBuff);
	void SetAtkBuff(int costBuff);
	void SetHPBuff(int costBuff);
	void SetMoveBuff(int costBuff);
	void SetToken(Card* token);
	void SetSources(Effect* source, Card* refCard);

	//Utils
	bool IsContinuous() { return source != nullptr; }

	//Function pointers
	void (*OnPreEvent)(EffectContext context, Event* event) = nullptr;
	void (*OnPostEvent)(EffectContext context, Event* event) = nullptr;
	void (*OnAddThis)(EffectContext) = nullptr;
	void (*OnRemoveThis)(EffectContext) = nullptr;
	void (*OnPreCastThis)(EffectContext, BoardTile* tile) = nullptr;
	void (*OnResolveThis)(EffectContext, BoardTile* tile) = nullptr;
	void (*ApplyEffect)(EffectContext) = nullptr;
	void (*RemoveEffect)(EffectContext) = nullptr;

//Properties
private:
	eEffect effectId;
	int keywords;
	std::string description;
	bool triggered;
	int fixedCost;
	int costBuff;
	int atkBuff;
	int hpBuff;
	int moveBuff;
	Card* token;
	Card* refCard;
	Effect* source;

};