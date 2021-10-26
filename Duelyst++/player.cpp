//Include
#include "player.h"

//Game constructor / destructor
Player::Player() {}
Player::~Player() {}

//Preset deck
void Player::preset(CardList& cl) {
	deck.push_back(cl.find("Argeon Highmayne"));
	for (int a = 0; a < 13; ++a) { deck.push_back(cl.find("Bloodshard Golem")); }
	for (int a = 0; a < 13; ++a) { deck.push_back(cl.find("Brightmoss Golem")); }
	for (int a = 0; a < 13; ++a) { deck.push_back(cl.find("Azure Herald")); }
}

//Initialize deck/hand
void Player::init(int _mana) {
	mana = _mana;
	manaMax = _mana;
	for (int a = 0; a < 9; ++a) {
		crystal[a].buffer[0].Char.AsciiChar = '';
		crystal[a].setCol(COLOR_GRAY);
	}
	shuffle();
	for (int a = 0; a < 5; ++a) { hand.push_back(deck[a]); }
	deck.erase(deck.begin(), deck.begin() + 5);
}

//Update mana sprites
void Player::updateMana(eColor col) {
	for (int a = 0; a < mana; ++a) { crystal[a].setCol(col); }
	for (int a = mana; a < manaMax; ++a) { crystal[a].setCol(COLOR_LTWHITE); }
	for (int a = manaMax; a < 9; ++a) { crystal[a].setCol(COLOR_GRAY); }
}

//Shuffle deck
void Player::shuffle() {
	std::vector<Card*> shuffle;
	for (int a = 0; a < deck.size(); ) {
		int i = rand() % deck.size();
		shuffle.push_back(deck[i]);
		deck.erase(deck.begin() + i);
	}
	for (int a = 0; a < shuffle.size(); ++a) { deck.push_back(shuffle[a]); }
	shuffle.clear();
}