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

}