//Include
#include "game.h"

#pragma region Helper Constructors

//Callback stuff
EffectCallback::EffectCallback() : EffectCallback(nullptr, nullptr) {}
EffectCallback::EffectCallback(Card* _card, BoardTile* _tile) {
	card = _card;
	tile = _tile;
	callback = nullptr;
}
EffectCallback::~EffectCallback() {}

//Path co-ordinate constructor
PathCoord::PathCoord() : PathCoord(Coord(), 0, 0) {}
PathCoord::PathCoord(Coord _pos, int _last, int _count) {
	pos = _pos;
	last = _last;
	count = _count;
}
PathCoord::~PathCoord() {}

#pragma endregion

#pragma region Game Constructor

//Game constructor
Game::Game(Collections* _collections) {

	//Collections reference
	collections = _collections;

	//Board border
	board.CreateFromFile("resources/board.txt");

	//Event manager context
	eventManager.game = this;

	//Character sprites
	char c[] = { 'Û', 'Þ', 'Ý', 'Ü', 'ß', 'X', '\\', '/', '³', 'Ä', '®', 'é', '¯' };
	for (int i = 0; i < 10; ++i) { chars[i].buffer[0].Char.AsciiChar = c[i]; }

	//Turn indicator
	light.Resize(3, 1);
	for (int i = 0; i < 3; ++i) { light.buffer[i].Char.AsciiChar = c[i + 10]; }
	light.SetColor(COLOR_LTBLUE);

	//Set opponents
	players[0].opponent = &players[1];
	players[1].opponent = &players[0];

	//Initialize players
	for (int i = 0; i < 2; ++i) {
		players[i].Preset(collections, this);
		Summon(players[i].deck[0], i * 8, 2, false);
		players[i].deck.erase(players[i].deck.begin());
		players[i].general = minions.back();
		players[i].Init(i + 2);
	}

	//Hand
	for (int i = 0; i < 7; ++i) {
		hand[i].border.pos.X = (i * 7) + 1;
		hand[i].border.pos.Y = 41;
	}

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
	renderer.Render(board);

	//Turn indicator
	renderer.Render(light, 31, 2);

	//Tiles
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 5; ++j) {
			renderer.Render(map.tiles[i][j].border);
			renderer.Render(map.tiles[i][j].sprite);
		}
	}

	//Units
	for (int i = 0; i < minions.size(); ++i)
		minions[i]->Render(renderer);

	//Render active minion card while selecting opening gambit targets
	if (activeCard != nullptr && activeCard->GetMinion() != nullptr && mode == MODE_SELECT && callback.callback)
		activeCard->GetMinion()->Render(renderer);

	//Move mode indicators
	if (mode == MODE_MOVE) {
		for (int i = 0; i < attackable.size(); ++i) {
			if (attackable[i] == selectable[selectionIdx]) {
				COORD coord = selectable[selectionIdx]->border.pos;
				DrawSword(coord.X, coord.Y, renderer);
				coord = activeUnit->curTile->border.pos;
				if (selectable[selectionIdx]->minion->CanAttack(activeUnit))
					DrawSword(coord.X, coord.Y, renderer);
				goto end;
			}
		}
		DrawPath(renderer);
	end:;
	}

	//Hand
	for (int i = 0; i < 7; ++i)
		renderer.Render(hand[i].border);
	for (int i = 0; i < players[turn].hand.size(); ++i) {
		players[turn].hand[i]->sprite.SetColor(COLOR_LTWHITE);
		renderer.Render(players[turn].hand[i]->sprite, (i * 7) + 9, 42);
	}

	//Player UI
	players[0].Render(renderer, true);
	players[1].Render(renderer, false);

	//Sidebar
	RenderSidebar(renderer);

}

//Render sidebar
void Game::RenderSidebar(Renderer& renderer) {

	//Vertical position
	int y = 1;

	//Minion on board
	if (mode == MODE_NONE) {
		if (map.tiles[pos.x][pos.y].minion != nullptr)
			map.tiles[pos.x][pos.y].minion->DrawDetails(renderer, y);
	}

	//Minion in hand
	else if (mode == MODE_HAND || mode == MODE_SELECT) {
		if (handIdx < players[turn].hand.size())
			players[turn].hand[handIdx]->DrawDetails(renderer, y);
	}

}

