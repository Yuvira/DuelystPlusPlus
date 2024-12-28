//Include
#include "game.h"

#pragma region Constructor

//Game constructor
Player::Player() {
	mana = 0;
	manaMax = 0;
	replaces = 0;
	game = nullptr;
	general = nullptr;
	opponent = nullptr;
}
Player::~Player() {}

#pragma endregion

#pragma region Initialization

//Preset deck
void Player::Preset(Collections& collections, Game* _game) {
	game = _game;
	deck.push_back(new Minion(*(collections.FindCard("Argeon Highmayne")->GetMinion())));
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections.FindCard("Komodo Charger")->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections.FindCard("Ash Mephyt")->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Minion(*(collections.FindCard("Bloodtear Alchemist")->GetMinion()))); }
	for (int i = 0; i < 7; ++i) { deck.push_back(new Spell(*(collections.FindCard("Breath of The Unborn")->GetSpell()))); }
	for (int i = 0; i < deck.size(); ++i)
		game->SetContext(deck[i], this);
}

//Initialize deck/hand
void Player::Init(int _mana) {
	mana = _mana;
	manaMax = _mana;
	replaces = 1;
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
	if (&game->players[game->turn] == this) {
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
		if (hand.size() < 6) {
			hand.push_back(deck[0]);
			game->eventManager.SendOnDraw(hand.back(), true);
		}
		else {
			game->grave.push_back(deck[0]);
			game->eventManager.SendOnDraw(game->grave.back(), true);
		}
		deck.erase(deck.begin());
	}
}

//Add card to hand
void Player::AddToHand(Card* card, bool cast) {
	if (cast) {
		if (card->cardType == CARD_MINION)
			card = new Minion(*(card->GetMinion()));
		else if (card->cardType == CARD_SPELL)
			card = new Spell(*(card->GetSpell()));
	}
	game->SetContext(card, this);
	if (hand.size() < 6) {
		hand.push_back(card);
		game->eventManager.SendOnDraw(hand.back(), false);
	}
	else {
		game->grave.push_back(card);
		game->eventManager.SendOnDraw(game->grave.back(), false);
	}
}

//Replace card
void Player::Replace(int idx) {
	if (replaces > 0 && hand.size() > idx) {
		game->eventManager.SendOnReplace(hand[idx]);
		deck.push_back(hand[idx]);
		hand.erase(hand.begin() + idx);
		Shuffle();
		hand.insert(hand.begin() + idx, deck[0]);
		deck.erase(deck.begin());
		--replaces;
	}
}

#pragma endregion