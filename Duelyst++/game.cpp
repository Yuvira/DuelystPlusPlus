//Include
#include <conio.h>
#include "game.h"
#include "card.h"
#include "collections.h"
#include "event.h"
#include "renderer.h"

#pragma region Helper Constructors

//Callback constructor
EffectCallback::EffectCallback() : EffectCallback(EffectContext(nullptr, nullptr, nullptr), nullptr, nullptr) {}
EffectCallback::EffectCallback(EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets)) {
	this->context = ctx;
	this->source = source;
	this->Callback = Callback;
}
EffectCallback::~EffectCallback() {}

//Execute effect callback
void EffectCallback::Execute(std::vector<BoardTile*>& targets) {
	if (Callback)
		Callback(context, source, targets);
}

//Path co-ordinate constructor
PathCoord::PathCoord() : PathCoord(Coord(), 0, 0) {}
PathCoord::PathCoord(Coord pos, int last, int count) {
	this->pos = pos;
	this->last = last;
	this->count = count;
}
PathCoord::~PathCoord() {}

#pragma endregion

#pragma region Game Constructor

//Game constructor
Game::Game(Collections* collections, bool* modeSwitch) : players{ Player(this, IS_DEBUG ? 9 : 2), Player(this, IS_DEBUG ? 9 : 3) } {

	//Collections reference
	this->collections = collections;
	this->modeSwitch = modeSwitch;

	//Sprites
	board.CreateFromFile("resources/board.txt");
	tileBorder.CreateFromFile("resources/tile.txt");

	//Initialize debug log
	debugLog.push_back("{Log Start}");
	debugSprite.CreateFromString(debugLog.back());
	debugMode = false;

	//Character sprites
	char c[] = { 'Û', 'Þ', 'Ý', 'Ü', 'ß', 'X', '\\', '/', '³', 'Ä' };
	for (int i = 0; i < 10; ++i)
		chars[i].CreateFromChar(c[i]);

	//Turn indicator
	light.CreateFromString("®é¯", COLOR_LTBLUE);

	//Initialize players
	for (int i = 0; i < 2; ++i) {
		Summon(collections->GetNewUnitInstance(CARD_ARGEON_HIGHMAYNE, this, &players[i]), map.GetTile(i * 8, 2), false);
		players[i].general = units.back();
		players[i].InitStartingHand();
	}
	players[0].opponent = &players[1];
	players[1].opponent = &players[0];

	//Generate mana tiles
	CreateTile(CARD_MANA_SPRING, map.GetTile(4, 0), nullptr);
	CreateTile(CARD_MANA_SPRING, map.GetTile(5, 2), nullptr);
	CreateTile(CARD_MANA_SPRING, map.GetTile(4, 4), nullptr);

	//Variables
	pos = Coord(0, 0);
	handIdx = -1;
	selectionIdx = -1;
	turnCount = 0;
	endTurn = false;
	ChangeTurn(false);
	MoveCursor(0, 0);
	activeUnit = nullptr;
	mode = MODE_NONE;

}
Game::~Game() {}

#pragma endregion

#pragma region Rendering

//Render objects
void Game::RenderGame(Renderer& renderer) {

	//Board
	renderer.Render(board, 0, 0);

	//Turn indicator
	renderer.Render(light, 31, 2);

	//Tile borders
	for (int x = 0; x < map.GetWidth(); ++x) {
		for (int y = 0; y < map.GetHeight(); ++y) {
			tileBorder.SetColor(map.GetTile(x, y)->borderColor);
			RenderTile(renderer, x, y);
		}
	}

	//Tiles
	for (Tile* tile : tiles)
		RenderSprite(renderer, tile->sprite, tile->curTile->pos);

	//Units
	for (Unit* unit : units)
		if (unit->curTile != nullptr)
			unit->RenderOnBoard(renderer, SpriteX(unit->curTile->pos.x), SpriteY(unit->curTile->pos.y));

	//Render active card while selecting opening gambit targets
	if (activeCard != nullptr && activeCard->IsUnit() && mode == MODE_SELECT && effectCallback.Callback)
		activeCard->GetUnit()->RenderOnBoard(renderer, SpriteX(castPos.x), SpriteY(castPos.y));

	//Move mode indicators
	if (mode == MODE_MOVE) {
		BoardTile* atkTile = nullptr;
		for (BoardTile* tile : attackable)
			if (tile == selectable[selectionIdx])
				atkTile = tile;
		if (atkTile != nullptr) {
			DrawSword(TileX(selectable[selectionIdx]->pos.x), TileY(selectable[selectionIdx]->pos.y), renderer);
			if (selectable[selectionIdx]->unit->CanAttack(activeUnit))
				DrawSword(TileX(activeUnit->curTile->pos.x), TileY(activeUnit->curTile->pos.y), renderer);
		}
		else
			DrawPath(renderer);
	}

	//Hand
	for (int x = 0; x < handSize; ++x) {
		tileBorder.SetColor(COLOR_LTWHITE);
		if (x == handIdx + 1 && mode == MODE_HAND)
			tileBorder.SetColor(COLOR_AQUA);
		renderer.Render(tileBorder, TileX(x), 41);
	}
	for (int x = 0; x < players[turn].hand.size(); ++x) {
		players[turn].hand[x]->sprite.SetColor(COLOR_LTWHITE);
		renderer.Render(players[turn].hand[x]->sprite, SpriteX(x + 1), 42);
	}

	//Player UI
	players[0].Render(renderer, true);
	players[1].Render(renderer, false);

	//Sidebar
	RenderSidebar(renderer);

	//Debug
	RenderDebug(renderer);

}