#pragma endregion

#pragma region Input

//Input
void Game::Input() {

	//Get keyPress
	int asciiVal = _getch();

	//Standard mode
	if (mode == MODE_NONE) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { MoveCursor(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveCursor(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveCursor(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveCursor(1, 0); } //D

		//Select minion
		else if (asciiVal == 32) {
			if (map.tiles[pos.x][pos.y].minion != nullptr)
				if (map.tiles[pos.x][pos.y].minion->owner == &players[turn])
					SelectTile(map.tiles[pos.x][pos.y]);
		}

		//Change turn
		else if (asciiVal == 10 || asciiVal == 13)
			ChangeTurn(!turn);

	}

	//Hand mode
	else if (mode == MODE_HAND) {

		//Move pointer
		if (asciiVal == 119 || asciiVal == 87) { MoveCursorHand(0, 4); }       //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveCursorHand(-1, -1); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveCursorHand(0, 0); }  //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveCursorHand(1, -1); } //D

		//Replace card
		else if (asciiVal == 114 || asciiVal == 82)
			players[turn].Replace(handIdx);

		//Select card
		else if (asciiVal == 32)
			if (handIdx < players[turn].hand.size())
				SelectCard();

	}

	//Move mode
	else if (mode == MODE_MOVE) {

		//Move arrow
		if (asciiVal == 119 || asciiVal == 87) { MoveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveSelect(1, 0); } //D

		//Move minion
		else if (asciiVal == 32) {
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

	//Select mode
	else if (mode == MODE_SELECT) {

		//Move selection
		if (asciiVal == 119 || asciiVal == 87) { MoveSelect(0, -1); }     //W
		else if (asciiVal == 97 || asciiVal == 65) { MoveSelect(-1, 0); } //A
		else if (asciiVal == 115 || asciiVal == 83) { MoveSelect(0, 1); } //S
		else if (asciiVal == 100 || asciiVal == 68) { MoveSelect(1, 0); } //D

		//Use card at selection
		else if (asciiVal == 32) {
			if (callback.callback)
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

	//Update units
	bool reloop;
	do { 
		reloop = false;
		for (int a = 0; a < minions.size(); ++a)
			if (!minions[a]->isDead)
				minions[a]->Update(reloop);
	} while (reloop);
	for (int i = 0; i < minions.size(); ++i) {
		if (minions[i]->isDead) {
			if (minions[i] == minions[i]->curTile->minion)
				minions[i]->curTile->minion = nullptr;
			minions[i]->curTile = nullptr;
			grave.push_back(minions[i]);
			minions.erase(minions.begin() + i);
			--i;
		}
		else
			minions[i]->UpdateStatSprites();
	}

	//Update mana bars
	players[0].UpdateMana(COLOR_LTBLUE);
	players[1].UpdateMana(COLOR_RED);

	//Clear tiles highlights
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 5; ++j)
			map.tiles[i][j].SetColor(COLOR_LTWHITE);

	//Re-highlight hostiles
	hostile.clear();
	for (int i = 0; i < minions.size(); ++i)
		if (minions[i]->owner != &players[turn])
			hostile.push_back(minions[i]->curTile);
	for (int i = 0; i < hostile.size(); ++i)
		hostile[i]->SetColor(COLOR_LTRED);

	//Highlight moveable spaces and create move path
	if (mode == MODE_MOVE) {
		GeneratePaths();
		for (int i = 0; i < highlighted.size(); ++i)
			highlighted[i]->SetColor(COLOR_AQUA);
		map.tiles[pos.x][pos.y].SetColor(COLOR_LTWHITE);
		for (int i = 0; i < attackable.size(); ++i)
			attackable[i]->SetColor(COLOR_RED);
	}

	//Highlight selectable spaces
	else if (mode == MODE_SELECT) {
		for (int i = 0; i < selectable.size(); ++i)
			selectable[i]->SetColor(COLOR_GREEN);
		selectable[selectionIdx]->SetColor(COLOR_LTGREEN);
	}

	//Highlight cursor position
	else if (mode == MODE_NONE) {
		highlighted.clear();
		HighlightTile(pos.x, pos.y, COLOR_AQUA);
	}

}

#pragma endregion

#pragma region Actions

//Use active card
void Game::UseCard() {

	//Use mana
	players[turn].mana -= activeCard->cost;

	//Set position and clear selectables
	pos = selectable[selectionIdx]->pos;
	hand[handIdx + 1].SetColor(COLOR_LTWHITE);
	selectable.clear();
	selectionIdx = -1;
	mode = MODE_NONE;

	//Remove from hand and do pre-cast effect targeting
	players[turn].hand.erase(players[turn].hand.begin() + handIdx);
	activeCard->PreCast(&map.tiles[pos.x][pos.y]);
	if (activeCard->GetMinion() != nullptr)
		activeCard->GetMinion()->SetPosition(pos.x, pos.y);
	castPos = pos;

	//Reset hand position
	handIdx = -1;

	//Do post-cast if we're not targeting anything
	if (mode == MODE_NONE)
		PostCast();

}

//Use active effect
void Game::UseEffect() {
	BoardTile* tile = selectable[selectionIdx];
	pos = tile->pos;
	selectable.clear();
	selectionIdx = -1;
	mode = MODE_NONE;
	if (callback.callback) {
		callback.Execute(tile);
		callback = EffectCallback();
	}
	if (activeCard != nullptr)
		PostCast();
}

//Send cast event after pre-cast effect targets have been set
void Game::PostCast() {
	eventManager.SendOnCast(activeCard, &map.tiles[castPos.x][castPos.y]);
	activeCard->Resolve(&map.tiles[castPos.x][castPos.y]);
	if (activeCard->cardType == CARD_SPELL)
		spellHistory.push_back(activeCard->original->GetSpell());
	activeCard = nullptr;
}

//Summon at tile / position
void Game::Summon(Card* card, BoardTile* tile, bool actionBar) { Summon(card, tile->pos.x, tile->pos.y, actionBar); }
void Game::Summon(Card* card, int x, int y, bool actionBar) {
	if (!card->IsMinion())
		return;
	minions.push_back(card->GetMinion());
	minions.back()->SetPosition(x, y);
	eventManager.SendOnSummon(minions.back(), actionBar);
}

//Move selected minion
void Game::MoveUnit() {
	if (selectable[selectionIdx]->minion == nullptr || selectable[selectionIdx] == &map.tiles[pos.x][pos.y]) {
		if (selectable[selectionIdx] != &map.tiles[pos.x][pos.y]) {
			activeUnit->SetPosition(selectable[selectionIdx]->pos.x, selectable[selectionIdx]->pos.y);
			if (!activeUnit->hasCelerityMoved) { activeUnit->hasCelerityMoved = true; }
			else { activeUnit->hasMoved = true; }
			pos = selectable[selectionIdx]->pos;
			eventManager.SendOnMove(activeUnit, false);
		}
		moveable.clear();
		attackable.clear();
		selectable.clear();
		activeUnit = nullptr;
		mode = MODE_NONE;
	}
}

//Attack minion
void Game::AttackUnit() {
	activeUnit->Attack(selectable[selectionIdx]->minion, false);
	pos = selectable[selectionIdx]->pos;
	moveable.clear();
	attackable.clear();
	selectable.clear();
	activeUnit = nullptr;
	mode = MODE_NONE;
}

//Change turn
void Game::ChangeTurn(bool _turn) {

	//Indicate turn is ending
	endTurn = true;

	//Process end of turn events
	while (lateCallbacks.size() > 0) {
		if (lateCallbacks[0].callback)
			lateCallbacks[0].Execute();
		lateCallbacks.erase(lateCallbacks.begin());
		if (selectable.size() > 0)
			return;
	}

	//Draw and reset replaces
	if (turnCount > 0)
		players[turn].Draw();
	players[turn].replaces = 1;

	//Change turn
	turn = _turn;
	if (!turn)
		++turnCount;

	//Refresh mana
	if (turnCount > 1 && players[turn].manaMax < 9)
		++players[turn].manaMax;
	players[turn].mana = players[turn].manaMax;

	//Set turn indicator
	if (turn)
		light.SetColor(COLOR_RED);
	else
		light.SetColor(COLOR_LTBLUE);

	//End old turn, start new turn
	eventManager.SendOnTurnEnd(&players[!turn]);
	eventManager.SendOnTurnStart(&players[turn]);

	//Turn has ended
	endTurn = false;

}

#pragma endregion

#pragma region Selection / Movement

//Select minion
void Game::SelectTile(BoardTile& tile) {
	for (int i = 0; i < highlighted.size(); ++i)
		moveable.push_back(highlighted[i]);
	if (!tile.minion->hasAttacked) {
		bool provoked = false;
		for (int i = max(tile.pos.x - 1, 0); i < min(tile.pos.x + 2, 9); ++i)
			for (int j = max(tile.pos.y - 1, 0); j < min(tile.pos.y + 2, 5); ++j)
				if (map.tiles[i][j].minion != nullptr)
					if (map.tiles[i][j].minion->owner != &players[turn])
						if (map.tiles[i][j].minion->HasKeywords(KEYWORD_PROVOKE))
							if (tile.minion->CanAttack(map.tiles[i][j].minion)) {
								attackable.push_back(&map.tiles[i][j]);
								provoked = true;
							}
		if (!provoked)
			for (int a = 0; a < hostile.size(); ++a)
				if (tile.minion->CanAttack(hostile[a]->minion))
					attackable.push_back(hostile[a]);
	}
	if (moveable.size() > 1 || attackable.size() > 0) {
		selectable.insert(selectable.end(), moveable.begin(), moveable.end());
		selectable.insert(selectable.end(), attackable.begin(), attackable.end());
		activeUnit = tile.minion;
		selectionIdx = 0;
		mode = MODE_MOVE;
	}
	else
		moveable.clear();
}

//Select card in hand
void Game::SelectCard() {
	if (players[turn].hand[handIdx]->cost <= players[turn].mana) {
		HighlightSelectable(players[turn].hand[handIdx]->targetMode);
		if (selectable.size() > 0) { activeCard = players[turn].hand[handIdx]; }
	}
}

//Move cursor position
void Game::MoveCursor(int x, int y) {

	//Move cursor
	pos.x = (pos.x + x + 9) % 9;
	if ((y == -1 && pos.y == 0) || (y == 1 && pos.y == 4)) {
		handIdx = max(min(pos.x - 1, 5), 0);
		hand[handIdx + 1].SetColor(COLOR_AQUA);
		pos = Coord(-1, -1);
		mode = MODE_HAND;
	}
	else
		pos.y = (pos.y + y + 5) % 5;

}

//Move cursor position
void Game::MoveCursorHand(int x, int y) {

	//Deselect tiles
	hand[handIdx + 1].SetColor(COLOR_LTWHITE);

	//Move cursor
	handIdx = (handIdx + x + 6) % 6;
	if (y != -1) {
		pos = Coord(handIdx + 1, y);
		handIdx = -1;
		mode = MODE_NONE;
		return;
	}

	//Select new tiles
	hand[handIdx + 1].SetColor(COLOR_AQUA);

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
	map.tiles[x][y].SetColor(color);
	highlighted.push_back(&map.tiles[x][y]);
	if (color == COLOR_AQUA && mode == MODE_NONE)
		if (map.tiles[pos.x][pos.y].minion != nullptr)
			if (!map.tiles[pos.x][pos.y].minion->hasMoved && map.tiles[pos.x][pos.y].minion->IsMoveable())
				if (map.tiles[pos.x][pos.y].minion->owner == &players[turn])
					if (!map.tiles[pos.x][pos.y].minion->IsProvoked())
						HighlightMoveable(pos.x, pos.y);
}

//Highlight moveable spaces
void Game::HighlightMoveable(int x, int y) {
	if (map.tiles[pos.x][pos.y].minion->HasKeywords(KEYWORD_FLYING)) {
		for (int i = 0; i < 9; ++i)
			for (int j = 0; j < 5; ++j)
				if (i != pos.x || j != pos.y)
					if (CanMove(i, j))
						HighlightTile(i, j, COLOR_GRAY);
	}
	else
		SearchMoveable(pos.x, pos.y, map.tiles[pos.x][pos.y].minion->MoveRange());
	map.tiles[x][y].SetColor(COLOR_AQUA);
}

//Recursively search for moveable spaces
void Game::SearchMoveable(int x, int y, int range) {
	if (range == 0) { return; }
	if (CanMove(x, y - 1)) { HighlightTile(x, y - 1, COLOR_GRAY); SearchMoveable(x, y - 1, range - 1); } //Up
	if (CanMove(x - 1, y)) { HighlightTile(x - 1, y, COLOR_GRAY); SearchMoveable(x - 1, y, range - 1); } //Left
	if (CanMove(x, y + 1)) { HighlightTile(x, y + 1, COLOR_GRAY); SearchMoveable(x, y + 1, range - 1); } //Down
	if (CanMove(x + 1, y)) { HighlightTile(x + 1, y, COLOR_GRAY); SearchMoveable(x + 1, y, range - 1); } //Right
}

//Highlight targetable tiles
void Game::HighlightSelectable(eTarget targetMode) { HighlightSelectable(targetMode, nullptr); }
void Game::HighlightSelectable(eTarget targetMode, Minion* minion) {

	//Clear
	selectable.clear();

	//Target type
	switch (targetMode) {

		//Empty tiles
	case TARGET_TILE:
		for (int i = 0; i < 9; ++i)
			for (int j = 0; j < 5; ++j)
				if (map.tiles[i][j].minion == nullptr)
					selectable.push_back(&map.tiles[i][j]);
		break;

		//Any minion
	case TARGET_UNIT:
		for (int i = 0; i < minions.size(); ++i)
			selectable.push_back(minions[i]->curTile);
		break;

		//Any (non-general) minion
	case TARGET_MINION:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->tribe != TRIBE_GENERAL)
				selectable.push_back(minions[i]->curTile);
		break;

		//Enemies
	case TARGET_ENEMY:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->owner != &players[turn])
				selectable.push_back(minions[i]->curTile);
		break;

		//Allied minions
	case TARGET_ALLY_MINON:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->owner == &players[turn])
				if (minions[i]->tribe != TRIBE_GENERAL)
					selectable.push_back(minions[i]->curTile);
		break;

		//Enemy minions
	case TARGET_ENEMY_MINION:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->owner != &players[turn])
				if (minions[i]->tribe != TRIBE_GENERAL)
					selectable.push_back(minions[i]->curTile);
		break;

		//Enemy minions
	case TARGET_ENEMY_RANGED:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->owner != &players[turn])
				if (minions[i]->tribe != TRIBE_GENERAL)
					if (minions[i]->HasKeywords(KEYWORD_RANGED))
						selectable.push_back(minions[i]->curTile);
		break;

		//Enemy general
	case TARGET_ENEMY_GENERAL:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i] == players[!turn].general)
				selectable.push_back(minions[i]->curTile);
		break;

		//Spaces near minion
	case TARGET_NEAR_UNIT:
		if (minion != nullptr)
			for (int i = max(minion->curTile->pos.x - 1, 0); i < min(minion->curTile->pos.x + 2, 9); ++i)
				for (int j = max(minion->curTile->pos.y - 1, 0); j < min(minion->curTile->pos.y + 2, 5); ++j)
					if (i != minion->curTile->pos.x || j != minion->curTile->pos.y)
						selectable.push_back(&map.tiles[i][j]);
		break;

		//Minons near minion
	case TARGET_MINION_NEAR_UNIT:
		if (minion != nullptr)
			for (int i = max(minion->curTile->pos.x - 1, 0); i < min(minion->curTile->pos.x + 2, 9); ++i)
				for (int j = max(minion->curTile->pos.y - 1, 0); j < min(minion->curTile->pos.y + 2, 5); ++j)
					if (map.tiles[i][j].minion != nullptr && map.tiles[i][j].minion != minion && map.tiles[i][j].minion->tribe != TRIBE_GENERAL)
						selectable.push_back(&map.tiles[i][j]);
		break;

		//Empty tiles near minion
	case TARGET_TILE_NEAR_UNIT:
		if (minion != nullptr)
			for (int i = max(minion->curTile->pos.x - 1, 0); i < min(minion->curTile->pos.x + 2, 9); ++i)
				for (int j = max(minion->curTile->pos.y - 1, 0); j < min(minion->curTile->pos.y + 2, 5); ++j)
					if (map.tiles[i][j].minion == nullptr)
						selectable.push_back(&map.tiles[i][j]);
		break;

		//Near allies (summon)
	case TARGET_NEAR_ALLY:
		for (int i = 0; i < minions.size(); ++i)
			if (minions[i]->owner == &players[turn])
				for (int j = max(minions[i]->curTile->pos.x - 1, 0); j < min(minions[i]->curTile->pos.x + 2, 9); ++j)
					for (int k = max(minions[i]->curTile->pos.y - 1, 0); k < min(minions[i]->curTile->pos.y + 2, 5); ++k)
						if (map.tiles[j][k].minion == nullptr)
							if (map.tiles[j][k].border.buffer[0].Attributes != COLOR_GREEN)
								selectable.push_back(&map.tiles[j][k]);
		break;

	}

	//If selectable tiles found, highlight first and switch to select mode
	if (selectable.size() > 0) {
		selectionIdx = 0;
		mode = MODE_SELECT;
	}

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
		if (map.tiles[pos.x][pos.y].minion->HasKeywords(KEYWORD_FLYING)) {
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

#pragma region Path Rendering

//Draw arrow path
void Game::DrawPath(Renderer& renderer) {
	int pathSize = path.size() - 1;
	if (pathSize > 0) {
		COORD coord = map.tiles[path[0].x][path[0].y].border.pos;
		if (path[0].y > path[1].y) { DrawArrow(1, coord.X, coord.Y, renderer); }      //Start up
		else if (path[0].x > path[1].x) { DrawArrow(2, coord.X, coord.Y, renderer); } //Start left
		else if (path[0].y < path[1].y) { DrawArrow(3, coord.X, coord.Y, renderer); } //Start down
		else if (path[0].x < path[1].x) { DrawArrow(4, coord.X, coord.Y, renderer); } //Start right
		coord = map.tiles[path[pathSize].x][path[pathSize].y].border.pos;
		if (path[pathSize].y < path[pathSize - 1].y) { DrawArrow(5, coord.X, coord.Y, renderer); }      //End up
		else if (path[pathSize].x < path[pathSize - 1].x) { DrawArrow(6, coord.X, coord.Y, renderer); } //End left
		else if (path[pathSize].y > path[pathSize - 1].y) { DrawArrow(7, coord.X, coord.Y, renderer); } //End down
		else if (path[pathSize].x > path[pathSize - 1].x) { DrawArrow(8, coord.X, coord.Y, renderer); } //End right
		if (pathSize > 1) {
			for (int i = 1; i < pathSize; ++i) {
				coord = map.tiles[path[i].x][path[i].y].border.pos;
				if (path[i].x == path[i - 1].x && path[i].x == path[i + 1].x) { DrawArrow(9, coord.X, coord.Y, renderer); }       //Up-down
				else if (path[i].y == path[i - 1].y && path[i].y == path[i + 1].y) { DrawArrow(10, coord.X, coord.Y, renderer); } //Left-right
				else if ((path[i].x > path[i - 1].x && path[i].y > path[i + 1].y) || (path[i].x > path[i + 1].x && path[i].y > path[i - 1].y)) { DrawArrow(11, coord.X, coord.Y, renderer); } //Up-left
				else if ((path[i].x > path[i - 1].x && path[i].y < path[i + 1].y) || (path[i].x > path[i + 1].x && path[i].y < path[i - 1].y)) { DrawArrow(12, coord.X, coord.Y, renderer); } //Down-left
				else if ((path[i].x < path[i - 1].x && path[i].y < path[i + 1].y) || (path[i].x < path[i + 1].x && path[i].y < path[i - 1].y)) { DrawArrow(13, coord.X, coord.Y, renderer); } //Down-right
				else if ((path[i].x < path[i - 1].x && path[i].y > path[i + 1].y) || (path[i].x < path[i + 1].x && path[i].y > path[i - 1].y)) { DrawArrow(14, coord.X, coord.Y, renderer); } //Up-right
			}
		}
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

#pragma endregion

#pragma region Utils

//Set game context for new token cards
void Game::SetContext(Card* card, Player* player) {
	card->game = this;
	card->owner = player;
}

//Check if tiles is moveable
bool Game::CanMove(int x, int y) {
	if (x > -1 && x < 9 && y > -1 && y < 5) {
		if (map.tiles[x][y].minion == nullptr)
			return true;
		else if (map.tiles[x][y].minion->owner == map.tiles[pos.x][pos.y].minion->owner)
			return true;
	}
	return false;
}

#pragma endregion