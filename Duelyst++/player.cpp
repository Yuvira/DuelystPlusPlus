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
	deck.push_back(new Unit(*(collections->FindCard(CARD_ARGEON_HIGHMAYNE)->GetUnit())));
	for (int i = 0; i < deck.size(); ++i)
		this->game->SetContext(deck[i], this);
}

//Initialize deck/hand
void Player::Init(int mana) {

	//Init values and UI
	this->mana = mana;
	manaMax = mana;
	replaces = maxReplaces;
	for (int i = 0; i < 9; ++i) {
		uiCrystal[i].buffer[0].Char.AsciiChar = '';
		uiCrystal[i].SetColor(COLOR_GRAY);
	}

	//Shuffle and draw 5
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

}

//Shuffle hand into deck and re-draw starting hand
void Player::ReInit() {
	for (Card* card : hand)
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
void Player::AddNewToHand(Card* original) {
	Card* newCard = nullptr;
	if (original->IsUnit())
		newCard = new Unit(*(original->GetUnit()));
	else if (original->IsSpell())
		newCard = new Spell(*(original->GetSpell()));
	game->SetContext(newCard, this);
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
void Player::AddNewToDeck(Card* original) {
	Card* newCard = nullptr;
	if (original->IsUnit())
		newCard = new Unit(*(original->GetUnit()));
	else if (original->IsSpell())
		newCard = new Spell(*(original->GetSpell()));
	game->SetContext(newCard, this);
	deck.push_back(newCard);
	deck.back()->UpdateAuras();
}

//Replace card
void Player::Replace(int idx) {
	if (replaces > 0 && deck.size() > 0 && hand.size() > idx) {

		//Remove card from hand
		Card* replaced = hand[idx];
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

//Change max replaces
void Player::ModifyReplaceCount(int delta) {
	maxReplaces += delta;
	if (delta > 0) { replaces += delta; }
	else { replaces = min(replaces, maxReplaces); }
}

//Remove card from deck
bool Player::RemoveFromDeck(Card* card) {
	for (int i = 0; i < deck.size(); ++i) {
		if (deck[i] == card) {
			deck.erase(deck.begin() + i);
			return true;
		}
	}
	return false;
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

//Check if player has a specific card in their hand
bool Player::HasCardInHand(Card* match) {
	for (Card* card : hand)
		if (card == match)
			return true;
	return false;
}