//Render objects at board position
void Game::RenderTile(Renderer& renderer, Coord xy) { RenderTile(renderer, xy.x, xy.y); }
void Game::RenderTile(Renderer& renderer, int x, int y) { renderer.Render(tileBorder, TileX(x), TileY(y)); }
void Game::RenderSprite(Renderer& renderer, Sprite& sprite, Coord xy) { RenderSprite(renderer, sprite, xy.x, xy.y); }
void Game::RenderSprite(Renderer& renderer, Sprite& sprite, int x, int y) { renderer.Render(sprite, SpriteX(x), SpriteY(y)); }

//Render sidebar
void Game::RenderSidebar(Renderer& renderer) {

	//Vertical position
	int y = 1;

	//Unit on board
	if (mode == MODE_NONE) {
		if (map.GetTile(pos)->HasUnit())
			map.GetTile(pos)->unit->DrawDetails(renderer, y);
		else if (map.GetTile(pos)->HasTile())
			map.GetTile(pos)->tile->DrawDetails(renderer, y);
	}

	//Card in hand
	else if (mode == MODE_HAND || mode == MODE_SELECT) {

		//Show target for current effect
		if (mode == MODE_SELECT && effectCallback.Callback) {
			if (selectable[selectionIdx]->unit != nullptr)
				selectable[selectionIdx]->unit->DrawDetails(renderer, y);
		}

		//Otherwise show card in hand
		else if (handIdx < players[turn].hand.size())
			players[turn].hand[handIdx]->DrawDetails(renderer, y);

	}

}

#pragma region Debug Render

//Render debug info
void Game::RenderDebug(Renderer& renderer) {

	//Skip if not debug mode
	if (!IS_DEBUG)
		return;

	//Draw deck / field / grave info
	if (debugMode) {

		//Lines object
		std::vector<std::string> lines;

		//Player 1 deck data
		lines.push_back("{P1 Deck}");
		for (Card* card : players[0].deck)
			lines.push_back(card->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 115, 1);
		lines.clear();

		//Player 2 deck data
		lines.push_back("{P2 Deck}");
		for (Card* card : players[1].deck)
			lines.push_back(card->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 140, 1);
		lines.clear();

		//Unit data
		lines.push_back("{Units}");
		for (Card* card : units)
			lines.push_back(card->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 165, 1);
		lines.clear();

		//Graveyard data
		lines.push_back("{Graveyard}");
		for (Card* card : grave)
			lines.push_back(card->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 190, 1);
		lines.clear();

		//Aura data
		lines.push_back("{Auras}");
		for (Aura* aura : auras)
			lines.push_back(aura->GetSourceCard()->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 215, 1);
		lines.clear();

		//Tile data
		lines.push_back("{Tiles}");
		for (Tile* tile : tiles)
			lines.push_back(tile->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 190, 25);
		lines.clear();

		//Delayed trigger data
		lines.push_back("{Delayed Triggers}");
		for (Spell* spell : delayedTriggers)
			lines.push_back(spell->name);
		debugSprite.CreateFromTextBlock(lines);
		renderer.Render(debugSprite, 215, 25);

	}

	//Draw log info
	else {
		debugSprite.CreateFromTextBlock(debugLog);
		renderer.Render(debugSprite, 115, 1);
	}

}

#pragma endregion

#pragma endregion

#pragma region Input

