//Include
#include "player.h"
#include "game.h"
#include "collections.h";
#include "card.h"
#include "effect.h"
#include "event.h"
#include "renderer.h"

#pragma region Constructor

//Game constructor
Player::Player() {
	mana = 0;
	manaMax = 0;
	replaces = 0;
	maxReplaces = 1;
	game = nullptr;
	general = nullptr;
	opponent = nullptr;
}
Player::~Player() {}

#pragma endregion

#pragma region Initialization

//Preset deck
void Player::Preset(Collections* collections, Game* game) {
	this->game = game;
	deck.push_back(new Minion(*(collections->FindCard(CARD_ARGEON_HIGHMAYNE)->GetMinion())));
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections->FindCard(CARD_EPHEMERAL_SHROUD)->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections->FindCard(CARD_BLISTERING_SKORN)->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections->FindCard(CARD_CAPTAIN_HANK_HART)->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections->FindCard(CARD_CHAOS_ELEMENTAL)->GetMinion()))); }
	//for (int i = 0; i < 7; ++i) { deck.push_back(new Spell(*(collections->FindCard(CARD_DARK_SEED)->GetSpell()))); }
	for (int i = 0; i < deck.size(); ++i)
		this->game->SetContext(deck[i], this);
}

//Initialize deck/hand
void Player::Init(int mana) {
	this->mana = mana;
	manaMax = mana;
	replaces = maxReplaces;
	for (int i = 0; i < 9; ++i) {
		uiCrystal[i].buffer[0].Char.AsciiChar = '';
		uiCrystal[i].SetColor(COLOR_GRAY);
	}
	Shuffle();
	for (int i = 0; i < 5; ++i)
		hand.push_back(deck[i]);
	deck.erase(deck.begin(), deck.begin() + 5);
}

#pragma endregion

#pragma region Rendering

//Render UI
void Player::Render(Renderer& renderer, bool left) {

	//Mana crystals
	for (int i = 0; i < 9; ++i)
		renderer.Render(uiCrystal[i], left ? (i * 2) + 2 : 62 - (i * 2), 2);

	//Cards in hand/deck
	std::string str = std::to_string(hand.size()) + "/6 " + std::to_string(deck.size()) + "/40";
	if (!left)
		str = std::to_string(deck.size()) + "/40 " + std::to_string(hand.size()) + "/6";
	Sprite spr;
	spr.CreateFromString(str);
	renderer.Render(spr, left ? 20 : 45 - str.length(), 2);

	//Replace
	if (game->GetTurnPlayer() == this) {
		uiReplace.buffer[0].Char.AsciiChar = std::to_string(replaces)[0];
		renderer.Render(uiReplace, 4, 44);
	}

}

#pragma endregion

#pragma region Updates

//Update mana sprites
void Player::UpdateMana(eColor color) {
	for (int i = 0; i < mana; ++i)
		uiCrystal[i].SetColor(color);
	for (int i = mana; i < manaMax; ++i)
		uiCrystal[i].SetColor(COLOR_LTWHITE);
	for (int i = max(mana, manaMax); i < 9; ++i)
		uiCrystal[i].SetColor(COLOR_GRAY);
}

#pragma endregion

#pragma region Actions

//Shuffle deck
void Player::Shuffle() {
	std::vector<Card*> shuffle;
	for (int i = 0; i < deck.size(); ) {
		int idx = rand() % deck.size();
		shuffle.push_back(deck[idx]);
		deck.erase(deck.begin() + idx);
	}
	for (int i = 0; i < shuffle.size(); ++i)
		deck.push_back(shuffle[i]);
	shuffle.clear();
}

//Draw card
void Player::Draw() {
	if (deck.size() > 0) {
		Card* card = deck[0];
		OnDrawEvent* event = new OnDrawEvent(game->GetTurnCount(), card);
		game->eventManager.SendOnPreEvent(game, event);
		deck.erase(deck.begin());
		if (hand.size() < 6)
			hand.push_back(card);
		else
			game->Overflow(card);
		game->eventManager.SendOnPostEvent(game, event);
		delete event;
	}
}

//Add card to hand
void Player::AddNewToHand(Card* original) {
	Card* newCard = nullptr;
	if (original->IsMinion())
		newCard = new Minion(*(original->GetMinion()));
	else if (original->IsSpell())
		newCard = new Spell(*(original->GetSpell()));
	game->SetContext(newCard, this);
	OnAddToHandEvent* event = new OnAddToHandEvent(game->GetTurnCount(), newCard);
	game->eventManager.SendOnPreEvent(game, event);
	if (hand.size() < 6)
		hand.push_back(newCard);
	else
		game->Overflow(newCard);
	game->eventManager.SendOnPostEvent(game, event);
	delete event;
}

//Replace card
void Player::Replace(int idx) {
	if (replaces > 0 && hand.size() > idx) {

		//Remove card from hand
		Card* replaced = hand[idx];
		hand.erase(hand.begin() + idx);

		//Send replace event
		OnReplaceEvent* event = new OnReplaceEvent(game->GetTurnCount(), replaced, true);
		game->eventManager.SendOnPreEvent(game, event);
		for (Effect* e : replaced->GetEffects())
			e->TriggerPreEvent(EffectContext(e, replaced, game), event);
		game->eventManager.SendOnPostEvent(game, event);
		for (Effect* e : replaced->GetEffects())
			e->TriggerPostEvent(EffectContext(e, replaced, game), event);

		//Draw new card and put replaced card back in the deck if able
		if (deck.size() > 0) {
			hand.insert(hand.begin() + idx, deck[0]);
			deck.erase(deck.begin());
			if (event->sendToDeck) {
				deck.push_back(replaced);
				Shuffle();
			}
		}

		//If no cards in deck, put replaced card back in hand
		else if (event->sendToDeck)
			hand.insert(hand.begin() + idx, replaced);

		//Decrement replace count
		--replaces;

		//Trigger replace and delet event
		delete event;

	}
}

//Change max replaces
void Player::ModifyReplaceCount(int delta) {
	maxReplaces += delta;
	if (delta > 0) { replaces += delta; }
	else { replaces = min(replaces, maxReplaces); }
}

#pragma endregion

//Getters
std::vector<Card*> Player::GetDeckAndHand() {
	std::vector<Card*> valid;
	valid.reserve(hand.size() + deck.size());
	valid.insert(valid.end(), hand.begin(), hand.end());
	valid.insert(valid.end(), deck.begin(), deck.end());
	return valid;
}