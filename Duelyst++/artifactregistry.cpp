//Include
#include "card.h"
#include "collections.h"
#include "effectcontext.h"
#include "effectdefinition.h"
#include "event.h"
#include "game.h"

//Register spell effects
void Collections::RegisterArtifacts() {

	//Horn of the Forsaken
	effectDefinitions.try_emplace(ARTIFACT_HORN_OF_THE_FORSAKEN, ARTIFACT_HORN_OF_THE_FORSAKEN, KEYWORD_NONE, "Whenever your General deals damage, summon a 1/1 Wraithling nearby your General");
	effectDefinitions.at(ARTIFACT_HORN_OF_THE_FORSAKEN).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) {
			return ctx.IsActiveArtifact() && ev.source == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetAllyGeneral()->curTile))
				ctx.game->SummonToken(CARD_WRAITHLING, tile, ctx.GetOwner());
	});
	effectDefinitions.at(ARTIFACT_HORN_OF_THE_FORSAKEN).SetToken(CARD_WRAITHLING);

}