//Input
void Game::Input() {

	//Get keyPress
	int asciiVal = _getch();

	//Navigating board
	if (mode == MODE_NONE) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { MoveCursor(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveCursor(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveCursor(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveCursor(1, 0); } //D

		//Select unit
		else if (asciiVal == 32) {                                        //Space
			if (map.tiles[pos.x][pos.y].unit != nullptr)
				if (map.tiles[pos.x][pos.y].unit->owner == &players[turn])
					SelectTile(map.tiles[pos.x][pos.y]);
		}

		//Change turn
		else if (asciiVal == 10 || asciiVal == 13)                        //Enter
			ChangeTurn(!turn);

		//Debug inputs
		else if (IS_DEBUG) {

			//Damage or destroy target
			if (asciiVal == 122 || asciiVal == 90) {                      //Z
				if (map.tiles[pos.x][pos.y].unit != nullptr)
					map.tiles[pos.x][pos.y].unit->DealDamage(players[turn].general, 1);
			}
			else if (asciiVal == 120 || asciiVal == 88) {                 //X
				if (map.tiles[pos.x][pos.y].unit != nullptr)
					map.tiles[pos.x][pos.y].unit->Destroy(players[turn].general);
			}

			//Swap debug info page
			else if (asciiVal == 108 || asciiVal == 76) {                 //L
				debugMode = !debugMode;
			}

		}

	}

	//Navigating hand
	else if (mode == MODE_HAND) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { MoveCursorHand(0, 4); }       //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveCursorHand(-1, -1); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveCursorHand(0, 0); }  //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveCursorHand(1, -1); } //D

		//Replace card
		else if (asciiVal == 114 || asciiVal == 82)                            //R
			Replace();

		//Select card
		else if (asciiVal == 32)                                               //Space
			if (handIdx < players[turn].hand.size())
				SelectCard();

	}

	//Moving a unit
	else if (mode == MODE_MOVE) {

		//Move arrow
		if (asciiVal == 119 || asciiVal == 87) { MoveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveSelect(1, 0); } //D

		//Move unit
		else if (asciiVal == 32) {                                        //Space
			for (int a = 0; a < attackable.size(); ++a) {
				if (attackable[a] == selectable[selectionIdx]) {
					AttackUnit();
					goto end;
				}
			}
			MoveUnit();
		end:;
		}

	}

	//Selecting targets
	else if (mode == MODE_SELECT) {

		//Move selection
		if (asciiVal == 119 || asciiVal == 87) { MoveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveSelect(1, 0); } //D

		//Use card at selection
		else if (asciiVal == 32) {                                        //Space
			if (effectCallback.Callback)
				UseEffect();
			else
				UseCard();
			if (endTurn) {
				Update();
				ChangeTurn(!turn);
			}
		}

	}

	//Switch mode
	if (asciiVal == 80 || asciiVal == 112) { *modeSwitch = false; } //P

}

#pragma endregion

#pragma region Updates

//Update loop
void Game::Update() {

	//Update unit states and process events until no units are dead and no events are queued
	bool reloop = true;
	while (reloop) { 
		reloop = false;
		for (int a = 0; a < units.size(); ++a)
			if (!units[a]->isDead)
				units[a]->Update(reloop);
		if (eventStack.size() > 0) {
			ProcessEventStack();
			reloop = true;
		}
	}

	//Clean up dead units from board and update stat sprites
	for (int i = 0; i < units.size(); ++i) {
		if (units[i]->isDead) {
			units[i]->RemoveEffects();
			if (units[i] == units[i]->curTile->unit)
				units[i]->curTile->unit = nullptr;
			units[i]->curTile = nullptr;
			grave.push_back(units[i]);
			destroyedUnits.push_back(units[i]);
			units.erase(units.begin() + i);
			--i;
		}
	}

	//Update mana bars
	players[0].UpdateMana(COLOR_LTBLUE);
	players[1].UpdateMana(COLOR_RED);

	//Clear highlights
	for (int x = 0; x < map.GetWidth(); ++x)
		for (int y = 0; y < map.GetHeight(); ++y)
			map.GetTile(x, y)->borderColor = COLOR_LTWHITE;

	//Highlight hostiles
	hostile.clear();
	for (Unit* unit : units) {
		if (unit->owner != &players[turn]) {
			hostile.push_back(unit->curTile);
			unit->curTile->borderColor = COLOR_LTRED;
		}
	}

	//Highlight moveable spaces and create move path
	if (mode == MODE_MOVE) {
		GeneratePaths();
		for (BoardTile* tile : highlighted)
			tile->borderColor = COLOR_AQUA;
		map.GetTile(pos)->borderColor = COLOR_LTWHITE;
		for (BoardTile* tile : attackable)
			tile->borderColor = COLOR_RED;
	}

	//Highlight selectable spaces
	else if (mode == MODE_SELECT) {
		for (BoardTile* tile : selectable)
			tile->borderColor = COLOR_GREEN;
		selectable[selectionIdx]->borderColor = COLOR_LTGREEN;
	}

	//Highlight cursor position
	else if (mode == MODE_NONE) {
		highlighted.clear();
		HighlightTile(pos.x, pos.y, COLOR_AQUA);
	}

}

#pragma endregion

#pragma region Events

//Add event to queue
void Game::QueueEvent(Event* event) { eventStack.push_back(event); }

//Process waiting events
void Game::ProcessEventStack() {
	if (eventStack.size() == 0)
		UpdateAuras();
	while (eventStack.size() > 0) {
		std::vector<Event*> eventsToProcess = eventStack;
		eventStack.clear();
		for (Event* event : eventsToProcess)
			eventManager.TriggerPostEvent(this, event);
		UpdateAuras();
	}
}

//Update auras on all cards
void Game::UpdateAuras() {
	for (Unit* unit : units)
		unit->UpdateAuras();
	for (PlayableCard* card : players[0].GetDeckAndHand())
		card->UpdateAuras();
	for (PlayableCard* card : players[1].GetDeckAndHand())
		card->UpdateAuras();
}

//Track spells that need to receive events post-resolution for delayed triggers
void Game::AddDelayedTrigger(Spell* spell) {
	if (spell != nullptr && !IsTrackingDelayedTrigger(spell))
		delayedTriggers.push_back(spell);
}
void Game::RemoveDelayedTrigger(Spell* spell) {
	for (int i = 0; i < delayedTriggers.size(); ++i) {
		if (delayedTriggers[i] == spell) {
			delayedTriggers.erase(delayedTriggers.begin() + i);
			return;
		}
	}
}
bool Game::IsTrackingDelayedTrigger(Spell* spell) { return std::find(delayedTriggers.begin(), delayedTriggers.end(), spell) != delayedTriggers.end(); }

#pragma endregion

#pragma region Effects

//Add effect to list
void Game::ApplyAura(eEffect auraId, Player* owner, Card* sourceCard, Effect* sourceEffect) {

	//Get effect instance
	if (auraId == EFFECT_NONE)
		return;
	Aura* newAura = collections->GetNewAuraInstance(auraId, owner, sourceCard, sourceEffect);
	if (newAura == nullptr)
		return;

	//Ignore if source is already applying this aura
	if (sourceEffect != nullptr)
		for (int i = 0; i < auras.size(); ++i)
			if (auras[i]->ReadID() == newAura->ReadID() && auras[i]->GetSourceEffect() == sourceEffect)
				return;

	//Add aura to list and update
	auras.push_back(newAura);
	UpdateAuras();

}

//Remove aura from list
void Game::RemoveAura(Aura* aura) {
	for (int i = 0; i < auras.size(); ++i) {
		if (auras[i] == aura) {
			RemoveAurasAt({ i });
			return;
		}
	}
}

//Remove all auras from a given source
void Game::RemoveAurasFromSource(Effect* sourceEffect) {
	std::vector<int> indices;
	for (int i = 0; i < auras.size(); ++i)
		if (auras[i]->GetSourceEffect() == sourceEffect)
			indices.push_back(i);
	RemoveAurasAt(indices);
}

//Remove effects at indices
void Game::RemoveAurasAt(std::vector<int> indices) {

	//Return if no indices provided
	if (indices.empty())
		return;

	//Remove auras
	std::vector<Aura*> removed;
	for (int i = indices.size() - 1; i >= 0; --i) {
		removed.push_back(auras[indices[i]]);
		auras.erase(auras.begin() + indices[i]);
	}

	//Update cards
	UpdateAuras();

}

//Check if aura is still being tracked
bool Game::IsTrackingAura(Aura* match) {
	for (Aura* aura : auras)
		if (aura == match)
			return true;
	return false;
}

#pragma endregion

#pragma region Actions

//Use active card
void Game::UseCard() {

	//Set position and clear selectables
	pos = selectable[selectionIdx]->pos;
	selectable.clear();
	selectionIdx = -1;
	mode = MODE_NONE;

	//Use mana, return if not enough available
	if (!players[turn].SpendMana(activeCard->cost))
		return;

	//Remove from hand and do pre-cast effect targeting
	players[turn].hand.erase(players[turn].hand.begin() + handIdx);
	for (Effect* effect : activeCard->GetEffects()) {
		OnPreCastThisEvent preCastEvent = OnPreCastThisEvent(&map.tiles[pos.x][pos.y]);
		effect->TriggerSelfEvent(EffectContext(effect, activeCard, this), &preCastEvent);
	}
	castPos = pos;

	//Reset hand position
	handIdx = -1;

	//Do post-cast if we're not targeting anything
	if (mode == MODE_NONE)
		PostCast();

}

//Use active callback effect
void Game::UseEffect() {

	//Get target and clear selectables
	BoardTile* tile = selectable[selectionIdx];
	pos = tile->pos;
	selectable.clear();
	selectionIdx = -1;
	mode = MODE_NONE;

	//Execute callback
	if (effectCallback.Callback) {
		callbackTargets.push_back(tile);
		effectCallback.Execute(callbackTargets);
		if (nextEffectCallback.Callback) {
			effectCallback = nextEffectCallback;
			nextEffectCallback = EffectCallback();
		}
		else {
			effectCallback = EffectCallback();
			callbackTargets.clear();
		}
	}

	//Trigger cast event for active card
	if (activeCard != nullptr)
		PostCast();

}

//Send cast event after pre-cast effect targets have been set
void Game::PostCast() {

	//Get target tile
	BoardTile* tile = &map.tiles[castPos.x][castPos.y];

	//Process cast event
	QueueEvent(new OnCastEvent(GetTurnCount(), activeCard, tile));
	ProcessEventStack();

	//Track card cast this turn
	castThisTurn.push_back(activeCard);

	//Trigger resolve event on card
	for (Effect* effect : activeCard->GetEffects()) {
		OnResolveThisEvent resolveEvent = OnResolveThisEvent(tile);
		effect->TriggerSelfEvent(EffectContext(effect, activeCard, this), &resolveEvent);
	}

	//Process post-cast behaviour
	if (activeCard->IsUnit())
		Summon(activeCard->GetUnit(), tile, true);
	else if (activeCard->IsSpell()) {
		grave.push_back(activeCard);
		spellHistory.push_back(activeCard->GetSpell());
	}
	ProcessEventStack();

	//Remove active card reference
	activeCard = nullptr;

}

//Move selected unit
void Game::MoveUnit() {
	if (selectable[selectionIdx]->unit == nullptr || selectable[selectionIdx] == &map.tiles[pos.x][pos.y]) {
		if (selectable[selectionIdx] != &map.tiles[pos.x][pos.y]) {
			activeUnit->MoveToTile(selectable[selectionIdx], false);
			pos = selectable[selectionIdx]->pos;
			ProcessEventStack();
		}
		moveable.clear();
		attackable.clear();
		selectable.clear();
		activeUnit = nullptr;
		mode = MODE_NONE;
	}
}

//Attack unit
void Game::AttackUnit() {

	//Set source and target
	Unit* source = activeUnit;
	Unit* target = selectable[selectionIdx]->unit;

	//Flag whether target can counterattack (in case it moves during combat or otherwise)
	bool canCounter = target->CanAttack(source);

	//Process attack event
	QueueEvent(new OnAttackEvent(GetTurnCount(), source, target, false));
	ProcessEventStack();

	//Attack target and process events
	source->Attack(target, false);
	ProcessEventStack();

	//Counterattack
	if (canCounter) {

		//Process counterattack event
		QueueEvent(new OnAttackEvent(GetTurnCount(), target, source, true));
		ProcessEventStack();

		//Attack target and process events
		target->Attack(source, true);
		ProcessEventStack();

	}

	//Clear selectables and return to normal interaction mode
	pos = selectable[selectionIdx]->pos;
	moveable.clear();
	attackable.clear();
	selectable.clear();
	activeUnit = nullptr;
	mode = MODE_NONE;

}

//Summon at tile or position
Unit* Game::Summon(Unit* unit, BoardTile* tile, bool actionBar) {
	units.push_back(unit);
	unit->SetTile(tile);
	unit->AddEffects();
	unit->UpdateAuras();
	unit->InitState();
	QueueEvent(new OnSummonEvent(GetTurnCount(), unit, actionBar));
	return unit;
}

//Summon token at tile
Unit* Game::SummonToken(eCard cardId, BoardTile* tile, Player* owner) {
	if (cardId == CARD_NONE)
		return nullptr;
	Unit* token = collections->GetNewUnitInstance(cardId, this, owner);
	if (token == nullptr)
		return nullptr;
	return Summon(token, tile, false);
}

//Create tile object on board tile
Tile* Game::CreateTile(eCard cardId, BoardTile* tile, Player* owner) {

	//Ensure validity
	if (cardId == CARD_NONE)
		return nullptr;
	Tile* newTile = collections->GetNewTileInstance(cardId, this, owner);
	if (newTile == nullptr)
		return nullptr;

	//Remove old tile object
	if (tile->HasTile())
		RemoveTile(tile->tile);

	//Add new tile object
	tiles.push_back(newTile);
	newTile->curTile = tile;
	tile->tile = newTile;
	newTile->AddEffects();

	//Set color
	if (newTile->owner == &players[0])
		newTile->sprite.SetColor(COLOR_LTBLUE);
	else if (newTile->owner == &players[1])
		newTile->sprite.SetColor(COLOR_LTRED);
	else
		newTile->sprite.SetColor(COLOR_LTWHITE);

	//Return new tile object
	return newTile;

}

//Remove tile object from board
void Game::DispelTile(BoardTile* tile) {
	if (tile->HasTile())
		RemoveTile(tile->tile);
	if (tile->HasUnit())
		tile->unit->Dispel();
}

//Remove tile object from board
bool Game::RemoveTile(Tile* tile) {
	for (int i = 0; i < tiles.size(); ++i) {
		if (tiles[i] == tile) {
			tiles[i]->RemoveEffects();
			if (tiles[i] == tiles[i]->curTile->tile)
				tiles[i]->curTile->tile = nullptr;
			tiles[i]->curTile = nullptr;
			grave.push_back(tiles[i]);
			tiles.erase(tiles.begin() + i);
			return true;
		}
	}
	return false;
}

//Replace active card
void Game::Replace() {
	players[turn].Replace(handIdx);
	ProcessEventStack();
}

//Change turn
void Game::ChangeTurn(bool newTurn) {

	//Indicate turn is ending
	endTurn = true;

	//Draw and trigger end of turn
	if (turnCount > 0) {
		players[turn].Draw();
		QueueEvent(new OnTurnEndEvent(GetTurnCount(), &players[turn]));
		ProcessEventStack();
	}

	//Clear cast history
	castThisTurn.clear();

	//Change turn
	turn = newTurn;
	++turnCount;

	//Refresh mana
	if (turnCount > 2)
		players[turn].AddMaxMana(1);
	players[turn].Refresh();

	//Refresh units
	for (Unit* unit : units)
		unit->Refresh();

	//Set turn indicator
	if (turn)
		light.SetColor(COLOR_RED);
	else
		light.SetColor(COLOR_LTBLUE);

	//Trigger start of turn
	QueueEvent(new OnTurnStartEvent(GetTurnCount(), &players[turn]));
	ProcessEventStack();

	//Turn has ended
	endTurn = false;

}

//Send card that can't be added to hand to graveyard instead
void Game::Overflow(Card* card) { grave.push_back(card); }

#pragma endregion

#pragma region Selection / Movement

//Select unit
void Game::SelectTile(BoardTile& tile) {
	for (int i = 0; i < highlighted.size(); ++i)
		moveable.push_back(highlighted[i]);
	if (!tile.unit->hasAttacked) {
		bool provoked = false;
		for (int i = max(tile.pos.x - 1, 0); i < min(tile.pos.x + 2, 9); ++i)
			for (int j = max(tile.pos.y - 1, 0); j < min(tile.pos.y + 2, 5); ++j)
				if (map.tiles[i][j].unit != nullptr)
					if (map.tiles[i][j].unit->owner != &players[turn])
						if (map.tiles[i][j].unit->HasKeyword(KEYWORD_PROVOKE))
							if (tile.unit->CanAttack(map.tiles[i][j].unit)) {
								attackable.push_back(&map.tiles[i][j]);
								provoked = true;
							}
		if (!provoked)
			for (int a = 0; a < hostile.size(); ++a)
				if (tile.unit->CanAttack(hostile[a]->unit))
					attackable.push_back(hostile[a]);
	}
	if (moveable.size() > 1 || attackable.size() > 0) {
		selectable.insert(selectable.end(), moveable.begin(), moveable.end());
		selectable.insert(selectable.end(), attackable.begin(), attackable.end());
		activeUnit = tile.unit;
		selectionIdx = 0;
		mode = MODE_MOVE;
	}
	else
		moveable.clear();
}

//Select card in hand
void Game::SelectCard() {
	if (players[turn].hand[handIdx]->cost <= players[turn].ReadMana()) {
		selectable = players[turn].hand[handIdx]->GetTargets();
		if (selectable.size() > 0) {
			activeCard = players[turn].hand[handIdx];
			selectionIdx = 0;
			mode = MODE_SELECT;
		}
	}
}

//Move cursor position
void Game::MoveCursor(int x, int y) {
	pos.x = (pos.x + x + 9) % 9;
	if ((y == -1 && pos.y == 0) || (y == 1 && pos.y == 4)) {
		handIdx = max(min(pos.x - 1, 5), 0);
		pos = Coord(-1, -1);
		mode = MODE_HAND;
	}
	else
		pos.y = (pos.y + y + 5) % 5;
}

//Move cursor position
void Game::MoveCursorHand(int x, int y) {
	handIdx = (handIdx + x + 6) % 6;
	if (y != -1) {
		pos = Coord(handIdx + 1, y);
		handIdx = -1;
		mode = MODE_NONE;
		return;
	}
}

//Move selected tiles
void Game::MoveSelect(int x, int y) {
	int _x = selectable[selectionIdx]->pos.x + x;
	int _y = selectable[selectionIdx]->pos.y + y;
	if (x != 0) {
		for (int i = 0; x > 0 ? i < 9 : i > -9; i += x) {
			for (int j = 0; j < 3; j > 0 ? j *= -1 : j = (j - 1) * -1) {
				int __x = (_x + i + 9) % 9;
				int __y = (_y + j + 5) % 5;
				for (int k = 0; k < selectable.size(); ++k) {
					if (&map.tiles[__x][__y] == selectable[k]) {
						selectionIdx = k;
						goto end;
					}
				}
			}
		}
	}
	else if (y != 0) {
		for (int i = 0; y > 0 ? i < 5 : i > -5; i += y) {
			for (int j = 0; j < 5; j > 0 ? j *= -1 : j = (j - 1) * -1) {
				int __x = (_x + j + 9) % 9;
				int __y = (_y + i + 5) % 5;
				for (int k = 0; k < selectable.size(); ++k) {
					if (&map.tiles[__x][__y] == selectable[k]) {
						selectionIdx = k;
						goto end;
					}
				}
			}
		}
	}
end:;
}

#pragma endregion

#pragma region Highlighting

//Highlight tiles
void Game::HighlightTile(int x, int y, eColor color) {
	map.GetTile(x, y)->borderColor = color;
	highlighted.push_back(map.GetTile(x, y));
	if (color == COLOR_AQUA && mode == MODE_NONE)
		if (map.tiles[pos.x][pos.y].unit != nullptr)
			if (!map.tiles[pos.x][pos.y].unit->hasMoved && map.tiles[pos.x][pos.y].unit->IsMoveable())
				if (map.tiles[pos.x][pos.y].unit->owner == &players[turn])
					if (!map.tiles[pos.x][pos.y].unit->IsProvoked())
						HighlightMoveable(pos.x, pos.y);
}

//Highlight moveable spaces
void Game::HighlightMoveable(int x, int y) {
	if (map.tiles[pos.x][pos.y].unit->HasKeyword(KEYWORD_FLYING)) {
		for (int i = 0; i < 9; ++i)
			for (int j = 0; j < 5; ++j)
				if (i != pos.x || j != pos.y)
					if (CanMove(i, j))
						HighlightTile(i, j, COLOR_GRAY);
	}
	else
		SearchMoveable(pos.x, pos.y, map.tiles[pos.x][pos.y].unit->moveRange);
	map.GetTile(x, y)->borderColor = COLOR_AQUA;
}

//Recursively search for moveable spaces
void Game::SearchMoveable(int x, int y, int range) {
	if (range == 0) { return; }
	if (CanMove(x, y - 1)) { HighlightTile(x, y - 1, COLOR_GRAY); SearchMoveable(x, y - 1, range - 1); } //Up
	if (CanMove(x - 1, y)) { HighlightTile(x - 1, y, COLOR_GRAY); SearchMoveable(x - 1, y, range - 1); } //Left
	if (CanMove(x, y + 1)) { HighlightTile(x, y + 1, COLOR_GRAY); SearchMoveable(x, y + 1, range - 1); } //Down
	if (CanMove(x + 1, y)) { HighlightTile(x + 1, y, COLOR_GRAY); SearchMoveable(x + 1, y, range - 1); } //Right
}

//Highlight targets and generate callback
void Game::HighlightTargetsForCallback(std::vector<BoardTile*>& targets, EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets)) {
	if (targets.size() == 0)
		return;
	selectable = targets;
	selectionIdx = 0;
	mode = MODE_SELECT;
	if (effectCallback.Callback)
		nextEffectCallback = EffectCallback(EffectContext(ctx.effect, ctx.card, ctx.game), source, Callback);
	else
		effectCallback = EffectCallback(EffectContext(ctx.effect, ctx.card, ctx.game), source, Callback);
}

