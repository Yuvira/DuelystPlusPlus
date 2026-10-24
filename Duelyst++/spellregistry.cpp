//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "effectcontext.h"
#include "effectdefinition.h"
#include "game.h"

//Register spell effects
void Collections::RegisterSpells() {

	//Breath of The Unborn
	effectDefinitions.try_emplace(SPELL_BREATH_OF_THE_UNBORN, SPELL_BREATH_OF_THE_UNBORN, "Deal 2 damage to all enemy minions. Fully heal all friendly minions",
		[](Game* game, Spell* spell) {
			return game->map.GetAll(); });
	effectDefinitions.at(SPELL_BREATH_OF_THE_UNBORN).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			for (Unit* unit : ctx.GetMap()->GetAllUnits(FILTER_MINIONS, FILTER_NEUTRAL, nullptr))
				ctx.SharesOwner(unit)
					? unit->Heal(ctx.card, 999)
					: unit->DealDamage(ctx.card, 2);
	});

	//Consuming Rebirth
	effectDefinitions.try_emplace(SPELL_CONSUMING_REBIRTH, SPELL_CONSUMING_REBIRTH, "Destroy a friendly minion. At the end of your turn, re-summon it on the same space and give it +1/+1.",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_CONSUMING_REBIRTH).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ctx.IsSpellContext() && ev.tile->HasUnit()) {
				SpellContext sctx = ctx.GetSpellContext();
				sctx.effect->AddTarget(ev.tile);
				sctx.effect->SetToken(ev.tile->unit->ReadID());
				ev.tile->unit->Destroy(sctx.spell);
				sctx.game->AddDelayedTrigger(sctx.spell);
			}
	});
	effectDefinitions.at(SPELL_CONSUMING_REBIRTH).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			return ctx.IsSpellContext() && ctx.game->IsTrackingDelayedTrigger(ctx.card->GetSpell()) && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			if (ctx.IsSpellContext()) {
				SpellContext sctx = ctx.GetSpellContext();
				std::vector<BoardTile*> targets = sctx.effect->GetEffectTargets();
				if (targets.size() > 0 && !targets[0]->HasUnit())
					if (Unit* unit = ctx.game->SummonToken(sctx.effect->ReadToken(), targets[0], ctx.GetOwner()))
						unit->AddEffect(MODIFIER_CONSUMING_REBIRTH);
				ctx.game->RemoveDelayedTrigger(sctx.spell);
			}
	});
	effectDefinitions.try_emplace(MODIFIER_CONSUMING_REBIRTH, MODIFIER_CONSUMING_REBIRTH, KEYWORD_NONE, "Consuming Rebirth");
	effectDefinitions.at(MODIFIER_CONSUMING_REBIRTH).AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Daemonic Lure
	effectDefinitions.try_emplace(SPELL_DAEMONIC_LURE, SPELL_DAEMONIC_LURE, "Deal 1 damage to an enemy minion and teleport it anywhere",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ENEMY, spell->owner); });
	effectDefinitions.at(SPELL_DAEMONIC_LURE).RegisterSelfTrigger<OnPreCastThisEvent>(
		[](EffectContext ctx, OnPreCastThisEvent& ev) {
			std::vector<BoardTile*> valid = ctx.GetMap()->GetAllEmpty();
			ctx.game->HighlightTargetsForCallback(valid, ctx, ev.tile,
				[](EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets) {
					if (ctx.IsSpellContext() && targets.size() > 0 && !targets[0]->HasUnit())
						ctx.GetSpellContext().effect->AddTarget(targets[0]);
			});
	});
	effectDefinitions.at(SPELL_DAEMONIC_LURE).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->DealDamage(ctx.card, 1);
			if (ctx.IsSpellContext()) {
				std::vector<BoardTile*> targets = ctx.effect->GetSpellEffect()->GetEffectTargets();
				if (targets.size() > 0 && !targets[0]->HasUnit())
					ev.tile->unit->MoveToTile(targets[0], true);
			}
	});

	//Dark Seed
	effectDefinitions.try_emplace(SPELL_DARK_SEED, SPELL_DARK_SEED, "Deal 1 damage to the enemy general for each card in the opponent's action bar",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_GENERALS, FILTER_ENEMY, spell->owner); });
	effectDefinitions.at(SPELL_DARK_SEED).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			int damage = ctx.GetOpponent()->hand.size();
			ctx.GetEnemyGeneral()->DealDamage(ctx.card, damage);
	});

	//Dark Transformation
	effectDefinitions.try_emplace(SPELL_DARK_TRANSFORMATION, SPELL_DARK_TRANSFORMATION, "Destroy an enemy minion and summon a 1/1 Wraithling on that space",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ENEMY, spell->owner); });
	effectDefinitions.at(SPELL_DARK_TRANSFORMATION).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->Destroy(ctx.card);
			ctx.game->SummonToken(CARD_WRAITHLING, ev.tile, ctx.GetOwner());
	});
	effectDefinitions.at(SPELL_DARK_TRANSFORMATION).SetToken(CARD_WRAITHLING);

	//Darkfire Sacrifice
	effectDefinitions.try_emplace(SPELL_DARKFIRE_SACRIFICE, SPELL_DARKFIRE_SACRIFICE, "Destroy a friendly minion. Your next minion costs 2 less",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_DARKFIRE_SACRIFICE).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->Destroy(ctx.card);
			ctx.game->ApplyAura(AURA_DARKFIRE_SACRIFICE, ctx.GetOwner(), ctx.card, ctx.effect);
	});
	effectDefinitions.try_emplace(AURA_DARKFIRE_SACRIFICE, AURA_DARKFIRE_SACRIFICE, KEYWORD_NONE, "Darkfire Sacrifice",
		[](EffectContext ctx) { return ctx.IsAuraContext() && ctx.IsInOwnerHand() && ctx.GetAuraContext().CardSharesAuraOwner() && ctx.card->IsMinion(); });
	effectDefinitions.at(AURA_DARKFIRE_SACRIFICE).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) {
			return ctx.IsAuraContext() && ctx.GetAuraContext().SharesAuraOwner(ev.unit) && ev.unit->IsMinion() && ev.wasCast; },
		[](EffectContext ctx, OnSummonEvent& ev) {
			ctx.game->RemoveAura(ctx.effect->GetAura()); });
	effectDefinitions.at(AURA_DARKFIRE_SACRIFICE).AddModifier(STAT_COST, MODTYPE_BUFF, -2);

}