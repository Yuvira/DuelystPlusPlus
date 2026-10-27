#pragma once

//Include
#include <vector>
#include "boardtile.h"
#include "cardids.h"
#include "effect.h"
#include "player.h"
#include "eventmanager.h"
#include "sprite.h"

//Define
class Collections;
class Card;
class Spell;
class Unit;

//Debug mode
inline constexpr bool IS_DEBUG = false;

#pragma region Enums / Helpers

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE,
	MODE_HAND,
	MODE_ARTIFACT,
	MODE_SELECT
};

//Card effect callback class
class EffectCallback {
public:
	EffectCallback();
	EffectCallback(EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets));
	~EffectCallback();
	void Execute(std::vector<BoardTile*>& targets);
	EffectContext context;
	BoardTile* source;
	void (*Callback)(EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets);
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
	Game(Collections* collections, bool* modeSwitch, bool* resetGame, bool* exitGame);
	~Game();

	//Rendering
	void RenderGame(Renderer& renderer);
	void RenderTile(Renderer& renderer, Coord xy);
	void RenderTile(Renderer& renderer, int x, int y);
	void RenderSprite(Renderer& renderer, Sprite& sprite, Coord xy);
	void RenderSprite(Renderer& renderer, Sprite& sprite, int x, int y);
	void RenderSidebar(Renderer& renderer);
	void RenderDebug(Renderer& renderer);

	//Input & Updates
	void Input();
	void Update();

	//Events
	void QueueEvent(Event* event);
	void ProcessEventStack();
	void UpdateAuras();
	void AddDelayedTrigger(Spell* spell);
	void RemoveDelayedTrigger(Spell* spell);
	bool IsTrackingDelayedTrigger(Spell* spell);

	//Aura effects
	void ApplyAura(eEffect auraId, Player* owner, Card* sourceCard, Effect* sourceEffect);
	void RemoveAura(Aura* aura);
	void RemoveAurasFromSource(Effect* sourceEffect);
	void RemoveAurasAt(std::vector<int> indices);
	bool IsTrackingAura(Aura* aura);

	//Actions
	void UseCard();
	void UseEffect();
	void PostCast();
	void MoveUnit();
	void AttackUnit();
	Unit* Summon(Unit* card, BoardTile* tile, bool actionBar);
	Unit* SummonToken(eCard cardId, BoardTile* tile, Player* owner);
	Tile* CreateTile(eCard cardId, BoardTile* tile, Player* owner);
	void DispelTile(BoardTile* tile);
	bool RemoveTile(Tile* tile);
	void Replace();
	void ChangeTurn(bool newTurn);
	void SendToGrave(Card* card);

	//Selection & Movement
	void SelectTile(BoardTile& tile);
	void SelectCard();
	void MoveCursor(int x, int y);
	void MoveCursorHand(int x, int y);
	void MoveCursorArtifacts(int x, int y);
	void MoveToHand(int x);
	void MoveToArtifacts(int x);
	void SetTargetSize(int size);
	void SetDefaultSelectionIndex();
	void MoveSelect(int dX, int dY);
	bool IsValidTargetPos(Coord xy);

	//Highlights
	void HighlightTile(int x, int y, eColor color);
	void HighlightMoveable(int x, int y);
	void SearchMoveable(int x, int y, int range);
	void HighlightTargetsForCallback(std::vector<BoardTile*>& targets, EffectContext ctx, BoardTile* source, void (*Callback)(EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets));

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
	std::vector<Unit*> GetUnits();
	std::vector<Tile*> GetTiles();
	std::vector<Aura*> GetAuras();
	std::vector<Spell*> GetDelayedTriggers();
	const std::vector<Card*>& GetGrave() const;
	const std::vector<Unit*>& GetDestroyedUnits() const;
	const std::vector<Spell*>& GetSpellHistory() const;
	const std::vector<PlayableCard*>& GetCardsCastThisTurn() const;
	const int& GetTurnCount() const;
	BoardTile* GetTileInFront(BoardTile* tile, Player* player);

	//Utils
	bool CanMove(int x, int y);
	bool AnyCastThisTurn(bool (*Predicate)(PlayableCard*));
	std::string GetPlayerString(Player* player);
	std::string GetCardString(Card* card);
	void Log(std::string log);

	//Render position evals
	constexpr int TileX(int x) { return (x * 7) + 1; }
	constexpr int TileY(int y) { return (y * 7) + 5; }
	constexpr int SpriteX(int x) { return (x * 7) + 2; }
	constexpr int SpriteY(int y) { return (y * 7) + 6; }

	//Gameplay objects
	Collections* collections;
	EventManager eventManager;
	Map map;

private:

	//Gameplay properties
	eMode mode;
	Player players[2];
	std::vector<Event*> eventStack;
	std::vector<Aura*> auras;
	EffectCallback effectCallback;
	EffectCallback nextEffectCallback;
	std::vector<BoardTile*> callbackTargets;
	std::vector<Spell*> delayedTriggers;
	bool* modeSwitch;
	bool* resetGame;
	bool* exitGame;

	//Turn tracking
	bool turn;
	int turnCount;
	bool endTurn;

	//Selection tracking
	Coord pos;
	Coord castPos;
	int handIdx;
	int artifactIdx;
	int selectionIdx;
	int targetSize;
	int targetMinDelta;
	int targetMaxDelta;
	Unit* activeUnit;
	PlayableCard* activeCard;
	static constexpr int handSize = 7;

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
	std::vector<Tile*> tiles;
	std::vector<Card*> grave;
	std::vector<Unit*> destroyedUnits;
	std::vector<Spell*> spellHistory;
	std::vector<PlayableCard*> castThisTurn;

	//Sprites
	Sprite light;
	Sprite board;
	Sprite tileBorder;
	Sprite chars[10];
	Sprite artifactSprite;
	Sprite durabilitySpriteLeft;
	Sprite durabilitySpriteRight;

	//Debug
	bool debugMode;
	Sprite debugSprite;
	std::vector<std::string> debugLog;

};