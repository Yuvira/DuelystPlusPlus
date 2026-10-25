//Include
#include "card.h"
#include "collections.h"
#include "effectcontext.h"
#include "effectdefinition.h"
#include "event.h"
#include "game.h"

//Register spell effects
void Collections::RegisterTiles() {

	//Mana Spring
	effectDefinitions.try_emplace(TILE_MANA_SPRING, TILE_MANA_SPRING, KEYWORD_NONE, "Gain +1 mana this turn");
	effectDefinitions.at(TILE_MANA_SPRING).RegisterSelfTrigger<OnAddThisEvent>(
		[](EffectContext ctx, OnAddThisEvent& ev) {
			if (ctx.card->IsTile()) {
				BoardTile* tile = ctx.card->GetTile()->curTile;
				if (tile != nullptr && tile->HasUnit()) {
					tile->unit->owner->AddMana(1);
					ctx.game->CreateTile(CARD_MANA_SPRING_EMPTY, tile, ctx.GetOwner());
				}
			}
	});
	effectDefinitions.at(TILE_MANA_SPRING).RegisterSelfTrigger<OnThisTileUnitChangedEvent>(
		[](EffectContext ctx, OnThisTileUnitChangedEvent& ev) {
			if (ctx.card->IsTile()) {
				BoardTile* tile = ctx.card->GetTile()->curTile;
				if (tile != nullptr && tile->HasUnit()) {
					tile->unit->owner->AddMana(1);
					ctx.game->CreateTile(CARD_MANA_SPRING_EMPTY, tile, ctx.GetOwner());
				}
			}
	});

	//Shadow Creep
	effectDefinitions.try_emplace(TILE_SHADOW_CREEP, TILE_SHADOW_CREEP, KEYWORD_NONE, "Deals 1 damage to enemy minions and Generals standing on it at the end of owner's turn");
	effectDefinitions.at(TILE_SHADOW_CREEP).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			return ctx.GetOwner() == ev.player && ctx.card->IsTile() && ctx.GetTile() != nullptr && ctx.GetTile()->HasUnit() && ctx.GetTile()->unit->owner != ctx.GetOwner(); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			ctx.GetTile()->unit->DealDamage(ctx.card, 1); });

}