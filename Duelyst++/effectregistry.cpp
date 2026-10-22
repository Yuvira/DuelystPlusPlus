//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "effectdefinition.h"
#include "game.h"

//Register card effects
void Collections::RegisterEffects() {

	//Dispelled
	effectDefinitions.try_emplace(EFFECT_DISPELLED, EFFECT_DISPELLED, KEYWORD_NONE, "{Dispelled}");

	//Keyword skills
	effectDefinitions.try_emplace(SKILL_AIRDROP, SKILL_AIRDROP, KEYWORD_AIRDROP, "{Airdrop}");
	effectDefinitions.try_emplace(SKILL_CELERITY, SKILL_CELERITY, KEYWORD_CELERITY, "{Celerity}");
	effectDefinitions.try_emplace(SKILL_FLYING, SKILL_FLYING, KEYWORD_FLYING, "{Flying}");
	effectDefinitions.try_emplace(SKILL_FORCEFIELD, SKILL_FORCEFIELD, KEYWORD_FORCEFIELD, "{Forcefield}");
	effectDefinitions.try_emplace(SKILL_FRENZY, SKILL_FRENZY, KEYWORD_FRENZY, "{Frenzy}");
	effectDefinitions.try_emplace(SKILL_PROVOKE, SKILL_PROVOKE, KEYWORD_PROVOKE, "{Provoke}");
	effectDefinitions.try_emplace(SKILL_RANGED, SKILL_RANGED, KEYWORD_RANGED, "{Ranged}");
	effectDefinitions.try_emplace(SKILL_RUSH, SKILL_RUSH, KEYWORD_RUSH, "{Rush}");

	//Abjudicator
	effectDefinitions.try_emplace(SKILL_ABJUDICATOR, SKILL_ABJUDICATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Lower the cost of all spells in your action bar by 1");
	effectDefinitions.at(SKILL_ABJUDICATOR).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (Card* card : ctx.GetOwner()->hand)
			if (card->IsSpell())
				card->AddEffect(MODIFIER_ABJUDICATOR, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_ABJUDICATOR, MODIFIER_ABJUDICATOR, KEYWORD_NONE, "Abjudicator");
	effectDefinitions.at(MODIFIER_ABJUDICATOR).AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Aethermaster
	effectDefinitions.try_emplace(SKILL_AETHERMASTER, SKILL_AETHERMASTER, KEYWORD_NONE, "You may replace an additional card each turn");
	effectDefinitions.at(SKILL_AETHERMASTER).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.GetOwner()->AddMaxReplaces(1); });
	effectDefinitions.at(SKILL_AETHERMASTER).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.GetOwner()->AddMaxReplaces(-1); });

	//Alcuin Loremaster
	effectDefinitions.try_emplace(SKILL_ALCUIN_LOREMASTER, SKILL_ALCUIN_LOREMASTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Put a copy of the most recently cast spell into your action bar");
	effectDefinitions.at(SKILL_ALCUIN_LOREMASTER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (ctx.game->GetSpellHistory().size() > 0)
			ctx.GetOwner()->AddNewToHand(ctx.game->GetSpellHistory().back()->ReadID());
	});

	//Araki Headhunter
	effectDefinitions.try_emplace(SKILL_ARAKI_HEADHUNTER, SKILL_ARAKI_HEADHUNTER, KEYWORD_NONE, "Whenever you summon a minion with {Opening Gambit} from your action bar, gain +2 Attack");
	effectDefinitions.at(SKILL_ARAKI_HEADHUNTER).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ctx.IsAllied(ev.unit) && ev.unit->HasKeywords(KEYWORD_OPENING_GAMBIT) && ev.wasCast; },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->AddEffect(MODIFIER_ARAKI_HEADHUNTER, ctx.card); });
	effectDefinitions.try_emplace(MODIFIER_ARAKI_HEADHUNTER, MODIFIER_ARAKI_HEADHUNTER, KEYWORD_NONE, "Araki Headhunter");
	effectDefinitions.at(MODIFIER_ARAKI_HEADHUNTER).AddModifier(STAT_ATK, MODTYPE_BUFF, 2);

	//Archon Spellbinder
	effectDefinitions.try_emplace(SKILL_ARCHON_SPELLBINDER, SKILL_ARCHON_SPELLBINDER, KEYWORD_NONE, "Your opponent's non-Bloodborn spells cost 1 more to cast");
	effectDefinitions.at(SKILL_ARCHON_SPELLBINDER).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_ARCHON_SPELLBINDER, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_ARCHON_SPELLBINDER).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_ARCHON_SPELLBINDER, AURA_ARCHON_SPELLBINDER, KEYWORD_NONE, "Archon Spellbinder");
	effectDefinitions.at(AURA_ARCHON_SPELLBINDER).SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && !ctx.CardSharesAuraOwner() && ctx.card->IsSpell(); });
	effectDefinitions.at(AURA_ARCHON_SPELLBINDER).AddModifier(STAT_COST, MODTYPE_BUFF, 1);

	//Arrow Whistler
	effectDefinitions.try_emplace(SKILL_ARROW_WHISTLER, SKILL_ARROW_WHISTLER, KEYWORD_RANGED, "{Ranged}|Your other minions with {Ranged} have +1 Attack");
	effectDefinitions.at(SKILL_ARROW_WHISTLER).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_ARROW_WHISTLER, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_ARROW_WHISTLER).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_ARROW_WHISTLER, AURA_ARROW_WHISTLER, KEYWORD_NONE, "Arrow Whistler");
	effectDefinitions.at(AURA_ARROW_WHISTLER).SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->HasKeywords(KEYWORD_RANGED); });
	effectDefinitions.at(AURA_ARROW_WHISTLER).AddModifier(STAT_ATK, MODTYPE_BUFF, 1);

	//Ash Mephyt
	effectDefinitions.try_emplace(SKILL_ASH_MEPHYT, SKILL_ASH_MEPHYT, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon two copies of this minion on random spaces");
	effectDefinitions.at(SKILL_ASH_MEPHYT).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (ctx.card->IsUnit())
			for (int i = 0; i < 2; ++i)
				if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty(ev.tile))
					ctx.game->SummonToken(ctx.card->cardId, tile, ctx.GetOwner());
	});

	//Astral Crusader
	effectDefinitions.try_emplace(SKILL_ASTRAL_CRUSADER, SKILL_ASTRAL_CRUSADER, KEYWORD_NONE, "Whenever you replace this card, it costs 3 less and gains +3/+3");
	effectDefinitions.at(SKILL_ASTRAL_CRUSADER).RegisterPostTrigger<OnReplaceEvent>(
		[](EffectContext ctx, OnReplaceEvent& ev) { return ctx.IsCard(ev.card); },
		[](EffectContext ctx, OnReplaceEvent& ev) { ev.card->AddEffect(MODIFIER_ASTRAL_CRUSADER, ctx.card); });
	effectDefinitions.try_emplace(MODIFIER_ASTRAL_CRUSADER, MODIFIER_ASTRAL_CRUSADER, KEYWORD_NONE, "Astral Crusader");
	effectDefinitions.at(MODIFIER_ASTRAL_CRUSADER).AddModifier(STAT_ATK, MODTYPE_BUFF, 3).AddModifier(STAT_HP, MODTYPE_BUFF, 3).AddModifier(STAT_COST, MODTYPE_BUFF, -3);

	//Azure Herald
	effectDefinitions.try_emplace(SKILL_AZURE_HERALD, SKILL_AZURE_HERALD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 3 Health to your General");
	effectDefinitions.at(SKILL_AZURE_HERALD).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->Heal(ctx.card, 3); });

	//Azure Horn Shaman
	effectDefinitions.try_emplace(SKILL_AZURE_HORN_SHAMAN, SKILL_AZURE_HORN_SHAMAN, KEYWORD_DYING_WISH, "{Dying Wish}: Give +4 Health to friendly minions around it");
	effectDefinitions.at(SKILL_AZURE_HORN_SHAMAN).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->AddEffect(MODIFIER_AZURE_HORN_SHAMAN, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_AZURE_HORN_SHAMAN, MODIFIER_AZURE_HORN_SHAMAN, KEYWORD_NONE, "Azure Horn Shaman");
	effectDefinitions.at(MODIFIER_AZURE_HORN_SHAMAN).AddModifier(STAT_HP, MODTYPE_BUFF, 4);

	//Bastion
	effectDefinitions.try_emplace(SKILL_BASTION, SKILL_BASTION, KEYWORD_NONE, "At the end of your turn, give other friendly minions +1 Health");
	effectDefinitions.at(SKILL_BASTION).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits())
				if (ctx.IsAllied(unit) && !unit->IsGeneral())
					unit->AddEffect(MODIFIER_BASTION, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_BASTION, MODIFIER_BASTION, KEYWORD_NONE, "Bastion");
	effectDefinitions.at(MODIFIER_BASTION).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Black Locust
	effectDefinitions.try_emplace(SKILL_BLACK_LOCUST, SKILL_BLACK_LOCUST, KEYWORD_FLYING, "{Flying}|After this minion moves, summon a Black Locust nearby");
	effectDefinitions.at(SKILL_BLACK_LOCUST).RegisterPostTrigger<OnMoveEvent>(
		[](EffectContext ctx, OnMoveEvent& ev) { return ctx.IsCardOnBoard(ev.unit) && !ev.wasTeleported; },
		[](EffectContext ctx, OnMoveEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetTile()))
				ctx.game->SummonToken(CARD_BLACK_LOCUST, tile, ctx.card->owner);
	});

	//Blaze Hound
	effectDefinitions.try_emplace(SKILL_BLAZE_HOUND, SKILL_BLAZE_HOUND, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Both players draw a card");
	effectDefinitions.at(SKILL_BLAZE_HOUND).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetOwner()->Draw();
		ctx.GetOpponent()->Draw();
	});

	//Blistering Skorn
	effectDefinitions.try_emplace(SKILL_BLISTERING_SKORN, SKILL_BLISTERING_SKORN, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to everything (including itself)");
	effectDefinitions.at(SKILL_BLISTERING_SKORN).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (Unit* unit : ctx.game->GetUnits())
			unit->DealDamage(ctx.card, 1);
		if (ctx.card->IsUnit())
			ctx.card->GetUnit()->DealDamage(ctx.card, 1);
	});

	//Blood Taura
	effectDefinitions.try_emplace(SKILL_BLOOD_TAURA, SKILL_BLOOD_TAURA, KEYWORD_PROVOKE, "{Provoke}|This minion's cost is equal to your General's Health");
	effectDefinitions.at(SKILL_BLOOD_TAURA).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ev.target->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effectDefinitions.at(SKILL_BLOOD_TAURA).RegisterPostTrigger<OnHealEvent>(
		[](EffectContext ctx, OnHealEvent& ev) { return ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnHealEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ev.target->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effectDefinitions.at(SKILL_BLOOD_TAURA).RegisterPostTrigger<OnAddToHandEvent>(
		[](EffectContext ctx, OnAddToHandEvent& ev) { return ctx.IsCard(ev.card); },
		[](EffectContext ctx, OnAddToHandEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ctx.GetAllyGeneral()->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effectDefinitions.at(SKILL_BLOOD_TAURA).AddModifier(STAT_COST, MODTYPE_FIXED, 25);
	effectDefinitions.at(SKILL_BLOOD_TAURA).HideModifiers();

	//Bloodtear Alchemist
	effectDefinitions.try_emplace(SKILL_BLOODTEAR_ALCHEMIST, SKILL_BLOODTEAR_ALCHEMIST, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to an enemy");
	effectDefinitions.at(SKILL_BLOODTEAR_ALCHEMIST).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy(); }));
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->DealDamage(ctx.card, 1);
		});
	});

	//Bluetip Scorpion
	effectDefinitions.try_emplace(SKILL_BLUETIP_SCORPION, SKILL_BLUETIP_SCORPION, KEYWORD_NONE, "Deals double damage to minions");
	effectDefinitions.at(SKILL_BLUETIP_SCORPION).RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.source) && !ev.target->IsGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage *= 2; });

	//Bonereaper
	effectDefinitions.try_emplace(SKILL_BONEREAPER, SKILL_BONEREAPER, KEYWORD_PROVOKE, "{Provoke}|At the end of your turn, deal 2 damage to all nearby enemy minions");
	effectDefinitions.at(SKILL_BONEREAPER).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && !ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->DealDamage(ctx.card, 2);
	});

	//Captain Hank Hart
	effectDefinitions.try_emplace(SKILL_CAPTAIN_HANK_HART, SKILL_CAPTAIN_HANK_HART, KEYWORD_RANGED, "{Ranged}|Whenever this deals damage, restore that much Health to it");
	effectDefinitions.at(SKILL_CAPTAIN_HANK_HART).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.source) && ctx.GetUnit()->hp > 0; },
		[](EffectContext ctx, OnDamageEvent& ev) { ctx.GetUnit()->Heal(ctx.card, ev.damage); });

	//Chakkram
	effectDefinitions.try_emplace(SKILL_CHAKKRAM, SKILL_CHAKKRAM, KEYWORD_NONE, "Costs 2 less if your General took damage on your opponent's last turn");
	effectDefinitions.at(SKILL_CHAKKRAM).RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOwnerTurn(); },
		[](EffectContext ctx, OnTurnStartEvent& ev) {
			if (ctx.game->eventManager.MostRecentPlayerTurnHasEvent(ctx.GetOpponent(), [](Player* player, Event* event) {
				return event->type == EVENT_DAMAGE && event->As<OnDamageEvent>()->target == player->opponent->general;
			})) ctx.card->AddEffect(MODIFIER_CHAKKRAM, ctx.card);
	});
	effectDefinitions.at(SKILL_CHAKKRAM).RegisterPostTrigger<OnAddToHandEvent>(
		[](EffectContext ctx, OnAddToHandEvent& ev) { return ctx.IsCard(ev.card) && ctx.IsOwnerTurn(); },
		[](EffectContext ctx, OnAddToHandEvent& ev) {
			if (ctx.game->eventManager.MostRecentPlayerTurnHasEvent(ctx.GetOpponent(), [](Player* player, Event* event) {
				return event->type == EVENT_DAMAGE && event->As<OnDamageEvent>()->target == player->opponent->general;
			})) ctx.card->AddEffect(MODIFIER_CHAKKRAM, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_CHAKKRAM, MODIFIER_CHAKKRAM, KEYWORD_NONE, "Chakkram");
	effectDefinitions.at(MODIFIER_CHAKKRAM).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) { ctx.card->RemoveEffect(ctx.effect); });
	effectDefinitions.at(MODIFIER_CHAKKRAM).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->RemoveEffect(ctx.effect); });
	effectDefinitions.at(MODIFIER_CHAKKRAM).AddModifier(STAT_COST, MODTYPE_BUFF, -2);

	//Chaos Elemental
	effectDefinitions.try_emplace(SKILL_CHAOS_ELEMENTAL, SKILL_CHAOS_ELEMENTAL, KEYWORD_NONE, "Whenever this minion takes damage, it randomly teleports");
	effectDefinitions.at(SKILL_CHAOS_ELEMENTAL).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty(ctx.GetTile()))
				ctx.GetUnit()->MoveToTile(tile, true);
	});

	//Crimson Oculus
	effectDefinitions.try_emplace(SKILL_CRIMSON_OCULUS, SKILL_CRIMSON_OCULUS, KEYWORD_NONE, "Whenever opponent summons a minion, this minion gets +1/+1");
	effectDefinitions.at(SKILL_CRIMSON_OCULUS).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && !ctx.SharesOwner(ev.unit); },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->AddEffect(MODIFIER_CRIMSON_OCULUS, ctx.card); });
	effectDefinitions.try_emplace(MODIFIER_CRIMSON_OCULUS, MODIFIER_CRIMSON_OCULUS, KEYWORD_NONE, "Crimson Oculus");
	effectDefinitions.at(MODIFIER_CRIMSON_OCULUS).AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Crossbones
	effectDefinitions.try_emplace(SKILL_CROSSBONES, SKILL_CROSSBONES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Destroy an enemy minion with Ranged");
	effectDefinitions.at(SKILL_CROSSBONES).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy() && !tile->unit->IsGeneral() && tile->unit->HasKeywords(KEYWORD_RANGED); }));
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->Destroy(ctx.card);
		});
	});

	//Dancing Blades
	effectDefinitions.try_emplace(SKILL_DANCING_BLADES, SKILL_DANCING_BLADES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to ANY minion in front of this");
	effectDefinitions.at(SKILL_DANCING_BLADES).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (BoardTile* tile = ctx.game->GetTileInFront(ev.tile, ctx.GetOwner()))
			if (tile->HasUnit() && !tile->unit->IsGeneral())
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Dark Nemesis
	effectDefinitions.try_emplace(SKILL_DARK_NEMESIS, SKILL_DARK_NEMESIS, KEYWORD_NONE, "At the start of your turn, deal 4 damage to the enemy General and this minion gains +4 Attack");
	effectDefinitions.at(SKILL_DARK_NEMESIS).RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnStartEvent& ev) {
			ctx.GetEnemyGeneral()->DealDamage(ctx.card, 4);
			ctx.card->AddEffect(MODIFIER_DARK_NEMESIS, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_DARK_NEMESIS, MODIFIER_DARK_NEMESIS, KEYWORD_NONE, "Dark Nemesis");
	effectDefinitions.at(MODIFIER_DARK_NEMESIS).AddModifier(STAT_ATK, MODTYPE_BUFF, 4);

	//Day Watcher
	effectDefinitions.try_emplace(SKILL_DAY_WATCHER, SKILL_DAY_WATCHER, KEYWORD_NONE, "Whenever a friendly minion attacks, restore 1 Health to your General");
	effectDefinitions.at(SKILL_DAY_WATCHER).RegisterPostTrigger<OnAttackEvent>(
		[](EffectContext ctx, OnAttackEvent& ev) { return ctx.IsOnBoard() && ctx.SharesOwner(ev.source) && !ev.source->IsGeneral() && !ev.isCounterattack; },
		[](EffectContext ctx, OnAttackEvent& ev) { ctx.GetAllyGeneral()->Heal(ctx.card, 1); });

	//Deathblighter
	effectDefinitions.try_emplace(SKILL_DEATHBLIGHTER, SKILL_DEATHBLIGHTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to all enemy minions around it");
	effectDefinitions.at(SKILL_DEATHBLIGHTER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile))
			if (tile->HasUnit() && !ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Decimus
	effectDefinitions.try_emplace(SKILL_DECIMUS, SKILL_DECIMUS, KEYWORD_NONE, "Whenever your opponent draws a card, deal 2 damage to the enemy General");
	effectDefinitions.at(SKILL_DECIMUS).RegisterPostTrigger<OnDrawEvent>(
		[](EffectContext ctx, OnDrawEvent& ev) { return ctx.IsOnBoard() && !ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnDrawEvent& ev) { ctx.GetEnemyGeneral()->DealDamage(ctx.card, 2); });

	//Dioltas
	effectDefinitions.try_emplace(SKILL_DIOLTAS, SKILL_DIOLTAS, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 0/8 Tombstone minion with Provoke near your General");
	effectDefinitions.at(SKILL_DIOLTAS).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetAllyGeneral()->curTile))
				ctx.game->SummonToken(CARD_TOMBSTONE, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SKILL_DIOLTAS).SetToken(CARD_TOMBSTONE);

	//Dreamgazer
	effectDefinitions.try_emplace(SKILL_DREAMGAZER, SKILL_DREAMGAZER, KEYWORD_NONE, "When you replace this card, summon it nearby. Your General takes 2 damage");
	effectDefinitions.at(SKILL_DREAMGAZER).RegisterPostTrigger<OnReplaceEvent>(
		[](EffectContext ctx, OnReplaceEvent& ev) { return ctx.IsCard(ev.card) && ctx.card->IsUnit(); },
		[](EffectContext ctx, OnReplaceEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetAllyGeneral()->curTile)) {
				if (ctx.GetOwner()->RemoveFromDeck(ctx.card)) {
					ctx.game->Summon(ctx.card, tile, false);
					ctx.GetAllyGeneral()->DealDamage(ctx.card, 2);
				}
			}
	});

	//Dust Wailer
	effectDefinitions.try_emplace(SKILL_DUST_WAILER, SKILL_DUST_WAILER, KEYWORD_FLYING | KEYWORD_OPENING_GAMBIT, "{Flying}|{Opening Gambit}: Deal 3 damage to all enemies in front of this");
	effectDefinitions.at(SKILL_DUST_WAILER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		BoardTile* tile = ev.tile;
		while (tile = ctx.game->GetTileInFront(tile, ctx.GetOwner()))
			if (tile->HasUnit() && !ctx.SharesOwner(tile->unit))
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Eclipse
	effectDefinitions.try_emplace(SKILL_ECLIPSE, SKILL_ECLIPSE, KEYWORD_NONE, "Whenever this minion takes damage, it deals that much damage to the enemy General");
	effectDefinitions.at(SKILL_ECLIPSE).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) { ctx.GetEnemyGeneral()->DealDamage(ctx.card, ev.damage); });

	//Emerald Rejuvenator
	effectDefinitions.try_emplace(SKILL_EMERALD_REJUVENATOR, SKILL_EMERALD_REJUVENATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 4 Health to BOTH Generals");
	effectDefinitions.at(SKILL_EMERALD_REJUVENATOR).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->Heal(ctx.card, 4);
		ctx.GetEnemyGeneral()->Heal(ctx.card, 4);
	});

	//Envybaer
	effectDefinitions.try_emplace(SKILL_ENVYBAER, SKILL_ENVYBAER, KEYWORD_NONE, "Whenever this minion damages an enemy, teleport that enemy to a random corner");
	effectDefinitions.at(SKILL_ENVYBAER).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.source) && !ctx.SharesOwner(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyCorner())
				ev.target->MoveToTile(tile, true);
	});

	//Ephemeral Shroud
	effectDefinitions.try_emplace(SKILL_EPHEMERAL_SHROUD, SKILL_EPHEMERAL_SHROUD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel 1 nearby space");
	effectDefinitions.at(SKILL_EPHEMERAL_SHROUD).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, nullptr), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				target->SetFeature(TILE_NONE);
				if (target->HasUnit())
					target->unit->Dispel();
		});
	});

	//E'Xun
	effectDefinitions.try_emplace(SKILL_EXUN, SKILL_EXUN, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever this minion attacks or is attacked, draw a card");
	effectDefinitions.at(SKILL_EXUN).RegisterPostTrigger<OnAttackEvent>(
		[](EffectContext ctx, OnAttackEvent& ev) { return (ctx.IsCardOnBoard(ev.source) || ctx.IsCardOnBoard(ev.target)) && !ev.isCounterattack; },
		[](EffectContext ctx, OnAttackEvent& ev) { ctx.GetOwner()->Draw(); });

	//Facestriker
	effectDefinitions.try_emplace(SKILL_FACESTRIKER, SKILL_FACESTRIKER, KEYWORD_NONE, "Deals double damage to Generals");
	effectDefinitions.at(SKILL_FACESTRIKER).RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.source) && ev.target->IsGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage *= 2; });

	//Firestarter
	effectDefinitions.try_emplace(SKILL_FIRESTARTER, SKILL_FIRESTARTER, KEYWORD_NONE, "Whenever you cast a spell, summon a 1/1 Spellspark with Rush on a random nearby space");
	effectDefinitions.at(SKILL_FIRESTARTER).RegisterPostTrigger<OnCastEvent>(
		[](EffectContext ctx, OnCastEvent& ev) { return ctx.IsOnBoard() && ev.card->IsSpell() && ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnCastEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetUnit()->curTile))
				ctx.game->SummonToken(CARD_SPELLSPARK, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SKILL_FIRESTARTER).SetToken(CARD_SPELLSPARK);

	//First Sword of Akrane
	effectDefinitions.try_emplace(SKILL_FIRST_SWORD_OF_AKRANE, SKILL_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "Your other minions have +1 Attack");
	effectDefinitions.at(SKILL_FIRST_SWORD_OF_AKRANE).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_FIRST_SWORD_OF_AKRANE, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_FIRST_SWORD_OF_AKRANE).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_FIRST_SWORD_OF_AKRANE, AURA_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "First Sword of Akrane");
	effectDefinitions.at(AURA_FIRST_SWORD_OF_AKRANE).SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral(); });
	effectDefinitions.at(AURA_FIRST_SWORD_OF_AKRANE).AddModifier(STAT_ATK, MODTYPE_BUFF, 1);

	//Flameblood Warlock
	effectDefinitions.try_emplace(SKILL_FLAMEBLOOD_WARLOCK, SKILL_FLAMEBLOOD_WARLOCK, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to BOTH Generals");
	effectDefinitions.at(SKILL_FLAMEBLOOD_WARLOCK).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->DealDamage(ctx.card, 3);
		ctx.GetEnemyGeneral()->DealDamage(ctx.card, 3);
	});

	//Frostbone Naga
	effectDefinitions.try_emplace(SKILL_FROSTBONE_NAGA, SKILL_FROSTBONE_NAGA, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to everything around it");
	effectDefinitions.at(SKILL_FROSTBONE_NAGA).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile))
			if (tile->HasUnit())
				tile->unit->DealDamage(ctx.card, 2);
	});

	//Ghost Lynx
	effectDefinitions.try_emplace(SKILL_GHOST_LYNX, SKILL_GHOST_LYNX, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Teleport a nearby minion to a random space");
	effectDefinitions.at(SKILL_GHOST_LYNX).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && !tile->unit->IsGeneral(); }), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty(source, target))
						target->unit->MoveToTile(tile, true);
		});
	});

	//Golden Justicar
	effectDefinitions.try_emplace(SKILL_GOLDEN_JUSTICAR, SKILL_GOLDEN_JUSTICAR, KEYWORD_PROVOKE, "{Provoke}|Your other minions with {Provoke} can move two additional spaces");
	effectDefinitions.at(SKILL_GOLDEN_JUSTICAR).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GOLDEN_JUSTICAR, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_GOLDEN_JUSTICAR).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_GOLDEN_JUSTICAR, AURA_GOLDEN_JUSTICAR, KEYWORD_NONE, "Golden Justicar");
	effectDefinitions.at(AURA_GOLDEN_JUSTICAR).SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->HasKeywords(KEYWORD_PROVOKE); });
	effectDefinitions.at(AURA_GOLDEN_JUSTICAR).AddModifier(STAT_MOVE, MODTYPE_BUFF, 2);

	//Golem Metallurgist
	effectDefinitions.try_emplace(SKILL_GOLEM_METALLURGIST, SKILL_GOLEM_METALLURGIST, KEYWORD_NONE, "The first Golem you summon each turn costs 1 less");
	effectDefinitions.at(SKILL_GOLEM_METALLURGIST).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
			ctx.game->ApplyAura(AURA_GOLEM_METALLURGIST, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_GOLEM_METALLURGIST).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_GOLEM_METALLURGIST, AURA_GOLEM_METALLURGIST, KEYWORD_NONE, "Golem Metallurgist");
	effectDefinitions.at(AURA_GOLEM_METALLURGIST).SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && ctx.CardSharesAuraOwner() && ctx.card->IsUnit() && ctx.card->GetUnit()->tribe == TRIBE_GOLEM
			&& !ctx.game->AnyCastThisTurn([](Card* card) { return card->IsUnit() && card->GetUnit()->tribe == TRIBE_GOLEM; }); });
	effectDefinitions.at(AURA_GOLEM_METALLURGIST).AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Golem Vanquisher
	effectDefinitions.try_emplace(SKILL_GOLEM_VANQUISHER, SKILL_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "{Provoke}|Your other Golem minions have {Provoke}");
	effectDefinitions.at(SKILL_GOLEM_VANQUISHER).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GOLEM_VANQUISHER, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_GOLEM_VANQUISHER).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_GOLEM_VANQUISHER, AURA_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "Golem Vanquisher|{Provoke}");
	effectDefinitions.at(AURA_GOLEM_VANQUISHER).SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->tribe == TRIBE_GOLEM; });

	//Grove Lion
	effectDefinitions.try_emplace(SKILL_GROVE_LION, SKILL_GROVE_LION, KEYWORD_NONE, "While this minion is on the battlefield, your General has {Forcefield}");
	effectDefinitions.at(SKILL_GROVE_LION).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GROVE_LION, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_GROVE_LION).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_GROVE_LION, AURA_GROVE_LION, KEYWORD_FORCEFIELD, "Grove Lion|{Forcefield}");
	effectDefinitions.at(AURA_GROVE_LION).SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.card == ctx.effect->GetSourceCard()->owner->general; });

	//Healing Mystic
	effectDefinitions.try_emplace(SKILL_HEALING_MYSTIC, SKILL_HEALING_MYSTIC, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 2 Health to anything");
	effectDefinitions.at(SKILL_HEALING_MYSTIC).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) { return tile->HasUnit(); }));
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->Heal(ctx.card, 2);
		});
	});

	//Hollow Grovekeeper
	effectDefinitions.try_emplace(SKILL_HOLLOW_GROVEKEEPER, SKILL_HOLLOW_GROVEKEEPER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Destroy ANY nearby minion with Provoke or Frenzy. If you do, this minion gains Provoke and Frenzy");
	effectDefinitions.at(SKILL_HOLLOW_GROVEKEEPER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && !tile->unit->IsGeneral() && (tile->unit->HasKeywords(KEYWORD_PROVOKE) || tile->unit->HasKeywords(KEYWORD_FRENZY)); }), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit()) {
					target->unit->Destroy(ctx.card);
					ctx.card->AddEffect(MODIFIER_HOLLOW_GROVEKEEPER, ctx.card);
				}
		});
	});
	effectDefinitions.try_emplace(MODIFIER_HOLLOW_GROVEKEEPER, MODIFIER_HOLLOW_GROVEKEEPER, KEYWORD_PROVOKE | KEYWORD_FRENZY, "Hollow Grovekeeper|{Provoke}, {Frenzy}");

	//Ironclad
	effectDefinitions.try_emplace(SKILL_IRONCLAD, SKILL_IRONCLAD, KEYWORD_DYING_WISH, "{Dying Wish}: Dispel all enemy minions");
	effectDefinitions.at(SKILL_IRONCLAD).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits())
				if (!ctx.SharesOwner(unit) && !unit->IsGeneral())
					unit->Dispel();
	});

	//Jax Truesight
	effectDefinitions.try_emplace(SKILL_JAX_TRUESIGHT, SKILL_JAX_TRUESIGHT, KEYWORD_OPENING_GAMBIT | KEYWORD_RANGED, "{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged} Mini-Jax in each corner");
	effectDefinitions.at(SKILL_JAX_TRUESIGHT).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetEmptyCorners())
			if (tile != ev.tile)
				ctx.game->SummonToken(CARD_MINI_JAX, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SKILL_JAX_TRUESIGHT).SetToken(CARD_MINI_JAX);

	//Jaxi
	effectDefinitions.try_emplace(SKILL_JAXI, SKILL_JAXI, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 1/1 {Ranged} Mini-Jax in a random corner");
	effectDefinitions.at(SKILL_JAXI).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyCorner())
				ctx.game->SummonToken(CARD_MINI_JAX, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SKILL_JAXI).SetToken(CARD_MINI_JAX);

	//Keeper of the Vale
	effectDefinitions.try_emplace(SKILL_KEEPER_OF_THE_VALE, SKILL_KEEPER_OF_THE_VALE, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon a random non-token friendly minion destroyed this game nearby");
	effectDefinitions.at(SKILL_KEEPER_OF_THE_VALE).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return !tile->HasUnit(); }), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (!target->HasUnit()) {
					std::vector<Card*> valid;
					for (Card* destroyed : ctx.game->GetDestroyedUnits())
						if (!destroyed->isToken && destroyed->IsUnit() && ctx.SharesOwner(destroyed))
							valid.push_back(destroyed);
					if (valid.size() > 0)
						ctx.game->SummonToken(valid[rand() % valid.size()]->cardId, target, ctx.GetOwner());
				}
		});
	});

	//Khymera
	effectDefinitions.try_emplace(SKILL_KHYMERA, SKILL_KHYMERA, KEYWORD_NONE, "Whenever this minion takes damage, summon a random token minion nearby");
	effectDefinitions.at(SKILL_KHYMERA).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ev.target->curTile))
				ctx.game->SummonToken(ctx.game->collections->GetRandomTokenMinionID(), tile, ctx.GetOwner());
	});

	//Lady Locke
	effectDefinitions.try_emplace(SKILL_LADY_LOCKE, SKILL_LADY_LOCKE, KEYWORD_OPENING_GAMBIT | KEYWORD_PROVOKE, "{Provoke}|{Opening Gambit}: Other minions you summon this turn gain +1/+1 and gain {Provoke}");
	effectDefinitions.at(SKILL_LADY_LOCKE).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->ApplyAura(AURA_LADY_LOCKE, ctx.card, ctx.effect); });
	effectDefinitions.try_emplace(AURA_LADY_LOCKE, AURA_LADY_LOCKE, KEYWORD_NONE, "Lady Locke|Minions you summon this turn gain +1/+1 and gain {Provoke}");
	effectDefinitions.at(AURA_LADY_LOCKE).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsAllied(ev.unit) && !ev.unit->IsGeneral(); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->AddEffect(MODIFIER_LADY_LOCKE, ctx.card); });
	effectDefinitions.at(AURA_LADY_LOCKE).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) { ctx.game->RemoveAura(ctx.effect); });
	effectDefinitions.try_emplace(MODIFIER_LADY_LOCKE, MODIFIER_LADY_LOCKE, KEYWORD_PROVOKE, "Lady Locke|{Provoke}");
	effectDefinitions.at(MODIFIER_LADY_LOCKE).AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Lightbender
	effectDefinitions.try_emplace(SKILL_LIGHTBENDER, SKILL_LIGHTBENDER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel ALL spaces around it");
	effectDefinitions.at(SKILL_LIGHTBENDER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile)) {
			tile->SetFeature(TILE_NONE);
			if (tile->HasUnit())
				tile->unit->Dispel();
		}
	});

	//Lux Ignis
	effectDefinitions.try_emplace(SKILL_LUX_IGNIS, SKILL_LUX_IGNIS, KEYWORD_RANGED, "{Ranged}|At the end of your turn, restore 2 Health to all nearby friendly minions");
	effectDefinitions.at(SKILL_LUX_IGNIS).RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && ctx.IsAllied(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->Heal(ctx.card, 2);
	});
	
	//Manaforger
	effectDefinitions.try_emplace(SKILL_MANAFORGER, SKILL_MANAFORGER, KEYWORD_NONE, "The first non-Bloodborn spell you cast each turn costs 1 less");
	effectDefinitions.at(SKILL_MANAFORGER).RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
			ctx.game->ApplyAura(AURA_MANAFORGER, ctx.card, ctx.effect); });
	effectDefinitions.at(SKILL_MANAFORGER).RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effectDefinitions.try_emplace(AURA_MANAFORGER, AURA_MANAFORGER, KEYWORD_NONE, "Manaforger");
	effectDefinitions.at(AURA_MANAFORGER).SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && ctx.CardSharesAuraOwner() && ctx.card->IsSpell()
			&& !ctx.game->AnyCastThisTurn([](Card* card) { return card->IsSpell(); }); });
	effectDefinitions.at(AURA_MANAFORGER).AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Maw
	effectDefinitions.try_emplace(SKILL_MAW, SKILL_MAW, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to a nearby enemy minion");
	effectDefinitions.at(SKILL_MAW).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy() && !tile->unit->IsGeneral(); }), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->DealDamage(ctx.card, 2);
		});
	});

	//Mindwarper
	effectDefinitions.try_emplace(SKILL_MINDWARPER, SKILL_MINDWARPER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Gain a copy of a random spell from your opponent's action bar");
	effectDefinitions.at(SKILL_MINDWARPER).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		std::vector<Card*> valid;
		for (Card* card : ctx.GetOpponent()->hand)
			if (card->IsSpell())
				valid.push_back(card);
		if (valid.size() > 0)
			ctx.GetOwner()->AddNewToHand(valid[rand() % valid.size()]->ReadID());
	});

	//Mirkblood Devourer
	effectDefinitions.try_emplace(SKILL_MIRKBLOOD_DEVOURER, SKILL_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "Friendly minions summoned nearby this minion gain +1/+1");
	effectDefinitions.at(SKILL_MIRKBLOOD_DEVOURER).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ctx.IsAllied(ev.unit) && ctx.GetTile()->IsNear(ev.unit->curTile); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->AddEffect(MODIFIER_MIRKBLOOD_DEVOURER, ctx.card); });
	effectDefinitions.try_emplace(MODIFIER_MIRKBLOOD_DEVOURER, MODIFIER_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "Mirkblood Devourer");
	effectDefinitions.at(MODIFIER_MIRKBLOOD_DEVOURER).AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Moebius
	effectDefinitions.try_emplace(SKILL_MOEBIUS, SKILL_MOEBIUS, KEYWORD_NONE, "At the start of your turn, fully heal this minion and switch its Attack and Health");
	effectDefinitions.at(SKILL_MOEBIUS).RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnStartEvent& ev) {
			int atk = ctx.card->GetStat(STAT_ATK, ctx.card->GetUnit()->baseAtk, false);
			int hp = ctx.card->GetStat(STAT_HP, ctx.card->GetUnit()->baseHpMax, false);
			if (Effect* effect = ctx.card->AddEffect(MODIFIER_MOEBIUS, ctx.card)) {
				effect->UpdateModifier(STAT_ATK, hp);
				effect->UpdateModifier(STAT_HP, atk);
			}
			ctx.card->UpdateModifiers();
			ctx.card->GetUnit()->hp = ctx.card->GetUnit()->hpMax;
	});
	effectDefinitions.try_emplace(MODIFIER_MOEBIUS, MODIFIER_MOEBIUS, KEYWORD_NONE, "Moebius");
	effectDefinitions.at(MODIFIER_MOEBIUS).AddModifier(STAT_ATK, MODTYPE_OVERRIDE, 3).AddModifier(STAT_HP, MODTYPE_OVERRIDE, 5);
	effectDefinitions.at(MODIFIER_MOEBIUS).SetRemovable(false);

	//Mogwai
	effectDefinitions.try_emplace(SKILL_MOGWAI, SKILL_MOGWAI, KEYWORD_NONE, "After this moves, draw a card");
	effectDefinitions.at(SKILL_MOGWAI).RegisterPostTrigger<OnMoveEvent>(
		[](EffectContext ctx, OnMoveEvent& ev) { return ctx.IsCardOnBoard(ev.unit) && !ev.wasTeleported; },
		[](EffectContext ctx, OnMoveEvent& ev) { ctx.GetOwner()->Draw(); });

	//Necroseer
	effectDefinitions.try_emplace(SKILL_NECROSEER, SKILL_NECROSEER, KEYWORD_DYING_WISH, "{Dying Wish}: Draw a card");
	effectDefinitions.at(SKILL_NECROSEER).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) { ctx.GetOwner()->Draw(); });

	//Night Watcher
	effectDefinitions.try_emplace(SKILL_NIGHT_WATCHER, SKILL_NIGHT_WATCHER, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever ANY player summons a minion with {Rush}, exhaust it");
	effectDefinitions.at(SKILL_NIGHT_WATCHER).RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ev.unit->HasKeywords(KEYWORD_RUSH); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->Exhaust(); });

	//Owlbeast Sage
	effectDefinitions.try_emplace(SKILL_OWLBEAST_SAGE, SKILL_OWLBEAST_SAGE, KEYWORD_NONE, "Whenever you cast a spell, your Arcanyst minions gain +2 Health");
	effectDefinitions.at(SKILL_OWLBEAST_SAGE).RegisterPostTrigger<OnCastEvent>(
		[](EffectContext ctx, OnCastEvent& ev) { return ctx.IsOnBoard() && ev.card->IsSpell() && ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnCastEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits())
				if (ctx.SharesOwner(ctx.card) && !unit->IsGeneral() && unit->tribe == TRIBE_ARCANYST)
					unit->AddEffect(MODIFIER_OWLBEAST_SAGE, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_OWLBEAST_SAGE, MODIFIER_OWLBEAST_SAGE, KEYWORD_NONE, "Owlbeast Sage");
	effectDefinitions.at(MODIFIER_OWLBEAST_SAGE).AddModifier(STAT_HP, MODTYPE_BUFF, 2);

	//Paddo
	effectDefinitions.try_emplace(SKILL_PADDO, SKILL_PADDO, KEYWORD_AIRDROP | KEYWORD_OPENING_GAMBIT, "{Airdrop}|{Opening Gambit}: Push ALL nearby minions and Generals to random spaces");
	effectDefinitions.at(SKILL_PADDO).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile))
			if (tile->HasUnit())
				if (BoardTile* target = ctx.GetMap()->GetRandomEmpty(ev.tile))
					tile->unit->MoveToTile(target, true);
	});

	//Primus Fist
	effectDefinitions.try_emplace(SKILL_PRIMUS_FIST, SKILL_PRIMUS_FIST, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Give a nearby friendly minion +2 Attack this turn");
	effectDefinitions.at(SKILL_PRIMUS_FIST).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsAlly() && !tile->unit->IsGeneral(); }), ev.tile);
		ctx.game->GenerateCallback(ctx, ev.tile,
			[](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->AddEffect(MODIFIER_PRIMUS_FIST, ctx.card);
		});
	});
	effectDefinitions.try_emplace(MODIFIER_PRIMUS_FIST, MODIFIER_PRIMUS_FIST, KEYWORD_NONE, "Primus Fist");
	effectDefinitions.at(MODIFIER_PRIMUS_FIST).RegisterPostTrigger<OnTurnEndEvent>(
		nullptr,
		[](EffectContext ctx, OnTurnEndEvent& ev) { ctx.card->RemoveEffect(ctx.effect); });
	effectDefinitions.at(MODIFIER_PRIMUS_FIST).AddModifier(STAT_ATK, MODTYPE_BUFF, 2);

	//Prismatic Illusionist
	effectDefinitions.try_emplace(SKILL_PRISMATIC_ILLUSIONIST, SKILL_PRISMATIC_ILLUSIONIST, KEYWORD_NONE, "Whenever you cast a spell, summon a 2/1 Illusion on a random nearby space");
	effectDefinitions.at(SKILL_PRISMATIC_ILLUSIONIST).RegisterPostTrigger<OnCastEvent>(
		[](EffectContext ctx, OnCastEvent& ev) { return ctx.IsOnBoard() && ev.card->IsSpell() && ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnCastEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetUnit()->curTile))
				ctx.game->SummonToken(CARD_ILLUSION, tile, ctx.GetOwner());
	});
	effectDefinitions.at(SKILL_PRISMATIC_ILLUSIONIST).SetToken(CARD_ILLUSION);

	//Purgatos, the Realmkeeper
	effectDefinitions.try_emplace(SKILL_PURGATOS_THE_REALMKEEPER, SKILL_PURGATOS_THE_REALMKEEPER, KEYWORD_NONE, "Whenever this minion deals damage, either deal 3 damage to the enemy General OR restore 3 Health to your General");
	effectDefinitions.at(SKILL_PURGATOS_THE_REALMKEEPER).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.source); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			int target = rand() % 2;
			if (target == 0)
				ctx.GetAllyGeneral()->Heal(nullptr, 3);
			else
				ctx.GetEnemyGeneral()->DealDamage(nullptr, 3);
	});

	//Prophet of the White Palm
	effectDefinitions.try_emplace(SKILL_PROPHET_OF_THE_WHITE_PALM, SKILL_PROPHET_OF_THE_WHITE_PALM, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Prevent ALL spell damage until your next turn");
	effectDefinitions.at(SKILL_PROPHET_OF_THE_WHITE_PALM).RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->ApplyAura(AURA_PROPHET_OF_THE_WHITE_PALM, ctx.card, ctx.effect); });
	effectDefinitions.try_emplace(AURA_PROPHET_OF_THE_WHITE_PALM, AURA_PROPHET_OF_THE_WHITE_PALM, KEYWORD_NONE, "Prophet of the White Palm|Spell damage is prevented until your next turn");
	effectDefinitions.at(AURA_PROPHET_OF_THE_WHITE_PALM).RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ev.source->IsSpell(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage = 0; });
	effectDefinitions.at(AURA_PROPHET_OF_THE_WHITE_PALM).RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnStartEvent& ev) { ctx.game->RemoveAura(ctx.effect); });

	//Quartermaster Gauj
	effectDefinitions.try_emplace(SKILL_QUARTERMASTER_GAUJ, SKILL_QUARTERMASTER_GAUJ, KEYWORD_NONE, "Takes no damage from minions or Generals");
	effectDefinitions.at(SKILL_QUARTERMASTER_GAUJ).RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.target) && ev.source->IsUnit(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage = 0; });

	//Red Synja
	effectDefinitions.try_emplace(SKILL_RED_SYNJA, SKILL_RED_SYNJA, KEYWORD_NONE, "Whenever your General takes damage, deal 7 damage to a random nearby enemy minion");
	effectDefinitions.at(SKILL_RED_SYNJA).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsOnBoard() && ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			std::vector<Unit*> valid;
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && !ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
					valid.push_back(tile->unit);
			if (valid.size() > 0)
				valid[rand() % valid.size()]->DealDamage(ctx.card, 7);
	});

	//Ruby Rifter
	effectDefinitions.try_emplace(SKILL_RUBY_RIFTER, SKILL_RUBY_RIFTER, KEYWORD_NONE, "Whenever your General takes damage, draw a card and give this minion +2 Attack");
	effectDefinitions.at(SKILL_RUBY_RIFTER).RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsOnBoard() && ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			ctx.GetOwner()->Draw();
			ctx.card->AddEffect(MODIFIER_RUBY_RIFTER, ctx.card);
	});
	effectDefinitions.try_emplace(MODIFIER_RUBY_RIFTER, MODIFIER_RUBY_RIFTER, KEYWORD_NONE, "Ruby Rifter");
	effectDefinitions.at(MODIFIER_RUBY_RIFTER).AddModifier(STAT_ATK, MODTYPE_BUFF, 2);

	//Sarlac the Eternal
	effectDefinitions.try_emplace(SKILL_SARLAC_THE_ETERNAL, SKILL_SARLAC_THE_ETERNAL, KEYWORD_DYING_WISH, "{Dying Wish}: Re-summon this minion on a random space");
	effectDefinitions.at(SKILL_SARLAC_THE_ETERNAL).RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty())
				ctx.game->SummonToken(ctx.card->ReadID(), tile, ctx.GetOwner());
	});

}