//Include
#include "game.h"

//Game constructor / destructor
Player::Player() {
	mana = 0;
	manaMax = 0;
	replaces = 0;
	game = nullptr;
	general = nullptr;
	opponent = nullptr;
}
Player::~Player() {}

//Preset deck
void Player::Preset(CardList& cardList, Game* _game) {
	game = _game;
	deck.push_back(new Minion(*(dynamic_cast<Minion*>(cardList.Find("Argeon Highmayne")))));
	for (int i = 0; i < 3; ++i) { deck.push_back(new Minion(*(dynamic_cast<Minion*>(cardList.Find("Lady Locke"))))); }
	for (int i = 0; i < 3; ++i) { deck.push_back(new Minion(*(dynamic_cast<Minion*>(cardList.Find("Komodo Charger"))))); }
	for (int i = 0; i < 3; ++i) { deck.push_back(new Minion(*(dynamic_cast<Minion*>(cardList.Find("Ephemeral Shroud"))))); }
	for (int i = 0; i < 3; ++i) { deck.push_back(new Minion(*(dynamic_cast<Minion*>(cardList.Find("Araki Headhunter"))))); }
	//for (int a = 0; a < 3; ++a) { deck.push_back(new Spell(*(dynamic_cast<Spell*>(cl.find("Dark Transformation"))))); }
	for (int i = 0; i < deck.size(); ++i) {
		deck[i]->game = game;
		deck[i]->owner = this;
	}
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

//Update mana sprites
void Player::UpdateMana(eColor color) {
	for (int i = 0; i < mana; ++i)
		uiCrystal[i].SetColor(color);
	for (int i = mana; i < manaMax; ++i)
		uiCrystal[i].SetColor(COLOR_LTWHITE);
	for (int i = max(mana, manaMax); i < 9; ++i)
		uiCrystal[i].SetColor(COLOR_GRAY);
}

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
		if (card->cardType == CARD_UNIT)
			card = new Minion(*(dynamic_cast<Minion*>(card)));
		else if (card->cardType == CARD_SPELL)
			card = new Spell(*(dynamic_cast<Spell*>(card)));
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