#pragma endregion

#pragma region Pathfinding

//Get final path from list
void Game::CreatePath() {
	int idx = pathList.size() - 1;
	for (int i = 0; i < pathList.back().count + 1; ++i) {
		path.insert(path.begin(), pathList[idx].pos);
		idx = pathList[idx].last;
	}
}

//Add position to path list
bool Game::AddToPaths(int x, int y, int last, int count) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		for (int i = 0; i < pathList.size(); ++i)
			if (pathList[i].pos.x == x && pathList[i].pos.y == y)
				return false;
		if (map.tiles[pos.x][pos.y].unit->HasKeyword(KEYWORD_FLYING)) {
			pathList.push_back(PathCoord(Coord(x, y), last, count));
			if (selectable[selectionIdx]->pos.x == x && selectable[selectionIdx]->pos.y == y)
				return true;
			return false;
		}
		for (int i = 0; i < moveable.size(); ++i) {
			if (moveable[i] == &map.tiles[x][y]) {
				pathList.push_back(PathCoord(Coord(x, y), last, count));
				if (selectable[selectionIdx] == moveable[i])
					return true;
				return false;
			}
		}
	}
	return false;
}

//Generate paths to target
void Game::GeneratePaths() {
	path.clear();
	if (selectable[selectionIdx] != &map.tiles[pos.x][pos.y]) {
		for (int i = 0; i < attackable.size(); ++i)
			if (attackable[i] == selectable[selectionIdx])
				return;
		pathList.push_back(PathCoord(pos, 0, 0));
		int count = 0;
		while (true) {
			for (int i = 0; i < pathList.size(); ++i) {
				if (pathList[i].count == count) {
					if (AddToPaths(pathList[i].pos.x, pathList[i].pos.y - 1, i, pathList[i].count + 1)) { goto end; }
					if (AddToPaths(pathList[i].pos.x, pathList[i].pos.y + 1, i, pathList[i].count + 1)) { goto end; }
					if (AddToPaths(pathList[i].pos.x - 1, pathList[i].pos.y, i, pathList[i].count + 1)) { goto end; }
					if (AddToPaths(pathList[i].pos.x + 1, pathList[i].pos.y, i, pathList[i].count + 1)) { goto end; }
				}
			}
			++count;
		}
	end:;
		CreatePath();
		pathList.clear();
	}
}

