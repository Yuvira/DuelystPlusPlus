//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "effectdefinition.h"
#include "game.h"

//Register spell effects
void Collections::RegisterSpells() {

	//Breath of The Unborn
	effectDefinitions.try_emplace(SPELL_BREATH_OF_THE_UNBORN, SPELL_BREATH_OF_THE_UNBORN, KEYWORD_NONE, "Deal 2 damage to all enemy minions. Fully heal all friendly minions");
	effectDefinitions.at(SPELL_BREATH_OF_THE_UNBORN).RegisterSelfTrigger<OnResolveThisEvent>([](EffectContext ctx, OnResolveThisEvent& ev) {
		for (Unit* unit : ctx.game->GetUnits()) {
			if (!unit->IsGeneral()) {
				ctx.SharesOwner(unit)
					? unit->Heal(ctx.card, 999)
					: unit->DealDamage(ctx.card, 2);
			}
		}
	});

	//Dark Seed
	effectDefinitions.try_emplace(SPELL_DARK_SEED, SPELL_DARK_SEED, KEYWORD_NONE, "Deal 1 damage to the enemy general for each card in the opponent's action bar");
	effectDefinitions.at(SPELL_DARK_SEED).RegisterSelfTrigger<OnResolveThisEvent>([](EffectContext ctx, OnResolveThisEvent& ev) {
		if (ev.tile->HasUnit()) {
			int damage = ctx.GetOpponent()->hand.size();
			ev.tile->unit->DealDamage(ctx.card, damage);
		}
	});

}