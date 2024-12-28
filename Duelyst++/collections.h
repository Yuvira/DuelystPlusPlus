//Defines
#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

//Include
#include "card.h"

//Collections class
class Collections {
public:
	Collections();
	~Collections();
	Effect FindEffect(eEffect effect);
	Card* FindCard(std::string name);
	std::vector<Effect> effectList;
	std::vector<Card*> cardList;
	std::vector<Minion> generalList;
	std::vector<Minion> minionList;
	std::vector<Minion> minionTokenList;
	std::vector<Spell> spellList;
	std::vector<Spell> spellTokenList;
};

#endif