#pragma endregion

#pragma region Drawing

//Draw arrow path
void Game::DrawPath(Renderer& renderer) {
	int pathSize = path.size() - 1;
	if (pathSize > 0) {
		Coord coord = Coord((path[0].x * 7) + 1, (path[0].y * 7) + 5);
		if (path[0].y > path[1].y) { DrawArrow(1, coord.x, coord.y, renderer); }      //Start up
		else if (path[0].x > path[1].x) { DrawArrow(2, coord.x, coord.y, renderer); } //Start left
		else if (path[0].y < path[1].y) { DrawArrow(3, coord.x, coord.y, renderer); } //Start down
		else if (path[0].x < path[1].x) { DrawArrow(4, coord.x, coord.y, renderer); } //Start right
		coord = Coord((path[pathSize].x * 7) + 1, (path[pathSize].y * 7) + 5);
		if (path[pathSize].y < path[pathSize - 1].y) { DrawArrow(5, coord.x, coord.y, renderer); }      //End up
		else if (path[pathSize].x < path[pathSize - 1].x) { DrawArrow(6, coord.x, coord.y, renderer); } //End left
		else if (path[pathSize].y > path[pathSize - 1].y) { DrawArrow(7, coord.x, coord.y, renderer); } //End down
		else if (path[pathSize].x > path[pathSize - 1].x) { DrawArrow(8, coord.x, coord.y, renderer); } //End right
		if (pathSize > 1) {
			for (int i = 1; i < pathSize; ++i) {
				coord = Coord((path[i].x * 7) + 1, (path[i].y * 7) + 5);
				if (path[i].x == path[i - 1].x && path[i].x == path[i + 1].x) { DrawArrow(9, coord.x, coord.y, renderer); }       //Up-down
				else if (path[i].y == path[i - 1].y && path[i].y == path[i + 1].y) { DrawArrow(10, coord.x, coord.y, renderer); } //Left-right
				else if ((path[i].x > path[i - 1].x && path[i].y > path[i + 1].y) || (path[i].x > path[i + 1].x && path[i].y > path[i - 1].y)) { DrawArrow(11, coord.x, coord.y, renderer); } //Up-left
				else if ((path[i].x > path[i - 1].x && path[i].y < path[i + 1].y) || (path[i].x > path[i + 1].x && path[i].y < path[i - 1].y)) { DrawArrow(12, coord.x, coord.y, renderer); } //Down-left
				else if ((path[i].x < path[i - 1].x && path[i].y < path[i + 1].y) || (path[i].x < path[i + 1].x && path[i].y < path[i - 1].y)) { DrawArrow(13, coord.x, coord.y, renderer); } //Down-right
				else if ((path[i].x < path[i - 1].x && path[i].y > path[i + 1].y) || (path[i].x < path[i + 1].x && path[i].y > path[i - 1].y)) { DrawArrow(14, coord.x, coord.y, renderer); } //Up-right
			}
		}
	}
}

