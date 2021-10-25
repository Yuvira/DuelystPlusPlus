//Include
#include "player.h"

//Game constructor / destructor
Player::Player() {}
Player::~Player() {}

//Initialize deck/hand
void Player::init(CardList& cl) {
	for (int a = 0; a < 20; ++a) { deck.push_back(cl.clist[1]); }
	for (int a = 0; a < 20; ++a) { deck.push_back(cl.clist[2]); }

	shuffle();

	for (int a = 0; a < 5; ++a) { hand.push_back(deck[a]); }
	deck.erase(deck.begin(), deck.begin() + 5);
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