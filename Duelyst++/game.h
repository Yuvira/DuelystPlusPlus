#pragma once

//Include
#include <vector>
#include "cardids.h"
#include "effect.h"
#include "tile.h"
#include "player.h"
#include "eventmanager.h"
#include "sprite.h"

//Define
class Collections;
class Card;
class Spell;
class Unit;
struct TargetMode;

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
	EffectCallback(EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, BoardTile* target));
	~EffectCallback();
	void Execute(BoardTile* target);
	EffectContext context;
	BoardTile* source;
	void (*Callback)(EffectContext ctx, BoardTile* source, BoardTile* target);
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

	//Events
	void QueueEvent(Event* event);
	void ProcessEventStack();

	//Aura effects
	void ApplyAura(eEffect effectId, Card* sourceCard, Effect* sourceEffect);
	void RemoveAura(Effect* effect);
	void RemoveAurasFromSource(Effect* sourceEffect);
	void RemoveAurasAt(std::vector<int> indices);
	bool IsTrackingAura(Effect* aura);

	//Actions
	void UseCard();
	void UseEffect();
	void PostCast();
	void MoveUnit();
	void AttackUnit();
	void Summon(Card* card, BoardTile* tile, bool actionBar);
	void SummonToken(eCard cardId, BoardTile* tile, Player* owner);
	void Replace();
	void ChangeTurn(bool newTurn);
	void Overflow(Card* card);

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
	void HighlightTargetsForCallback(std::vector<BoardTile*>& targets, EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, BoardTile* target));

	//Pathfinding
	void CreatePath();
	bool AddToPaths(int x, int y, int last, int count);
	void GeneratePaths();

	//Draw
	void DrawPath(Renderer& renderer);
	void DrawArrow(int type, int x, int y, Renderer& renderer);
	void DrawSword(int x, int y, Renderer& renderer);
	void DrawForcefield(int x, int y, Renderer& renderer);

	//Getters
	Player* GetTurnPlayer();
	Player* GetNonTurnPlayer();
	std::vector<Effect*> GetAuras();
	std::vector<Unit*> GetUnits();
	const std::vector<Card*>& GetGrave() const;
	const std::vector<Unit*>& GetDestroyedUnits() const;
	const std::vector<Spell*>& GetSpellHistory() const;
	const std::vector<Card*>& GetCardsCastThisTurn() const;
	const int& GetTurnCount() const;
	BoardTile* GetTileInFront(BoardTile* tile, Player* player);

	//Utils
	bool CanMove(int x, int y);
	bool AnyCastThisTurn(bool (*Predicate)(Card*));
	std::string GetPlayerString(Player* player);
	std::string GetCardString(Card* card);
	void Log(std::string log);

	//Gameplay objects
	Collections* collections;
	EventManager eventManager;
	Map map;
	Tile hand[7];

private:

	//Gameplay properties
	eMode mode;
	Player players[2];
	std::vector<Event*> eventStack;
	std::vector<Effect*> auras;
	EffectCallback effectCallback;
	bool* modeSwitch;

	//Turn tracking
	bool turn;
	int turnCount;
	bool endTurn;

	//Selection tracking
	Coord pos;
	Coord castPos;
	int handIdx;
	int selectionIdx;
	Unit* activeUnit;
	Card* activeCard;

	//Pathfinding
	std::vector<Coord> path;
	std::vector<PathCoord> pathList;

	//Selection objects
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<BoardTile*> selectable;

	//Card object references
	std::vector<Unit*> units;
	std::vector<Card*> grave;
	std::vector<Unit*> destroyedUnits;
	std::vector<Spell*> spellHistory;
	std::vector<Card*> castThisTurn;

	//Sprites
	Sprite light;
	Sprite board;
	Sprite chars[10];

	//Debug
	bool debugMode;
	Sprite debugP1Deck;
	Sprite debugP2Deck;
	Sprite debugUnits;
	Sprite debugGrave;
	Sprite debugAuras;
	std::vector<std::string> debugLog;
	Sprite debugLogSprite;

};