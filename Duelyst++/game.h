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

//Methods
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
	void ApplyAura(eEffect effectId, Effect* source, Card* refCard);
	void ApplyAura(Effect effect, Effect* source, Card* refCard);
	void RemoveAura(Effect* effect);
	void RemoveAurasFromSource(Effect* source);
	void RemoveAurasAt(std::vector<int> indices);
	bool IsTrackingAura(Effect* aura);

	//Actions
	void UseCard();
	void UseEffect();
	void PostCast();
	void MoveUnit();
	void AttackUnit();
	void Replace();
	void ChangeTurn(bool newTurn);
	void Summon(Card* card, BoardTile* tile, bool actionBar);
	void Summon(Card* card, int x, int y, bool actionBar);
	void SummonToken(eCard cardId, BoardTile* tile, Player* owner);
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
	void SetContext(Card* card, Player* player);
	bool CanMove(int x, int y);
	bool HasSelectable();
	bool AnyCastThisTurn(bool (*Predicate)(Card*));
	std::string GetPlayerString(Player* player);
	std::string GetCardString(Card* card);
	void Log(std::string log);

	//Gameplay objects
	Collections* collections;
	EventManager eventManager;
	Player players[2];
	Map map;
	Tile hand[7];
	EffectCallback callback;

//Properties
private:

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

	//Gameplay objects
	std::vector<Event*> eventStack;
	std::vector<Effect*> auras;

	//Selection objects
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<BoardTile*> selectable;

	//Gameplay properties
	std::vector<PathCoord> pathList;
	std::vector<Coord> path;
	Unit* activeUnit;
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
	Sprite debugUnits;
	Sprite debugGrave;
	std::vector<std::string> debugLog;
	Sprite debugLogSprite;

};