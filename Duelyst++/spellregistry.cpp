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
	effectDefinitions.try_emplace(SPELL_BREATH_OF_THE_UNBORN, SPELL_BREATH_OF_THE_UNBORN, "Deal 2 damage to all enemy minions. Fully heal all friendly minions",
		[](Game* game, Spell* spell) {
			std::vector<BoardTile*> valid;
			for (Unit* unit : game->GetUnits())
				if (!unit->IsGeneral())
					valid.push_back(unit->curTile);
			return valid;
	});
	effectDefinitions.at(SPELL_BREATH_OF_THE_UNBORN).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits()) {
				if (!unit->IsGeneral()) {
					ctx.SharesOwner(unit)
						? unit->Heal(ctx.card, 999)
						: unit->DealDamage(ctx.card, 2);
				}
			}
	});

	//Consuming Rebirth
	effectDefinitions.try_emplace(SPELL_CONSUMING_REBIRTH, SPELL_CONSUMING_REBIRTH, "Destroy a friendly minion. At the end of your turn, re-summon it on the same space and give it +1/+1.",
		[](Game* game, Spell* spell) {
			std::vector<BoardTile*> valid;
			for (Unit* unit : game->GetUnits())
				if (spell->owner == unit->owner && !unit->IsGeneral())
					valid.push_back(unit->curTile);
			return valid;
	});
	effectDefinitions.at(SPELL_CONSUMING_REBIRTH).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ctx.card->IsSpell() && ctx.effect->IsSpellEffect() && ev.tile->HasUnit()) {
				ctx.effect->GetSpellEffect()->AddTarget(ev.tile);
				ctx.effect->SetToken(ev.tile->unit->ReadID());
				ev.tile->unit->Destroy(ctx.card);
				ctx.game->AddDelayedTrigger(ctx.GetSpell());
			}
	});
	effectDefinitions.at(SPELL_CONSUMING_REBIRTH).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			return ctx.card->IsSpell() && ctx.game->IsTrackingDelayedTrigger(ctx.card->GetSpell()) && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			if (ctx.effect->IsSpellEffect()) {
				std::vector<BoardTile*> targets = ctx.effect->GetSpellEffect()->GetEffectTargets();
				if (targets.size() > 0 && !targets[0]->HasUnit())
					if (Unit* unit = ctx.game->SummonToken(ctx.effect->GetSpellEffect()->ReadToken(), targets[0], ctx.GetOwner()))
						unit->AddEffect(MODIFIER_CONSUMING_REBIRTH);
			}
			ctx.game->RemoveDelayedTrigger(ctx.GetSpell());
	});
	effectDefinitions.try_emplace(MODIFIER_CONSUMING_REBIRTH, MODIFIER_CONSUMING_REBIRTH, KEYWORD_NONE, "Consuming Rebirth");
	effectDefinitions.at(MODIFIER_CONSUMING_REBIRTH).AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Dark Seed
	effectDefinitions.try_emplace(SPELL_DARK_SEED, SPELL_DARK_SEED, "Deal 1 damage to the enemy general for each card in the opponent's action bar",
		[](Game* game, Spell* spell) {
			return std::vector<BoardTile*> { spell->owner->opponent->general->curTile }; });
	effectDefinitions.at(SPELL_DARK_SEED).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit()) {
				int damage = ctx.GetOpponent()->hand.size();
				ev.tile->unit->DealDamage(ctx.card, damage);
			}
	});

}