//Draw arrows
void Game::DrawArrow(int type, int x, int y, Renderer& renderer) {
	if (type == 1) { //Start up
		renderer.Render(chars[0], x + 3, y + 1);
		renderer.Render(chars[0], x + 3, y);
	}
	else if (type == 2) { // Start left
		renderer.Render(chars[0], x + 1, y + 3);
		renderer.Render(chars[0], x, y + 3);
	}
	else if (type == 3) { //Start down
		renderer.Render(chars[0], x + 3, y + 5);
		renderer.Render(chars[0], x + 3, y + 6);
	}
	else if (type == 4) { //Start right
		renderer.Render(chars[0], x + 5, y + 3);
		renderer.Render(chars[0], x + 6, y + 3);
	}
	else if (type == 5) { //End up
		renderer.Render(chars[0], x + 3, y + 6);
		renderer.Render(chars[0], x + 2, y + 5);
		renderer.Render(chars[0], x + 3, y + 5);
		renderer.Render(chars[0], x + 4, y + 5);
		renderer.Render(chars[1], x + 2, y + 4);
		renderer.Render(chars[0], x + 3, y + 4);
		renderer.Render(chars[2], x + 4, y + 4);
		renderer.Render(chars[0], x + 3, y + 3);
	}
	else if (type == 6) { //End left
		renderer.Render(chars[0], x + 6, y + 3);
		renderer.Render(chars[0], x + 5, y + 2);
		renderer.Render(chars[0], x + 5, y + 3);
		renderer.Render(chars[0], x + 5, y + 4);
		renderer.Render(chars[3], x + 4, y + 2);
		renderer.Render(chars[0], x + 4, y + 3);
		renderer.Render(chars[4], x + 4, y + 4);
		renderer.Render(chars[0], x + 3, y + 3);
	}
	else if (type == 7) { //End down
		renderer.Render(chars[0], x + 3, y);
		renderer.Render(chars[0], x + 2, y + 1);
		renderer.Render(chars[0], x + 3, y + 1);
		renderer.Render(chars[0], x + 4, y + 1);
		renderer.Render(chars[1], x + 2, y + 2);
		renderer.Render(chars[0], x + 3, y + 2);
		renderer.Render(chars[2], x + 4, y + 2);
		renderer.Render(chars[0], x + 3, y + 3);
	}
	else if (type == 8) { //End right
		renderer.Render(chars[0], x, y + 3);
		renderer.Render(chars[0], x + 1, y + 2);
		renderer.Render(chars[0], x + 1, y + 3);
		renderer.Render(chars[0], x + 1, y + 4);
		renderer.Render(chars[3], x + 2, y + 2);
		renderer.Render(chars[0], x + 2, y + 3);
		renderer.Render(chars[4], x + 2, y + 4);
		renderer.Render(chars[0], x + 3, y + 3);
	}
	else if (type == 9) { //Up-down
		renderer.Render(chars[0], x + 3, y);
		renderer.Render(chars[0], x + 3, y + 1);
		renderer.Render(chars[0], x + 3, y + 2);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 3, y + 4);
		renderer.Render(chars[0], x + 3, y + 5);
		renderer.Render(chars[0], x + 3, y + 6);
	}
	else if (type == 10) { //Left-right
		renderer.Render(chars[0], x, y + 3);
		renderer.Render(chars[0], x + 1, y + 3);
		renderer.Render(chars[0], x + 2, y + 3);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 4, y + 3);
		renderer.Render(chars[0], x + 5, y + 3);
		renderer.Render(chars[0], x + 6, y + 3);
	}
	else if (type == 11) { //Up-left
		renderer.Render(chars[0], x, y + 3);
		renderer.Render(chars[0], x + 1, y + 3);
		renderer.Render(chars[0], x + 2, y + 3);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 3, y + 2);
		renderer.Render(chars[0], x + 3, y + 1);
		renderer.Render(chars[0], x + 3, y);
	}
	else if (type == 12) { //Down-left
		renderer.Render(chars[0], x, y + 3);
		renderer.Render(chars[0], x + 1, y + 3);
		renderer.Render(chars[0], x + 2, y + 3);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 3, y + 4);
		renderer.Render(chars[0], x + 3, y + 5);
		renderer.Render(chars[0], x + 3, y + 6);
	}
	else if (type == 13) { //Down-right
		renderer.Render(chars[0], x + 3, y + 6);
		renderer.Render(chars[0], x + 3, y + 5);
		renderer.Render(chars[0], x + 3, y + 4);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 4, y + 3);
		renderer.Render(chars[0], x + 5, y + 3);
		renderer.Render(chars[0], x + 6, y + 3);
	}
	else if (type == 14) { //Up-right
		renderer.Render(chars[0], x + 6, y + 3);
		renderer.Render(chars[0], x + 5, y + 3);
		renderer.Render(chars[0], x + 4, y + 3);
		renderer.Render(chars[0], x + 3, y + 3);
		renderer.Render(chars[0], x + 3, y + 2);
		renderer.Render(chars[0], x + 3, y + 1);
		renderer.Render(chars[0], x + 3, y);
	}
}

