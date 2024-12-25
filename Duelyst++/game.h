//Defines
#ifndef __GAME_H__
#define __GAME_H__

//Include
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include "eventmanager.h"

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE,
	MODE_HAND,
	MODE_SELECT
};

//Minion effect callback class
class EffectCallback {
public:
	EffectCallback();
	EffectCallback(Minion* _minion, Spell* _spell, BoardTile* _tile, eEffect _effect);
	~EffectCallback();
	Minion* minion;
	Spell* spell;
	BoardTile* tile;
	eEffect effect;
};

//Pathing coords
class PathCoord {
public:
	PathCoord();
	PathCoord(Coord _pos, int _last, int _count);
	~PathCoord();
	Coord pos;
	int last;
	int count;
};

//Game class
class Game {
public:
	Game();
	~Game();
	void Input();
	void Update();
	void RenderGame(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);
	void ChangeTurn(bool _turn);
	void SetContext(Card* card, Player* player);
	void Summon(Card* card, int x, int y, bool actionBar);
	void UseCard();
	void UseEffect();
	void SelectTile(BoardTile& tile);
	void SelectCard();
	void AttackUnit();
	void MoveUnit();
	void MoveCursor(int x, int y);
	void MoveCursorHand(int x, int y);
	void MoveSelect(int x, int y);
	bool CanMove(int x, int y);
	void HighlightTile(int x, int y, eColor color);
	void HighlightMoveable(int x, int y);
	void SearchMoveable(int x, int y, int range);
	void HighlightSelectable(eTarget targetMode);
	void HighlightSelectable(eTarget targetMode, Minion* minion);
	void CreatePath();
	bool AddToPaths(int x, int y, int last, int count);
	void GeneratePaths();
	void DrawPath(Renderer& renderer);
	void DrawSword(int x, int y, Renderer& renderer);
	void DrawArrow(int type, int x, int y, Renderer& renderer);
	std::vector<Card*> grave;
	std::vector<Minion*> minions;
	Sprite light;
	Sprite board;
	Sprite chars[10];
	CardList cardList;
	EventManager eventManager;
	Player players[2];
	Map map;
	Tile hand[7];
	EffectCallback callback;
	std::vector<EffectCallback> lateCallback;
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<BoardTile*> selectable;
	std::vector<PathCoord> pathList;
	std::vector<Coord> path;
	Minion* activeUnit;
	Card* activeCard;
	eMode mode;
	Coord pos;
	int handIdx;
	int selectionIdx;
	bool turn;
	bool endTurn;
	int turnCount;
	bool* modeSwitch;
};

#endif