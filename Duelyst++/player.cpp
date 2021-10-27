//Include
#include "game.h"

//Game constructor / destructor
Player::Player() {}
Player::~Player() {}

//Preset deck
void Player::preset(CardList& cl) {
	deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Argeon Highmayne")))));
	for (int a = 0; a < 7; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Ghost Lynx"))))); }
	for (int a = 0; a < 8; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Araki Headhunter"))))); }
	for (int a = 0; a < 8; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Azure Herald"))))); }
	for (int a = 0; a < 8; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Aethermaster"))))); }
	for (int a = 0; a < 8; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Dragonlark"))))); }
}

//Initialize deck/hand
void Player::init(int _mana, Game* g) {
	game = g;
	mana = _mana;
	manaMax = _mana;
	replaces = 1;
	for (int a = 0; a < 9; ++a) {
		uiCrystal[a].buffer[0].Char.AsciiChar = '';
		uiCrystal[a].setCol(COLOR_GRAY);
	}
	shuffle();
	for (int a = 0; a < 5; ++a) { hand.push_back(deck[a]); }
	deck.erase(deck.begin(), deck.begin() + 5);
}

//Update mana sprites
void Player::updateMana(eColor col) {
	for (int a = 0; a < mana; ++a) { uiCrystal[a].setCol(col); }
	for (int a = mana; a < manaMax; ++a) { uiCrystal[a].setCol(COLOR_LTWHITE); }
	for (int a = manaMax; a < 9; ++a) { uiCrystal[a].setCol(COLOR_GRAY); }
}

//Render UI
void Player::render(Renderer& rm, bool left) {

	//Mana crystals
	for (int a = 0; a < 9; ++a) { rm.render(uiCrystal[a], left ? (a * 2) + 2 : 62 - (a * 2), 2); }

	//Cards in hand/deck
	std::string s = std::to_string(hand.size()) + "/6 " + std::to_string(deck.size()) + "/40";
	if (!left) {s = std::to_string(deck.size()) + "/40 " + std::to_string(hand.size()) + "/6"; }
	Sprite _s;
	_s.createFromString(s);
	rm.render(_s, left ? 20 : 45 - s.length(), 2);

	//Replace
	if (&game->player[game->turn] == this) {
		uiReplace.buffer[0].Char.AsciiChar = std::to_string(replaces)[0];
		rm.render(uiReplace, 4, 44);
	}

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

//Draw card
void Player::draw() {
	if (deck.size() > 0) {
		if (hand.size() < 6) { hand.push_back(deck[0]); }
		else { game->grave.push_back(deck[0]); }
		deck.erase(deck.begin());
	}
}

//Replace card
void Player::replace(int i) {
	if (replaces > 0 && hand.size() > i) {
		deck.push_back(hand[i]);
		hand.erase(hand.begin() + i);
		shuffle();
		hand.insert(hand.begin() + i, deck[0]);
		deck.erase(deck.begin());
		--replaces;
	}
}