//Draw attack swords
void Game::DrawSword(int x, int y, Renderer& renderer) {
	renderer.Render(chars[6], x + 2, y + 2);
	renderer.Render(chars[7], x + 4, y + 2);
	renderer.Render(chars[5], x + 3, y + 3);
	renderer.Render(chars[5], x + 2, y + 4);
	renderer.Render(chars[5], x + 4, y + 4);
}

//Draw forcefield
void Game::DrawForcefield(int x, int y, Renderer& renderer) {
	renderer.Render(chars[7], x + 1, y + 1);
	renderer.Render(chars[9], x + 2, y + 1);
	renderer.Render(chars[6], x + 3, y + 1);
	renderer.Render(chars[8], x + 1, y + 2);
	renderer.Render(chars[8], x + 3, y + 2);
	renderer.Render(chars[6], x + 1, y + 3);
	renderer.Render(chars[9], x + 2, y + 3);
	renderer.Render(chars[7], x + 3, y + 3);
}

#pragma endregion

#pragma region Getters

//Players
Player* Game::GetTurnPlayer() { return &players[turn]; }
Player* Game::GetNonTurnPlayer() { return &players[!turn]; }

//Indirect iterators
std::vector<Aura*> Game::GetAuras() {
	std::vector<Aura*> auraIterator = auras;
	return auraIterator;
}
std::vector<Unit*> Game::GetUnits() {
	std::vector<Unit*> unitIterator = units;
	return unitIterator;
}
std::vector<Tile*> Game::GetTiles() {
	std::vector<Tile*> tileIterator = tiles;
	return tileIterator;
}
std::vector<Spell*> Game::GetDelayedTriggers() {
	std::vector<Spell*> spellIterator = delayedTriggers;
	return spellIterator;
}

