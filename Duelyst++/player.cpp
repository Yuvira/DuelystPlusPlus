//Include
#include "game.h"

//Game constructor / destructor
Player::Player() {}
Player::~Player() {}

//Preset deck
void Player::preset(CardList& cl, Game* g) {
	game = g;
	deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Argeon Highmayne")))));
	for (int a = 0; a < 2; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Day Watcher"))))); }
	for (int a = 0; a < 2; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Deathblighter"))))); }
	for (int a = 0; a < 2; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Dark Nemesis"))))); }
	for (int a = 0; a < 2; ++a) { deck.push_back(new Unit(*(dynamic_cast<Unit*>(cl.find("Dagger Kiri"))))); }
	for (int a = 0; a < 2; ++a) { deck.push_back(new Spell(*(dynamic_cast<Spell*>(cl.find("Daemonic Lure"))))); }
	for (int a = 0; a < 2; ++a) { deck.push_back(new Spell(*(dynamic_cast<Spell*>(cl.find("Consuming Rebirth"))))); }
	for (int a = 0; a < deck.size(); ++a) {
		deck[a]->game = game;
		deck[a]->player = this;
	}
}

//Initialize deck/hand
void Player::init(int _mana) {
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
		if (hand.size() < 6) {
			hand.push_back(deck[0]);
			game->em.sendOnDraw(hand.back(), true);
		}
		else {
			game->grave.push_back(deck[0]);
			game->em.sendOnDraw(game->grave.back(), true);
		}
		deck.erase(deck.begin());
	}
}

//Add card to hand
void Player::addToHand(Card* c, bool cast) {
	if (cast) {
		if (c->type == CARD_UNIT) { c = new Unit(*(dynamic_cast<Unit*>(c))); }
		else if (c->type == CARD_SPELL) { c = new Spell(*(dynamic_cast<Spell*>(c))); }
	}
	game->setContext(c, this);
	if (hand.size() < 6) {
		hand.push_back(c);
		game->em.sendOnDraw(hand.back(), false);
	}
	else {
		game->grave.push_back(c);
		game->em.sendOnDraw(game->grave.back(), false);
	}
}

//Replace card
void Player::replace(int i) {
	if (replaces > 0 && hand.size() > i) {
		if (hand[i]->onReplace()) {
			deck.push_back(hand[i]);
			hand.erase(hand.begin() + i);
			shuffle();
		}
		hand.insert(hand.begin() + i, deck[0]);
		deck.erase(deck.begin());
		--replaces;
	}
}