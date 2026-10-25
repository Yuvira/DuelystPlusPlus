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
Player::Player(Game* game, int mana) {

	//Set values
	manaMax = mana;
	this->mana = mana;
	maxReplaces = 1;
	replaces = 1;
	this->game = game;
	general = nullptr;
	opponent = nullptr;

	//Init UI
	for (int i = 0; i < 9; ++i)
		uiCrystal[i].CreateFromChar('', COLOR_GRAY);

}
Player::~Player() {}

#pragma endregion

#pragma region Setup

//Shuffle all cards into deck and draw 5
void Player::InitStartingHand() {
	for (PlayableCard* card : hand)
		deck.push_back(card);
	hand.clear();
	Shuffle();
	if (deck.size() < 5) {
		hand = deck;
		deck.clear();
	}
	else {
		for (int i = 0; i < 5; ++i)
			hand.push_back(deck[i]);
		deck.erase(deck.begin(), deck.begin() + 5);
	}
	for (PlayableCard* card : GetDeckAndHand())
		card->UpdateAuras();
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
		uiReplace.CreateFromString(std::to_string(replaces), COLOR_LTWHITE);
		renderer.Render(uiReplace, 4, 44);
	}

}

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
	std::vector<PlayableCard*> shuffle;
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
		PlayableCard* card = deck[0];
		deck.erase(deck.begin());
		if (hand.size() < 6) {
			hand.push_back(card);
			hand.back()->UpdateAuras();
		}
		else
			game->Overflow(card);
		game->QueueEvent(new OnDrawEvent(game->GetTurnCount(), card));
	}
}

//Add card to hand
void Player::AddNewToHand(eCard cardId) {
	if (cardId == CARD_NONE || game == nullptr)
		return;
	PlayableCard* newCard = game->collections->GetNewPlayableCardInstance(cardId, game, this);
	if (newCard == nullptr)
		return;
	if (hand.size() < 6) {
		hand.push_back(newCard);
		hand.back()->UpdateAuras();
	}
	else
		game->Overflow(newCard);
	game->QueueEvent(new OnAddToHandEvent(game->GetTurnCount(), newCard));
}

//Add card to deck
//No event call because this is only being used by CardViewer for now, update this when it becomes relevant to gameplay
void Player::AddNewToDeck(eCard cardId) {
	if (cardId == CARD_NONE || game == nullptr)
		return;
	PlayableCard* newCard = game->collections->GetNewPlayableCardInstance(cardId, game, this);
	if (newCard == nullptr)
		return;
	deck.push_back(newCard);
	deck.back()->UpdateAuras();
}

//Replace card
void Player::Replace(int idx) {
	if (replaces > 0 && deck.size() > 0 && hand.size() > idx) {

		//Remove card from hand
		PlayableCard* replaced = hand[idx];
		hand.erase(hand.begin() + idx);

		//Draw new card
		hand.insert(hand.begin() + idx, deck[0]);
		deck.erase(deck.begin());
		hand[idx]->UpdateAuras();

		//Put replaced card in deck and shuffle
		deck.push_back(replaced);
		deck.back()->UpdateAuras();
		Shuffle();

		//Decrement replace count
		--replaces;

		//Queue event
		game->QueueEvent(new OnReplaceEvent(game->GetTurnCount(), replaced));

	}
}

//Remove card from hand or deck
bool Player::RemoveFromHand(int idx) {
	if (idx < 0 || idx >= hand.size())
		return false;
	hand.erase(hand.begin() + idx);
	return true;
}
bool Player::RemoveFromHand(PlayableCard* card) {
	for (int i = 0; i < hand.size(); ++i)
		if (hand[i] == card)
			return RemoveFromHand(i);
	return false;
}
bool Player::RemoveFromDeck(int idx) {
	if (idx < 0 || idx >= deck.size())
		return false;
	deck.erase(deck.begin() + idx);
	return true;
}
bool Player::RemoveFromDeck(PlayableCard* card) {
	for (int i = 0; i < deck.size(); ++i)
		if (deck[i] == card)
			return RemoveFromDeck(i);
	return false;
}

#pragma endregion

#pragma region Resource Management

//Add to current mana
void Player::AddMana(int add) {
	mana += add;
	mana = min(mana, manaMax);
}

//Change mana limit
void Player::AddMaxMana(int add) {
	manaMax += add;
	manaMax = min(manaMax, 9);
	mana = min(mana, manaMax);
}

//Spend mana, returns false if not enough
bool Player::SpendMana(int spend) {
	if (mana < spend)
		return false;
	mana -= spend;
	return true;
}

//Change max replaces
void Player::AddMaxReplaces(int add) {
	maxReplaces += add;
	if (add > 0) { replaces += add; }
	else { replaces = min(replaces, maxReplaces); }
}

//Refresh mana and replace count
void Player::Refresh() {
	mana = manaMax;
	replaces = maxReplaces;
}

#pragma endregion

#pragma region Utils

//Get all cards from deck and hand
std::vector<PlayableCard*> Player::GetDeckAndHand() {
	std::vector<PlayableCard*> valid;
	valid.reserve(hand.size() + deck.size());
	valid.insert(valid.end(), hand.begin(), hand.end());
	valid.insert(valid.end(), deck.begin(), deck.end());
	return valid;
}

//Check if player has a specific card in their hand
bool Player::HasCardInHand(Card* match) {
	for (PlayableCard* card : hand)
		if (card == match)
			return true;
	return false;
}

#pragma endregion