//Unit list
const std::vector<Card*>& Game::GetGrave() const { return grave; }
const std::vector<Unit*>& Game::GetDestroyedUnits() const { return destroyedUnits; }
const std::vector<Spell*>& Game::GetSpellHistory() const { return spellHistory; }
const std::vector<PlayableCard*>& Game::GetCardsCastThisTurn() const { return castThisTurn; }

//Util getters
const int& Game::GetTurnCount() const { return turnCount; }
BoardTile* Game::GetTileInFront(BoardTile* tile, Player* player) {
	return map.GetTile(&players[0] == player ? tile->pos.x + 1 : tile->pos.x - 1, tile->pos.y); }

#pragma endregion

#pragma region Utils

//Check if tiles is moveable
bool Game::CanMove(int x, int y) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		if (map.tiles[x][y].unit == nullptr)
			return true;
		else if (map.tiles[x][y].unit->owner == map.tiles[pos.x][pos.y].unit->owner)
			return true;
	}
	return false;
}

//Check if any cards matching predicate were cast this turn
bool Game::AnyCastThisTurn(bool (*Predicate)(PlayableCard*)) {
	if (!Predicate)
		return true;
	for (PlayableCard* card : castThisTurn)
		if (Predicate(card))
			return true;
	return false;
}

//Get string for player objects
std::string Game::GetPlayerString(Player* player) {
	if (player == &players[false])
		return "`P1`";
	if (player == &players[true])
		return "~P2~";
	return "P?";
}

//Add formatting markers to text owned by a player
std::string Game::GetCardString(Card* card) {
	if (card->owner == &players[false])
		return '`' + card->name + '`';
	if (card->owner == &players[true])
		return '~' + card->name + '~';
	return card->name;
}

//Log string
void Game::Log(std::string log) {
	for (int i = 0; i < log.length(); ++i)
		if (log[i] == '|')
			log[i] = ' ';
	debugLog.push_back(log);
	while (debugLog.size() > 46)
		debugLog.erase(debugLog.begin());
}

#pragma endregion