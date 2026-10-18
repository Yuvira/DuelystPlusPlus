//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "game.h"

//Register card effects
void Collections::RegisterEffects() {

	//Dispelled
	effects[EFFECT_DISPELLED] = Effect(EFFECT_DISPELLED, KEYWORD_NONE, "{Dispelled}");

	//Keyword skills
	effects[SKILL_AIRDROP] = Effect(SKILL_AIRDROP, KEYWORD_AIRDROP, "{Airdrop}");
	effects[SKILL_CELERITY] = Effect(SKILL_CELERITY, KEYWORD_CELERITY, "{Celerity}");
	effects[SKILL_FLYING] = Effect(SKILL_FLYING, KEYWORD_FLYING, "{Flying}");
	effects[SKILL_FORCEFIELD] = Effect(SKILL_FORCEFIELD, KEYWORD_FORCEFIELD, "{Forcefield}");
	effects[SKILL_FRENZY] = Effect(SKILL_FRENZY, KEYWORD_FRENZY, "{Frenzy}");
	effects[SKILL_PROVOKE] = Effect(SKILL_PROVOKE, KEYWORD_PROVOKE, "{Provoke}");
	effects[SKILL_RANGED] = Effect(SKILL_RANGED, KEYWORD_RANGED, "{Ranged}");
	effects[SKILL_RUSH] = Effect(SKILL_RUSH, KEYWORD_RUSH, "{Rush}");

	//Abjudicator
	effects[SKILL_ABJUDICATOR] = Effect(SKILL_ABJUDICATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Lower the cost of all spells in your action bar by 1");
	effects[SKILL_ABJUDICATOR].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (Card* card : ctx.GetOwner()->hand)
			if (card->IsSpell())
				card->AddEffect(MODIFIER_ABJUDICATOR, ctx.card);
	});
	effects[MODIFIER_ABJUDICATOR] = Effect(MODIFIER_ABJUDICATOR, KEYWORD_NONE, "Abjudicator");
	effects[MODIFIER_ABJUDICATOR].AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Aethermaster
	effects[SKILL_AETHERMASTER] = Effect(SKILL_AETHERMASTER, KEYWORD_NONE, "You may replace an additional card each turn");
	effects[SKILL_AETHERMASTER].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.GetOwner()->ModifyReplaceCount(1); });
	effects[SKILL_AETHERMASTER].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.GetOwner()->ModifyReplaceCount(-1); });

	//Alcuin Loremaster
	effects[SKILL_ALCUIN_LOREMASTER] = Effect(SKILL_ALCUIN_LOREMASTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Put a copy of the most recently cast spell into your action bar");
	effects[SKILL_ALCUIN_LOREMASTER].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (ctx.game->GetSpellHistory().size() > 0)
			ctx.GetOwner()->AddNewToHand(ctx.game->GetSpellHistory().back()->original);
	});

	//Araki Headhunter
	effects[SKILL_ARAKI_HEADHUNTER] = Effect(SKILL_ARAKI_HEADHUNTER, KEYWORD_NONE, "Whenever you summon a minion with {Opening Gambit} from your action bar, gain +2 Attack");
	effects[SKILL_ARAKI_HEADHUNTER].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ctx.IsAllied(ev.unit) && ev.unit->HasKeywords(KEYWORD_OPENING_GAMBIT) && ev.wasCast; },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->AddEffect(MODIFIER_ARAKI_HEADHUNTER, ctx.card); });
	effects[MODIFIER_ARAKI_HEADHUNTER] = Effect(MODIFIER_ARAKI_HEADHUNTER, KEYWORD_NONE, "Araki Headhunter");
	effects[MODIFIER_ARAKI_HEADHUNTER].AddModifier(STAT_ATK, MODTYPE_BUFF, 2);

	//Archon Spellbinder
	effects[SKILL_ARCHON_SPELLBINDER] = Effect(SKILL_ARCHON_SPELLBINDER, KEYWORD_NONE, "Your opponent's non-Bloodborn spells cost 1 more to cast");
	effects[SKILL_ARCHON_SPELLBINDER].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_ARCHON_SPELLBINDER, ctx.effect, ctx.card); });
	effects[SKILL_ARCHON_SPELLBINDER].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_ARCHON_SPELLBINDER] = Effect(AURA_ARCHON_SPELLBINDER, KEYWORD_NONE, "Archon Spellbinder");
	effects[AURA_ARCHON_SPELLBINDER].SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && !ctx.CardSharesAuraOwner() && ctx.card->IsSpell(); });
	effects[AURA_ARCHON_SPELLBINDER].AddModifier(STAT_COST, MODTYPE_BUFF, 1);

	//Arrow Whistler
	effects[SKILL_ARROW_WHISTLER] = Effect(SKILL_ARROW_WHISTLER, KEYWORD_RANGED, "{Ranged}|Your other minions with {Ranged} have +1 Attack");
	effects[SKILL_ARROW_WHISTLER].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_ARROW_WHISTLER, ctx.effect, ctx.card); });
	effects[SKILL_ARROW_WHISTLER].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_ARROW_WHISTLER] = Effect(AURA_ARROW_WHISTLER, KEYWORD_NONE, "Arrow Whistler");
	effects[AURA_ARROW_WHISTLER].SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->HasKeywords(KEYWORD_RANGED); });
	effects[AURA_ARROW_WHISTLER].AddModifier(STAT_ATK, MODTYPE_BUFF, 1);

	//Ash Mephyt
	effects[SKILL_ASH_MEPHYT] = Effect(SKILL_ASH_MEPHYT, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon two copies of this minion on random spaces");
	effects[SKILL_ASH_MEPHYT].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (ctx.card->IsUnit())
			for (int i = 0; i < 2; ++i)
				if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty(ev.tile))
					ctx.game->SummonToken(ctx.card->cardId, tile, ctx.GetOwner());
	});

	//Astral Crusader
	effects[SKILL_ASTRAL_CRUSADER] = Effect(SKILL_ASTRAL_CRUSADER, KEYWORD_NONE, "Whenever you replace this card, it costs 3 less and gains +3/+3");
	effects[SKILL_ASTRAL_CRUSADER].RegisterPostTrigger<OnReplaceEvent>(
		[](EffectContext ctx, OnReplaceEvent& ev) { return ctx.IsCard(ev.card); },
		[](EffectContext ctx, OnReplaceEvent& ev) { ev.card->AddEffect(MODIFIER_ASTRAL_CRUSADER, ctx.card); });
	effects[MODIFIER_ASTRAL_CRUSADER] = Effect(MODIFIER_ASTRAL_CRUSADER, KEYWORD_NONE, "Astral Crusader");
	effects[MODIFIER_ASTRAL_CRUSADER].AddModifier(STAT_ATK, MODTYPE_BUFF, 3).AddModifier(STAT_HP, MODTYPE_BUFF, 3).AddModifier(STAT_COST, MODTYPE_BUFF, -3);

	//Azure Herald
	effects[SKILL_AZURE_HERALD] = Effect(SKILL_AZURE_HERALD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 3 Health to your General");
	effects[SKILL_AZURE_HERALD].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->Heal(ctx.card, 3); });

	//Azure Horn Shaman
	effects[SKILL_AZURE_HORN_SHAMAN] = Effect(SKILL_AZURE_HORN_SHAMAN, KEYWORD_DYING_WISH, "{Dying Wish}: Give +4 Health to friendly minions around it");
	effects[SKILL_AZURE_HORN_SHAMAN].RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->AddEffect(MODIFIER_AZURE_HORN_SHAMAN, ctx.card);
	});
	effects[MODIFIER_AZURE_HORN_SHAMAN] = Effect(MODIFIER_AZURE_HORN_SHAMAN, KEYWORD_NONE, "Azure Horn Shaman");
	effects[MODIFIER_AZURE_HORN_SHAMAN].AddModifier(STAT_HP, MODTYPE_BUFF, 4);

	//Bastion
	effects[SKILL_BASTION] = Effect(SKILL_BASTION, KEYWORD_NONE, "At the end of your turn, give other friendly minions +1 Health");
	effects[SKILL_BASTION].RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits())
				if (ctx.IsAllied(unit) && !unit->IsGeneral())
					unit->AddEffect(MODIFIER_BASTION, ctx.card);
	});
	effects[MODIFIER_BASTION] = Effect(MODIFIER_BASTION, KEYWORD_NONE, "Bastion");
	effects[MODIFIER_BASTION].AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Black Locust
	effects[SKILL_BLACK_LOCUST] = Effect(SKILL_BLACK_LOCUST, KEYWORD_FLYING, "{Flying}|After this minion moves, summon a Black Locust nearby");
	effects[SKILL_BLACK_LOCUST].RegisterPostTrigger<OnMoveEvent>(
		[](EffectContext ctx, OnMoveEvent& ev) { return ctx.IsCardOnBoard(ev.unit) && !ev.wasTeleported; },
		[](EffectContext ctx, OnMoveEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetTile()))
				ctx.game->SummonToken(CARD_BLACK_LOCUST, tile, ctx.card->owner);
	});

	//Blaze Hound
	effects[SKILL_BLAZE_HOUND] = Effect(SKILL_BLAZE_HOUND, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Both players draw a card");
	effects[SKILL_BLAZE_HOUND].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetOwner()->Draw();
		ctx.GetOpponent()->Draw();
	});

	//Blistering Skorn
	effects[SKILL_BLISTERING_SKORN] = Effect(SKILL_BLISTERING_SKORN, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to everything (including itself)");
	effects[SKILL_BLISTERING_SKORN].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (Unit* unit : ctx.game->GetUnits())
			unit->DealDamage(ctx.card, 1);
		if (ctx.card->IsUnit())
			ctx.card->GetUnit()->DealDamage(ctx.card, 1);
	});

	//Blood Taura
	effects[SKILL_BLOOD_TAURA] = Effect(SKILL_BLOOD_TAURA, KEYWORD_PROVOKE, "{Provoke}|This minion's cost is equal to your General's Health");
	effects[SKILL_BLOOD_TAURA].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ev.target->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effects[SKILL_BLOOD_TAURA].RegisterPostTrigger<OnHealEvent>(
		[](EffectContext ctx, OnHealEvent& ev) { return ev.target == ctx.GetAllyGeneral(); },
		[](EffectContext ctx, OnHealEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ev.target->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effects[SKILL_BLOOD_TAURA].RegisterPostTrigger<OnAddToHandEvent>(
		[](EffectContext ctx, OnAddToHandEvent& ev) { return ctx.IsCard(ev.card); },
		[](EffectContext ctx, OnAddToHandEvent& ev) {
			ctx.effect->UpdateModifier(STAT_COST, max(ctx.GetAllyGeneral()->hp, 0));
			ctx.card->UpdateModifiers();
	});
	effects[SKILL_BLOOD_TAURA].AddModifier(STAT_COST, MODTYPE_FIXED, 25);
	effects[SKILL_BLOOD_TAURA].HideModifiers();

	//Bloodtear Alchemist
	effects[SKILL_BLOODTEAR_ALCHEMIST] = Effect(SKILL_BLOODTEAR_ALCHEMIST, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to an enemy");
	effects[SKILL_BLOODTEAR_ALCHEMIST].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy(); }));
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->DealDamage(ctx.card, 1);
			};
		}
	});

	//Bluetip Scorpion
	effects[SKILL_BLUETIP_SCORPION] = Effect(SKILL_BLUETIP_SCORPION, KEYWORD_NONE, "Deals double damage to minions");
	effects[SKILL_BLUETIP_SCORPION].RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.source) && !ev.target->IsGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage *= 2; });

	//Bonereaper
	effects[SKILL_BONEREAPER] = Effect(SKILL_BONEREAPER, KEYWORD_PROVOKE, "{Provoke}|At the end of your turn, deal 2 damage to all nearby enemy minions");
	effects[SKILL_BONEREAPER].RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && !ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->DealDamage(ctx.card, 2);
	});

	//Captain Hank Hart
	effects[SKILL_CAPTAIN_HANK_HART] = Effect(SKILL_CAPTAIN_HANK_HART, KEYWORD_RANGED, "{Ranged}|Whenever this deals damage, restore that much Health to it");
	effects[SKILL_CAPTAIN_HANK_HART].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.source) && ctx.GetUnit()->hp > 0; },
		[](EffectContext ctx, OnDamageEvent& ev) { ctx.GetUnit()->Heal(ctx.card, ev.damage); });

	//Chakkram
	effects[SKILL_CHAKKRAM] = Effect(SKILL_CHAKKRAM, KEYWORD_NONE, "Costs 2 less if your General took damage on your opponent's last turn");
	effects[SKILL_CHAKKRAM].RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOwnerTurn(); },
		[](EffectContext ctx, OnTurnStartEvent& ev) {
			if (ctx.game->eventManager.MostRecentPlayerTurnHasEvent(ctx.GetOpponent(), [](Player* player, Event* event) {
				return event->type == EVENT_DAMAGE && event->As<OnDamageEvent>()->target == player->opponent->general;
			})) ctx.card->AddEffect(MODIFIER_CHAKKRAM, ctx.card);
	});
	effects[SKILL_CHAKKRAM].RegisterPostTrigger<OnAddToHandEvent>(
		[](EffectContext ctx, OnAddToHandEvent& ev) { return ctx.IsCard(ev.card) && ctx.IsOwnerTurn(); },
		[](EffectContext ctx, OnAddToHandEvent& ev) {
			if (ctx.game->eventManager.MostRecentPlayerTurnHasEvent(ctx.GetOpponent(), [](Player* player, Event* event) {
				return event->type == EVENT_DAMAGE && event->As<OnDamageEvent>()->target == player->opponent->general;
			})) ctx.card->AddEffect(MODIFIER_CHAKKRAM, ctx.card);
	});
	effects[MODIFIER_CHAKKRAM] = Effect(MODIFIER_CHAKKRAM, KEYWORD_NONE, "Chakkram");
	effects[MODIFIER_CHAKKRAM].RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) { ctx.card->RemoveEffect(ctx.effect); });
	effects[MODIFIER_CHAKKRAM].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->RemoveEffect(ctx.effect); });
	effects[MODIFIER_CHAKKRAM].AddModifier(STAT_COST, MODTYPE_BUFF, -2);

	//Chaos Elemental
	effects[SKILL_CHAOS_ELEMENTAL] = Effect(SKILL_CHAOS_ELEMENTAL, KEYWORD_NONE, "Whenever this minion takes damage, it randomly teleports");
	effects[SKILL_CHAOS_ELEMENTAL].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmpty(ctx.GetTile()))
				ctx.GetUnit()->MoveToTile(tile, true);
	});

	//Crimson Oculus
	effects[SKILL_CRIMSON_OCULUS] = Effect(SKILL_CRIMSON_OCULUS, KEYWORD_NONE, "Whenever opponent summons a minion, this minion gets +1/+1");
	effects[SKILL_CRIMSON_OCULUS].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && !ctx.SharesOwner(ev.unit); },
		[](EffectContext ctx, OnSummonEvent& ev) { ctx.card->AddEffect(MODIFIER_CRIMSON_OCULUS, ctx.card); });
	effects[MODIFIER_CRIMSON_OCULUS] = Effect(MODIFIER_CRIMSON_OCULUS, KEYWORD_NONE, "Crimson Oculus");
	effects[MODIFIER_CRIMSON_OCULUS].AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Crossbones
	effects[SKILL_CROSSBONES] = Effect(SKILL_CROSSBONES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Destroy an enemy minion with Ranged");
	effects[SKILL_CROSSBONES].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy() && !tile->unit->IsGeneral() && tile->unit->HasKeywords(KEYWORD_RANGED); }));
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->Destroy(ctx.card);
			};
		}
	});

	//Dancing Blades
	effects[SKILL_DANCING_BLADES] = Effect(SKILL_DANCING_BLADES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to ANY minion in front of this");
	effects[SKILL_DANCING_BLADES].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		if (BoardTile* tile = ctx.game->GetTileInFront(ev.tile, ctx.GetOwner()))
			if (tile->HasUnit() && !tile->unit->IsGeneral())
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Dark Nemesis
	effects[SKILL_DARK_NEMESIS] = Effect(SKILL_DARK_NEMESIS, KEYWORD_NONE, "At the start of your turn, deal 4 damage to the enemy General and this minion gains +4 Attack");
	effects[SKILL_DARK_NEMESIS].RegisterPostTrigger<OnTurnStartEvent>(
		[](EffectContext ctx, OnTurnStartEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnStartEvent& ev) {
			ctx.GetEnemyGeneral()->DealDamage(ctx.card, 4);
			ctx.card->AddEffect(MODIFIER_DARK_NEMESIS, ctx.card);
	});
	effects[MODIFIER_DARK_NEMESIS] = Effect(MODIFIER_DARK_NEMESIS, KEYWORD_NONE, "Dark Nemesis");
	effects[MODIFIER_DARK_NEMESIS].AddModifier(STAT_ATK, MODTYPE_BUFF, 4);

	//Day Watcher
	effects[SKILL_DAY_WATCHER] = Effect(SKILL_DAY_WATCHER, KEYWORD_NONE, "Whenever a friendly minion attacks, restore 1 Health to your General");
	effects[SKILL_DAY_WATCHER].RegisterPostTrigger<OnAttackEvent>(
		[](EffectContext ctx, OnAttackEvent& ev) { return ctx.IsOnBoard() && ctx.SharesOwner(ev.source) && !ev.source->IsGeneral() && !ev.isCounterattack; },
		[](EffectContext ctx, OnAttackEvent& ev) { ctx.GetAllyGeneral()->Heal(ctx.card, 1); });

	//Deathblighter
	effects[SKILL_DEATHBLIGHTER] = Effect(SKILL_DEATHBLIGHTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to all enemy minions around it");
	effects[SKILL_DEATHBLIGHTER].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile))
			if (tile->HasUnit() && !ctx.SharesOwner(tile->unit) && !tile->unit->IsGeneral())
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Decimus
	effects[SKILL_DECIMUS] = Effect(SKILL_DECIMUS, KEYWORD_NONE, "Whenever your opponent draws a card, deal 2 damage to the enemy General");
	effects[SKILL_DECIMUS].RegisterPostTrigger<OnDrawEvent>(
		[](EffectContext ctx, OnDrawEvent& ev) { return ctx.IsOnBoard() && !ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnDrawEvent& ev) { ctx.GetEnemyGeneral()->DealDamage(ctx.card, 2); });

	//Dioltas
	effects[SKILL_DIOLTAS] = Effect(SKILL_DIOLTAS, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 0/8 Tombstone minion with Provoke near your General");
	effects[SKILL_DIOLTAS].RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetAllyGeneral()->curTile))
				ctx.game->SummonToken(CARD_TOMBSTONE, tile, ctx.GetOwner());
	});

	//Dreamgazer
	effects[SKILL_DREAMGAZER] = Effect(SKILL_DREAMGAZER, KEYWORD_NONE, "When you replace this card, summon it nearby. Your General takes 2 damage");
	effects[SKILL_DREAMGAZER].RegisterPostTrigger<OnReplaceEvent>(
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
	effects[SKILL_DUST_WAILER] = Effect(SKILL_DUST_WAILER, KEYWORD_FLYING | KEYWORD_OPENING_GAMBIT, "{Flying}|{Opening Gambit}: Deal 3 damage to all enemies in front of this");
	effects[SKILL_DUST_WAILER].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		BoardTile* tile = ev.tile;
		while (tile = ctx.game->GetTileInFront(tile, ctx.GetOwner()))
			if (tile->HasUnit() && !ctx.SharesOwner(tile->unit))
				tile->unit->DealDamage(ctx.card, 3);
	});

	//Eclipse
	effects[SKILL_ECLIPSE] = Effect(SKILL_ECLIPSE, KEYWORD_NONE, "Whenever this minion takes damage, it deals that much damage to the enemy General");
	effects[SKILL_ECLIPSE].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) { ctx.GetEnemyGeneral()->DealDamage(ctx.card, ev.damage); });

	//Emerald Rejuvenator
	effects[SKILL_EMERALD_REJUVENATOR] = Effect(SKILL_EMERALD_REJUVENATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 4 Health to BOTH Generals");
	effects[SKILL_EMERALD_REJUVENATOR].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->Heal(ctx.card, 4);
		ctx.GetEnemyGeneral()->Heal(ctx.card, 4);
	});

	//Envybaer
	effects[SKILL_ENVYBAER] = Effect(SKILL_ENVYBAER, KEYWORD_NONE, "Whenever this minion damages an enemy, teleport that enemy to a random corner");
	effects[SKILL_ENVYBAER].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.source) && !ctx.SharesOwner(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyCorner())
				ev.target->MoveToTile(tile, true);
	});

	//Ephemeral Shroud
	effects[SKILL_EPHEMERAL_SHROUD] = Effect(SKILL_EPHEMERAL_SHROUD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel 1 nearby space");
	effects[SKILL_EPHEMERAL_SHROUD].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, nullptr), ev.tile);
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				target->SetFeature(TILE_NONE);
				if (target->HasUnit())
					target->unit->Dispel();
			};
		}
	});

	//E'Xun
	effects[SKILL_EXUN] = Effect(SKILL_EXUN, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever this minion attacks or is attacked, draw a card");
	effects[SKILL_EXUN].RegisterPostTrigger<OnAttackEvent>(
		[](EffectContext ctx, OnAttackEvent& ev) { return (ctx.IsCardOnBoard(ev.source) || ctx.IsCardOnBoard(ev.target)) && !ev.isCounterattack; },
		[](EffectContext ctx, OnAttackEvent& ev) { ctx.GetOwner()->Draw(); });

	//Facestriker
	effects[SKILL_FACESTRIKER] = Effect(SKILL_FACESTRIKER, KEYWORD_NONE, "Deals double damage to Generals");
	effects[SKILL_FACESTRIKER].RegisterPreTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCard(ev.source) && ev.target->IsGeneral(); },
		[](EffectContext ctx, OnDamageEvent& ev) { ev.damage *= 2; });

	//Firestarter
	effects[SKILL_FIRESTARTER] = Effect(SKILL_FIRESTARTER, KEYWORD_NONE, "Whenever you cast a spell, summon a 1/1 Spellspark with Rush on a random nearby space");
	effects[SKILL_FIRESTARTER].RegisterPostTrigger<OnCastEvent>(
		[](EffectContext ctx, OnCastEvent& ev) { return ctx.IsOnBoard() && ev.card->IsSpell() && ctx.SharesOwner(ev.card); },
		[](EffectContext ctx, OnCastEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ctx.GetUnit()->curTile))
				ctx.game->SummonToken(CARD_SPELLSPARK, tile, ctx.GetOwner());
	});

	//First Sword of Akrane
	effects[SKILL_FIRST_SWORD_OF_AKRANE] = Effect(SKILL_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "Your other minions have +1 Attack");
	effects[SKILL_FIRST_SWORD_OF_AKRANE].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_FIRST_SWORD_OF_AKRANE, ctx.effect, ctx.card); });
	effects[SKILL_FIRST_SWORD_OF_AKRANE].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_FIRST_SWORD_OF_AKRANE] = Effect(AURA_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "First Sword of Akrane");
	effects[AURA_FIRST_SWORD_OF_AKRANE].SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral(); });
	effects[AURA_FIRST_SWORD_OF_AKRANE].AddModifier(STAT_ATK, MODTYPE_BUFF, 1);

	//Flameblood Warlock
	effects[SKILL_FLAMEBLOOD_WARLOCK] = Effect(SKILL_FLAMEBLOOD_WARLOCK, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to BOTH Generals");
	effects[SKILL_FLAMEBLOOD_WARLOCK].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.GetAllyGeneral()->DealDamage(ctx.card, 3);
		ctx.GetEnemyGeneral()->DealDamage(ctx.card, 3);
	});

	//Frostbone Naga
	effects[SKILL_FROSTBONE_NAGA] = Effect(SKILL_FROSTBONE_NAGA, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to everything around it");
	effects[SKILL_FROSTBONE_NAGA].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile))
			if (tile->HasUnit())
				tile->unit->DealDamage(ctx.card, 2);
	});

	//Ghost Lynx
	effects[SKILL_GHOST_LYNX] = Effect(SKILL_GHOST_LYNX, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Teleport a nearby minion to a random space");
	effects[SKILL_GHOST_LYNX].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && !tile->unit->IsGeneral(); }), ev.tile);
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					if (BoardTile* tile = context.GetMap()->GetRandomEmpty(source, target))
						target->unit->MoveToTile(tile, true);
			};
		}
	});

	//Golden Justicar
	effects[SKILL_GOLDEN_JUSTICAR] = Effect(SKILL_GOLDEN_JUSTICAR, KEYWORD_PROVOKE, "{Provoke}|Your other minions with {Provoke} can move two additional spaces");
	effects[SKILL_GOLDEN_JUSTICAR].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GOLDEN_JUSTICAR, ctx.effect, ctx.card); });
	effects[SKILL_GOLDEN_JUSTICAR].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_GOLDEN_JUSTICAR] = Effect(AURA_GOLDEN_JUSTICAR, KEYWORD_NONE, "Golden Justicar");
	effects[AURA_GOLDEN_JUSTICAR].SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->HasKeywords(KEYWORD_PROVOKE); });
	effects[AURA_GOLDEN_JUSTICAR].AddModifier(STAT_MOVE, MODTYPE_BUFF, 2);

	//Golem Metallurgist
	effects[SKILL_GOLEM_METALLURGIST] = Effect(SKILL_GOLEM_METALLURGIST, KEYWORD_NONE, "The first Golem you summon each turn costs 1 less");
	effects[SKILL_GOLEM_METALLURGIST].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
			ctx.game->ApplyAura(AURA_GOLEM_METALLURGIST, ctx.effect, ctx.card); });
	effects[SKILL_GOLEM_METALLURGIST].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_GOLEM_METALLURGIST] = Effect(AURA_GOLEM_METALLURGIST, KEYWORD_NONE, "Golem Metallurgist");
	effects[AURA_GOLEM_METALLURGIST].SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && ctx.CardSharesAuraOwner() && ctx.card->IsUnit() && ctx.card->GetUnit()->tribe == TRIBE_GOLEM
			&& !ctx.game->AnyCastThisTurn([](Card* card) { return card->IsUnit() && card->GetUnit()->tribe == TRIBE_GOLEM; }); });
	effects[AURA_GOLEM_METALLURGIST].AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Golem Vanquisher
	effects[SKILL_GOLEM_VANQUISHER] = Effect(SKILL_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "{Provoke}|Your other Golem minions have {Provoke}");
	effects[SKILL_GOLEM_VANQUISHER].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GOLEM_VANQUISHER, ctx.effect, ctx.card); });
	effects[SKILL_GOLEM_VANQUISHER].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_GOLEM_VANQUISHER] = Effect(AURA_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "Golem Vanquisher|{Provoke}");
	effects[AURA_GOLEM_VANQUISHER].SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.CardSharesAuraOwner() && !ctx.CardIsAuraSource() && !ctx.card->IsGeneral() && ctx.card->GetUnit()->tribe == TRIBE_GOLEM; });

	//Grove Lion
	effects[SKILL_GROVE_LION] = Effect(SKILL_GROVE_LION, KEYWORD_NONE, "While this minion is on the battlefield, your General has {Forcefield}");
	effects[SKILL_GROVE_LION].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
		ctx.game->ApplyAura(AURA_GROVE_LION, ctx.effect, ctx.card); });
	effects[SKILL_GROVE_LION].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_GROVE_LION] = Effect(AURA_GROVE_LION, KEYWORD_FORCEFIELD, "Grove Lion|{Forcefield}");
	effects[AURA_GROVE_LION].SetAuraPredicate([](EffectContext ctx) {
		return ctx.IsOnBoard() && ctx.card == ctx.effect->GetRef()->owner->general; });

	//Healing Mystic
	effects[SKILL_HEALING_MYSTIC] = Effect(SKILL_HEALING_MYSTIC, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 2 Health to anything");
	effects[SKILL_HEALING_MYSTIC].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) { return tile->HasUnit(); }));
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->Heal(context.card, 2);
			};
		}
	});

	//Ironclad
	effects[SKILL_IRONCLAD] = Effect(SKILL_IRONCLAD, KEYWORD_DYING_WISH, "{Dying Wish}: Dispel all enemy minions");
	effects[SKILL_IRONCLAD].RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			for (Unit* unit : ctx.game->GetUnits())
				if (!ctx.SharesOwner(unit) && !unit->IsGeneral())
					unit->Dispel();
	});

	//Jax Truesight
	effects[SKILL_JAX_TRUESIGHT] = Effect(SKILL_JAX_TRUESIGHT, KEYWORD_OPENING_GAMBIT | KEYWORD_RANGED, "{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged} Mini-Jax in each corner");
	effects[SKILL_JAX_TRUESIGHT].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetEmptyCorners())
			if (tile != ev.tile)
				ctx.game->SummonToken(CARD_MINI_JAX, tile, ctx.GetOwner());
	});

	//Jaxi
	effects[SKILL_JAXI] = Effect(SKILL_JAXI, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 1/1 {Ranged} Mini-Jax in a random corner");
	effects[SKILL_JAXI].RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyCorner())
				ctx.game->SummonToken(CARD_MINI_JAX, tile, ctx.GetOwner());
	});

	//Keeper of the Vale
	effects[SKILL_KEEPER_OF_THE_VALE] = Effect(SKILL_KEEPER_OF_THE_VALE, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon a random non-token friendly minion destroyed this game nearby");
	effects[SKILL_KEEPER_OF_THE_VALE].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return !tile->HasUnit(); }), ev.tile);
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (!target->HasUnit()) {
					std::vector<Card*> valid;
					for (Card* destroyed : ctx.game->GetDestroyedUnits())
						if (!destroyed->isToken && destroyed->IsUnit() && ctx.SharesOwner(destroyed))
							valid.push_back(destroyed);
					if (valid.size() > 0)
						ctx.game->SummonToken(valid[rand() % valid.size()]->cardId, target, ctx.GetOwner());
				}
			};
		}
	});

	//Khymera
	effects[SKILL_KHYMERA] = Effect(SKILL_KHYMERA, KEYWORD_NONE, "Whenever this minion takes damage, summon a random token minion nearby");
	effects[SKILL_KHYMERA].RegisterPostTrigger<OnDamageEvent>(
		[](EffectContext ctx, OnDamageEvent& ev) { return ctx.IsCardOnBoard(ev.target); },
		[](EffectContext ctx, OnDamageEvent& ev) {
			if (BoardTile* tile = ctx.GetMap()->GetRandomEmptyNear(ev.target->curTile))
				ctx.game->SummonToken(ctx.game->collections->GetRandomTokenMinion()->cardId, tile, ctx.GetOwner());
	});

	//Lady Locke
	effects[SKILL_LADY_LOCKE] = Effect(SKILL_LADY_LOCKE, KEYWORD_OPENING_GAMBIT | KEYWORD_PROVOKE, "{Provoke}|{Opening Gambit}: Other minions you summon this turn gain +1/+1 and gain {Provoke}");
	effects[SKILL_LADY_LOCKE].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->ApplyAura(AURA_LADY_LOCKE, ctx.effect, ctx.card); });
	effects[AURA_LADY_LOCKE] = Effect(AURA_LADY_LOCKE, KEYWORD_NONE, "Lady Locke|Minions you summon this turn gain +1/+1 and gain {Provoke}");
	effects[AURA_LADY_LOCKE].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsAllied(ev.unit) && !ev.unit->IsGeneral(); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->AddEffect(MODIFIER_LADY_LOCKE, ctx.card); });
	effects[AURA_LADY_LOCKE].RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) { ctx.game->RemoveAura(ctx.effect); });
	effects[MODIFIER_LADY_LOCKE] = Effect(MODIFIER_LADY_LOCKE, KEYWORD_PROVOKE, "Lady Locke|{Provoke}");
	effects[MODIFIER_LADY_LOCKE].AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Lightbender
	effects[SKILL_LIGHTBENDER] = Effect(SKILL_LIGHTBENDER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel ALL spaces around it");
	effects[SKILL_LIGHTBENDER].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		for (BoardTile* tile : ctx.GetMap()->GetAllNear(ev.tile)) {
			tile->SetFeature(TILE_NONE);
			if (tile->HasUnit())
				tile->unit->Dispel();
		}
	});

	//Lux Ignis
	effects[SKILL_LUX_IGNIS] = Effect(SKILL_LUX_IGNIS, KEYWORD_RANGED, "{Ranged}|At the end of your turn, restore 2 Health to all nearby friendly minions");
	effects[SKILL_LUX_IGNIS].RegisterPostTrigger<OnTurnEndEvent>(
		[](EffectContext ctx, OnTurnEndEvent& ev) { return ctx.IsOnBoard() && ctx.IsOwnedBy(ev.player); },
		[](EffectContext ctx, OnTurnEndEvent& ev) {
			for (BoardTile* tile : ctx.GetMap()->GetAllNear(ctx.GetTile()))
				if (tile->HasUnit() && ctx.IsAllied(tile->unit) && !tile->unit->IsGeneral())
					tile->unit->Heal(ctx.card, 2);
	});
	
	//Manaforger
	effects[SKILL_MANAFORGER] = Effect(SKILL_MANAFORGER, KEYWORD_NONE, "The first non-Bloodborn spell you cast each turn costs 1 less");
	effects[SKILL_MANAFORGER].RegisterSelfTrigger<OnAddThisEvent>([](EffectContext ctx, OnAddThisEvent& ev) {
			ctx.game->ApplyAura(AURA_MANAFORGER, ctx.effect, ctx.card); });
	effects[SKILL_MANAFORGER].RegisterSelfTrigger<OnRemoveThisEvent>([](EffectContext ctx, OnRemoveThisEvent& ev) {
		ctx.game->RemoveAurasFromSource(ctx.effect); });
	effects[AURA_MANAFORGER] = Effect(AURA_MANAFORGER, KEYWORD_NONE, "Manaforger");
	effects[AURA_MANAFORGER].SetAuraPredicate([](EffectContext ctx) {
		return ctx.GetOwner()->HasCardInHand(ctx.card) && ctx.CardSharesAuraOwner() && ctx.card->IsSpell()
			&& !ctx.game->AnyCastThisTurn([](Card* card) { return card->IsSpell(); }); });
	effects[AURA_MANAFORGER].AddModifier(STAT_COST, MODTYPE_BUFF, -1);

	//Maw
	effects[SKILL_MAW] = Effect(SKILL_MAW, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to a nearby enemy minion");
	effects[SKILL_MAW].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		ctx.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasUnit() && tile->unit->IsEnemy() && !tile->unit->IsGeneral(); }), ev.tile);
		if (ctx.game->HasSelectable()) {
			ctx.game->callback = EffectCallback(ctx, ev.tile);
			ctx.game->callback.Callback = [](EffectContext ctx, BoardTile* source, BoardTile* target) {
				if (target->HasUnit())
					target->unit->DealDamage(ctx.card, 2);
			};
		}
	});

	//Mindwarper
	effects[SKILL_MINDWARPER] = Effect(SKILL_MINDWARPER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Gain a copy of a random spell from your opponent's action bar");
	effects[SKILL_MINDWARPER].RegisterSelfTrigger<OnPreCastThisEvent>([](EffectContext ctx, OnPreCastThisEvent& ev) {
		std::vector<Card*> valid;
		for (Card* card : ctx.GetOpponent()->hand)
			if (card->IsSpell())
				valid.push_back(card);
		if (valid.size() > 0)
			ctx.GetOwner()->AddNewToHand(valid[rand() % valid.size()]->original);
	});

	//Mirkblood Devourer
	effects[SKILL_MIRKBLOOD_DEVOURER] = Effect(SKILL_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "Friendly minions summoned nearby this minion gain +1/+1");
	effects[SKILL_MIRKBLOOD_DEVOURER].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ctx.IsAllied(ev.unit) && ctx.GetTile()->IsNear(ev.unit->curTile); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->AddEffect(MODIFIER_MIRKBLOOD_DEVOURER, ctx.card); });
	effects[MODIFIER_MIRKBLOOD_DEVOURER] = Effect(MODIFIER_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "Mirkblood Devourer");
	effects[MODIFIER_MIRKBLOOD_DEVOURER].AddModifier(STAT_ATK, MODTYPE_BUFF, 1).AddModifier(STAT_HP, MODTYPE_BUFF, 1);

	//Mogwai
	effects[SKILL_MOGWAI] = Effect(SKILL_MOGWAI, KEYWORD_NONE, "After this moves, draw a card");
	effects[SKILL_MOGWAI].RegisterPostTrigger<OnMoveEvent>(
		[](EffectContext ctx, OnMoveEvent& ev) { return ctx.IsCardOnBoard(ev.unit) && !ev.wasTeleported; },
		[](EffectContext ctx, OnMoveEvent& ev) { ctx.GetOwner()->Draw(); });

	//Necroseer
	effects[SKILL_NECROSEER] = Effect(SKILL_NECROSEER, KEYWORD_DYING_WISH, "{Dying Wish}: Draw a card");
	effects[SKILL_NECROSEER].RegisterPostTrigger<OnDeathEvent>(
		[](EffectContext ctx, OnDeathEvent& ev) { return ctx.IsCard(ev.unit); },
		[](EffectContext ctx, OnDeathEvent& ev) { ctx.GetOwner()->Draw(); });

	//Night Watcher
	effects[SKILL_NIGHT_WATCHER] = Effect(SKILL_NIGHT_WATCHER, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever ANY player summons a minion with {Rush}, exhaust it");
	effects[SKILL_NIGHT_WATCHER].RegisterPostTrigger<OnSummonEvent>(
		[](EffectContext ctx, OnSummonEvent& ev) { return ctx.IsOnBoard() && ev.unit->HasKeywords(KEYWORD_RUSH); },
		[](EffectContext ctx, OnSummonEvent& ev) { ev.unit->Exhaust(); });

}