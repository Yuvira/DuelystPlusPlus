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

#pragma region Abyssian

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

	//Deathfire Crescendo
	effectDefinitions.try_emplace(SPELL_DEATHFIRE_CRESCENDO, SPELL_DEATHFIRE_CRESCENDO, "Give a friendly minion, \"{Deathwatch}: Gains +2/+2\"",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_DEATHFIRE_CRESCENDO).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->AddEffect(EFFECT_DEATHFIRE_CRESCENDO);
	});
	effectDefinitions.try_emplace(EFFECT_DEATHFIRE_CRESCENDO, EFFECT_DEATHFIRE_CRESCENDO, KEYWORD_DEATHWATCH, "Deathfire Crescendo|{Deathwatch}: Gains +2/+2");
	effectDefinitions.at(EFFECT_DEATHFIRE_CRESCENDO).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) {
			return ctx.IsOnBoard() && !ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			ctx.card->AddEffect(MODIFIER_DEATHFIRE_CRESCENDO);
	});
	effectDefinitions.try_emplace(MODIFIER_DEATHFIRE_CRESCENDO, MODIFIER_DEATHFIRE_CRESCENDO, KEYWORD_NONE, "Deathfire Crescendo");
	effectDefinitions.at(MODIFIER_DEATHFIRE_CRESCENDO).AddModifier(STAT_ATK, MODTYPE_BUFF, 2).AddModifier(STAT_HP, MODTYPE_BUFF, 2);

	//Grasp of Agony
	effectDefinitions.try_emplace(SPELL_GRASP_OF_AGONY, SPELL_GRASP_OF_AGONY, "Choose an enemy minion. When it dies, it deals 3 damage to enemies around it",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ENEMY, spell->owner); });
	effectDefinitions.at(SPELL_GRASP_OF_AGONY).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->AddEffect(EFFECT_GRASP_OF_AGONY);
	});
	effectDefinitions.try_emplace(EFFECT_GRASP_OF_AGONY, EFFECT_GRASP_OF_AGONY, KEYWORD_NONE, "Grasp of Agony|When this dies, it deals 3 damage to allies around it");
	effectDefinitions.at(EFFECT_GRASP_OF_AGONY).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) {
			return ctx.IsCardOnBoard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			for (Unit* unit : ctx.GetMap()->GetUnitsNear(ctx.GetTile(), FILTER_ALL, FILTER_ALLY, ctx.GetOwner()))
				unit->DealDamage(ctx.card, 3);
	});

	//Nether Summoning
	effectDefinitions.try_emplace(SPELL_NETHER_SUMMONING, SPELL_NETHER_SUMMONING, "Summon 2 non-token minions destroyed since the end of your last turn nearby",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_GENERALS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_NETHER_SUMMONING).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			std::vector<Unit*> valid;
			OnDeathEvent* deathEvent = nullptr;
			for (Event* event : ctx.game->eventManager.GetEventsSincePlayersLastTurn(ctx.GetOwner())) {
				if (deathEvent = event->As<OnDeathEvent>())
					if (!deathEvent->unit->IsGeneral() && !deathEvent->unit->isToken)
						valid.push_back(deathEvent->unit);
			}
			BoardTile* tile = nullptr;
			for (int i = 0; i < 2; ++i) {
				if (valid.size() > 0) {
					if (tile = ctx.GetMap()->GetRandomEmptyNear(ev.tile)) {
						int idx = rand() % valid.size();
						ctx.game->SummonToken(valid[idx]->ReadID(), tile, ctx.GetOwner());
						valid.erase(valid.begin() + idx);
					}
				}
			}
	});

	//Rite of the Undervault
	effectDefinitions.try_emplace(SPELL_RITE_OF_THE_UNDERVAULT, SPELL_RITE_OF_THE_UNDERVAULT, "Draw to fill your action bar",
		[](Game* game, Spell* spell) {
			return game->map.GetAll(); });
	effectDefinitions.at(SPELL_RITE_OF_THE_UNDERVAULT).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			while (ctx.GetOwner()->hand.size() < ctx.GetOwner()->maxHandSize && ctx.GetOwner()->deck.size() > 0)
				ctx.GetOwner()->Draw();
	});

	//Ritual Banishing
	effectDefinitions.try_emplace(SPELL_RITUAL_BANSHING, SPELL_RITUAL_BANSHING, "Destroy a friendly minion to destroy an enemy minion",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_RITUAL_BANSHING).RegisterSelfTrigger<OnPreCastThisEvent>(
		[](EffectContext ctx, OnPreCastThisEvent& ev) {
			std::vector<BoardTile*> valid = ctx.GetMap()->GetAllUnitTiles(FILTER_MINIONS, FILTER_ENEMY, ctx.GetOwner());
			ctx.game->HighlightTargetsForCallback(valid, ctx, ev.tile,
				[](EffectContext ctx, BoardTile* source, std::vector<BoardTile*>& targets) {
					if (ctx.IsSpellContext() && targets.size() > 0 && targets[0]->HasUnit())
						ctx.GetSpellContext().effect->AddTarget(targets[0]);
			});
	});
	effectDefinitions.at(SPELL_RITUAL_BANSHING).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->Destroy(ctx.card);
			if (ctx.IsSpellContext()) {
				std::vector<BoardTile*> targets = ctx.effect->GetSpellEffect()->GetEffectTargets();
				if (targets.size() > 0 && targets[0]->HasUnit())
					targets[0]->unit->Destroy(ctx.card);
			}
	});

	//Shadow Nova
	effectDefinitions.try_emplace(SPELL_SHADOW_NOVA, SPELL_SHADOW_NOVA, "Turn a 2x2 area into {Shadow Creep}",
		[](Game* game, Spell* spell) {
			game->SetTargetSize(2); return game->map.GetAll(); });
	effectDefinitions.at(SPELL_SHADOW_NOVA).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->Get2x2(ev.tile->pos))
				ctx.game->CreateTile(CARD_SHADOW_CREEP, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SPELL_SHADOW_NOVA).SetToken(CARD_SHADOW_CREEP);

	//Shadow Reflection
	effectDefinitions.try_emplace(SPELL_SHADOW_REFLECTION, SPELL_SHADOW_REFLECTION, "Give a friendly minion +5 Attack",
		[](Game* game, Spell* spell) {
			return game->map.GetAllUnitTiles(FILTER_MINIONS, FILTER_ALLY, spell->owner); });
	effectDefinitions.at(SPELL_SHADOW_REFLECTION).RegisterSelfTrigger<OnResolveThisEvent>(
		[](EffectContext ctx, OnResolveThisEvent& ev) {
			if (ev.tile->HasUnit())
				ev.tile->unit->AddEffect(MODIFIER_SHADOW_REFLECTION);
	});
	effectDefinitions.try_emplace(MODIFIER_SHADOW_REFLECTION, MODIFIER_SHADOW_REFLECTION, KEYWORD_NONE, "Shadow Reflection");
	effectDefinitions.at(MODIFIER_SHADOW_REFLECTION).AddModifier(STAT_ATK, MODTYPE_BUFF, 5);

#pragma endregion

}