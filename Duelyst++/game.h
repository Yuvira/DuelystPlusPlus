#pragma once

//Include
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include "eventmanager.h"

//Debug mode
inline constexpr bool IS_DEBUG = false;

#pragma region Enums / Helpers

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE,
	MODE_HAND,
	MODE_SELECT
};

//Card effect callback class
class EffectCallback {
public:
	EffectCallback();
	EffectCallback(EffectContext context, BoardTile* source);
	~EffectCallback();
	void Execute() { if (Callback) Callback(context, source, nullptr); }
	void Execute(BoardTile* target) { if (Callback) Callback(context, source, target); }
	EffectContext context;
	BoardTile* source;
	void (*Callback)(EffectContext, BoardTile* source, BoardTile* target);
};

//Pathing coords
class PathCoord {
public:
	PathCoord();
	PathCoord(Coord pos, int last, int count);
	~PathCoord();
	Coord pos;
	int last;
	int count;
};

#pragma endregion

//Game class
class Game {
public:
	
	//Constructor
	Game(Collections* collections, bool* modeSwitch);
	~Game();

	//Rendering
	void RenderGame(Renderer& renderer);
	void RenderSidebar(Renderer& renderer);
	void RenderDebug(Renderer& renderer);

	//Input & Updates
	void Input();
	void Update();

	//Actions
	void UseCard();
	void UseEffect();
	void PostCast();
	void Summon(Card* card, BoardTile* tile, bool actionBar);
	void Summon(Card* card, int x, int y, bool actionBar);
	void SummonToken(eCard cardId, BoardTile* tile, Player* owner);
	void MoveUnit();
	void AttackUnit();
	void ChangeTurn(bool _turn);

	//Selection & Movement
	void SelectTile(BoardTile& tile);
	void SelectCard();
	void MoveCursor(int x, int y);
	void MoveCursorHand(int x, int y);
	void MoveSelect(int x, int y);

	//Highlights
	void HighlightTile(int x, int y, eColor color);
	void HighlightMoveable(int x, int y);
	void SearchMoveable(int x, int y, int range);
	void HighlightSelectable(TargetMode targetMode);
	void HighlightSelectable(TargetMode targetMode, BoardTile* tile);

	//Pathfinding
	void CreatePath();
	bool AddToPaths(int x, int y, int last, int count);
	void GeneratePaths();

	//Draw
	void DrawPath(Renderer& renderer);
	void DrawSword(int x, int y, Renderer& renderer);
	void DrawArrow(int type, int x, int y, Renderer& renderer);

	//Utils
	void SetContext(Card* card, Player* player);
	bool CanMove(int x, int y);
	std::string GetPlayerString(Player* player);
	std::string GetCardString(Card* card);
	void Log(std::string log);

	//Card object references
	std::vector<Minion*> minions;
	std::vector<Card*> grave;
	std::vector<Minion*> destroyedMinions;
	std::vector<Spell*> spellHistory;
	std::vector<Card*> castThisTurn;

	//Sprites
	Sprite light;
	Sprite board;
	Sprite chars[10];

	//Gameplay objects
	Collections* collections;
	EventManager eventManager;
	Player players[2];
	Map map;
	Tile hand[7];
	EffectCallback callback;

	//Selection objects
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<BoardTile*> selectable;

	//Gameplay properties
	std::vector<PathCoord> pathList;
	std::vector<Coord> path;
	Minion* activeUnit;
	Card* activeCard;
	eMode mode;
	Coord pos;
	Coord castPos;
	int handIdx;
	int selectionIdx;
	bool turn;
	bool endTurn;
	int turnCount;
	bool* modeSwitch;

	//Debug
	bool debugMode;
	Sprite debugP1Deck;
	Sprite debugP2Deck;
	Sprite debugMinions;
	Sprite debugGrave;
	std::vector<std::string> debugLog;
	Sprite debugLogSprite;

};