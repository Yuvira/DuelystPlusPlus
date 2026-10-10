//Include
#include "game.h"

#pragma region Constructor

//Card list constructor
Collections::Collections() {

#pragma region Effects

	//Keyword skills
	effects[SKILL_CELERITY] = Effect(SKILL_CELERITY, KEYWORD_CELERITY, "{Celerity}");
	effects[SKILL_FLYING] = Effect(SKILL_FLYING, KEYWORD_FLYING, "{Flying}");
	effects[SKILL_FORCEFIELD] = Effect(SKILL_FORCEFIELD, KEYWORD_FORCEFIELD, "{Forcefield}");
	effects[SKILL_FRENZY] = Effect(SKILL_FRENZY, KEYWORD_FRENZY, "{Frenzy}");
	effects[SKILL_PROVOKE] = Effect(SKILL_PROVOKE, KEYWORD_PROVOKE, "{Provoke}");
	effects[SKILL_RANGED] = Effect(SKILL_RANGED, KEYWORD_RANGED, "{Ranged}");
	effects[SKILL_RUSH] = Effect(SKILL_RUSH, KEYWORD_RUSH, "{Rush}");

	//Dispelled
	effects[EFFECT_DISPELLED] = Effect(EFFECT_DISPELLED, KEYWORD_NONE, "{Dispelled}");

#pragma region Minions

	//Abjudicator
	effects[SKILL_ABJUDICATOR] = Effect(SKILL_ABJUDICATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Lower the cost of all spells in your action bar by 1");
	effects[SKILL_ABJUDICATOR].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (Card* card : context.card->owner->hand)
			if (card->IsSpell())
				card->AddEffect(EFFECT_ABJUDICATOR, context.effect);
	};
	effects[EFFECT_ABJUDICATOR] = Effect(EFFECT_ABJUDICATOR, KEYWORD_NONE, "Abjudicator");
	effects[EFFECT_ABJUDICATOR].costBuff = -1;

	//Aethermaster
	effects[SKILL_AETHERMASTER] = Effect(SKILL_AETHERMASTER, KEYWORD_NONE, "You may replace an additional card each turn");
	effects[SKILL_AETHERMASTER].OnAddThis = [](EffectContext context) {
		++context.card->owner->maxReplaces;
		++context.card->owner->replaces;
	};
	effects[SKILL_AETHERMASTER].OnRemoveThis = [](EffectContext context) {
		--context.card->owner->maxReplaces;
		context.card->owner->replaces = min(context.card->owner->replaces, context.card->owner->maxReplaces);
	};

	//Alcuin Loremaster
	effects[SKILL_ALCUIN_LOREMASTER] = Effect(SKILL_ALCUIN_LOREMASTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Put a copy of the most recently cast spell into your action bar");
	effects[SKILL_ALCUIN_LOREMASTER].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (Spell* spell : context.game->spellHistory | std::views::reverse) {
			if (spell->IsSpell()) {
				context.card->owner->AddNewToHand(spell->original);
				break;
			}
		}
	};

	//Araki Headhunter
	effects[SKILL_ARAKI_HEADHUNTER] = Effect(SKILL_ARAKI_HEADHUNTER, KEYWORD_NONE, "Whenever you summon a minion with {Opening Gambit} from your action bar, gain +2 Attack");
	effects[SKILL_ARAKI_HEADHUNTER].OnSummon = [](EffectContext context, Minion* source, bool fromActionBar) {
		if (context.IsOnBoard() && context.IsAllied(source) && source->HasKeywords(KEYWORD_OPENING_GAMBIT) && fromActionBar)
			context.card->AddEffect(EFFECT_ARAKI_HEADHUNTER, nullptr);
	};
	effects[EFFECT_ARAKI_HEADHUNTER] = Effect(EFFECT_ARAKI_HEADHUNTER, KEYWORD_NONE, "Headhunter");
	effects[EFFECT_ARAKI_HEADHUNTER].atkBuff = 2;

	//Archon Spellbinder
	effects[SKILL_ARCHON_SPELLBINDER] = Effect(SKILL_ARCHON_SPELLBINDER, KEYWORD_NONE, "Your opponent's non-Bloodborn spells cost 1 more to cast");
	effects[SKILL_ARCHON_SPELLBINDER].OnAddThis = [](EffectContext context) {
		for (Card* card : context.card->owner->opponent->hand)
			if (card->IsSpell())
				card->AddEffect(EFFECT_ARCHON_SPELLBINDER, context.effect);
		for (Card* card : context.card->owner->opponent->deck)
			if (card->IsSpell())
				card->AddEffect(EFFECT_ARCHON_SPELLBINDER, context.effect);
	};
	effects[SKILL_ARCHON_SPELLBINDER].OnRemoveThis = [](EffectContext context) {
		for (Card* card : context.card->owner->opponent->hand)
			card->RemoveEffectsFromSource(context.effect);
		for (Card* card : context.card->owner->opponent->deck)
			card->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_ARCHON_SPELLBINDER].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsOnBoard() && !fromDeck && !context.SharesOwner(card) && card->IsSpell())
			card->AddEffect(EFFECT_ARCHON_SPELLBINDER, context.effect);
	};
	effects[EFFECT_ARCHON_SPELLBINDER] = Effect(EFFECT_ARCHON_SPELLBINDER, KEYWORD_NONE, "{Spellbound}");
	effects[EFFECT_ARCHON_SPELLBINDER].costBuff = 1;

	//Arrow Whistler
	effects[SKILL_ARROW_WHISTLER] = Effect(SKILL_ARROW_WHISTLER, KEYWORD_RANGED, "{Ranged}|Your other minions with {Ranged} have +1 Attack");
	effects[SKILL_ARROW_WHISTLER].OnAddThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			if (context.IsAllied(minion) && !minion->IsGeneral() && minion->HasKeywords(KEYWORD_RANGED))
				minion->AddEffect(EFFECT_ARROW_WHISTLER, context.effect);
	};
	effects[SKILL_ARROW_WHISTLER].OnRemoveThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			minion->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_ARROW_WHISTLER].OnSummon = [](EffectContext context, Minion* minion, bool fromActionBar) {
		if (context.IsOnBoard() && context.IsAllied(minion) && !minion->IsGeneral() && minion->HasKeywords(KEYWORD_RANGED))
			minion->AddEffect(EFFECT_ARROW_WHISTLER, context.effect);
	};
	effects[SKILL_ARROW_WHISTLER].OnEffectsChanged = [](EffectContext context, Card* card) {
		if (context.BothOnBoard(card) && context.IsAllied(card) && !card->IsGeneral()) {
			if (card->GetMinion()->HasKeywords(KEYWORD_RANGED))
				card->AddEffect(EFFECT_ARROW_WHISTLER, context.effect);
			else
				card->RemoveEffectsFromSource(context.effect);
		}
	};
	effects[EFFECT_ARROW_WHISTLER] = Effect(EFFECT_ARROW_WHISTLER, KEYWORD_NONE, "{Whistling Arrows}");
	effects[EFFECT_ARROW_WHISTLER].atkBuff = 1;

	//Ash Mephyt
	effects[SKILL_ASH_MEPHYT] = Effect(SKILL_ASH_MEPHYT, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon two copies of this minion on random spaces");
	effects[SKILL_ASH_MEPHYT].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		if (context.card->IsMinion()) {
			for (int i = 0; i < 2; ++i) {
				BoardTile* newTile = context.game->map.GetRandomEmpty(tile);
				if (newTile != nullptr)
					context.game->SummonToken(context.card->cardId, newTile, context.card->owner);
			}
		}
	};

	//Astral Crusader
	effects[SKILL_ASTRAL_CRUSADER] = Effect(SKILL_ASTRAL_CRUSADER, KEYWORD_NONE, "Whenever you replace this card, it costs 3 less and gains +3/+3");
	effects[SKILL_ASTRAL_CRUSADER].OnReplace = [](EffectContext context, Card* card, bool& sendToDeck) {
		if (context.IsCard(card))
			card->AddEffect(EFFECT_ASTRAL_CRUSADER, nullptr);
	};
	effects[EFFECT_ASTRAL_CRUSADER] = Effect(EFFECT_ASTRAL_CRUSADER, KEYWORD_NONE, "{Astral Crusader}");
	effects[EFFECT_ASTRAL_CRUSADER].costBuff = -3;
	effects[EFFECT_ASTRAL_CRUSADER].atkBuff = 3;
	effects[EFFECT_ASTRAL_CRUSADER].hpBuff = 3;

	//Azure Herald
	effects[SKILL_AZURE_HERALD] = Effect(SKILL_AZURE_HERALD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 3 Health to your General");
	effects[SKILL_AZURE_HERALD].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.card->owner->general->Heal(context.card, 3);
	};

	//Azure Horn Shaman
	effects[SKILL_AZURE_HORN_SHAMAN] = Effect(SKILL_AZURE_HORN_SHAMAN, KEYWORD_DYING_WISH, "{Dying Wish}: Give +4 Health to friendly minions around it");
	effects[SKILL_AZURE_HORN_SHAMAN].OnDeath = [](EffectContext context, Minion* minion) {
		if (context.IsCard(minion))
			for (BoardTile* tile : context.game->map.GetAllNear(minion->curTile))
				if (tile->HasMinion() && context.SharesOwner(tile->minion) && !tile->minion->IsGeneral())
					tile->minion->AddEffect(EFFECT_AZURE_HORN_SHAMAN, nullptr);
	};
	effects[EFFECT_AZURE_HORN_SHAMAN] = Effect(EFFECT_AZURE_HORN_SHAMAN, KEYWORD_NONE, "{Azure Horn Shaman}");
	effects[EFFECT_AZURE_HORN_SHAMAN].hpBuff = 4;

	//Bastion
	effects[SKILL_BASTION] = Effect(SKILL_BASTION, KEYWORD_NONE, "At the end of your turn, give other friendly minions +1 Health");
	effects[SKILL_BASTION].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player))
			for (Minion* minion : context.game->minions)
				if (context.IsAllied(minion) && !minion->IsGeneral())
					minion->AddEffect(EFFECT_BASTION, nullptr);
	};
	effects[EFFECT_BASTION] = Effect(EFFECT_BASTION, KEYWORD_NONE, "{Bastion}");
	effects[EFFECT_BASTION].hpBuff = 1;

	//Black Locust
	effects[SKILL_BLACK_LOCUST] = Effect(SKILL_BLACK_LOCUST, KEYWORD_FLYING, "{Flying}|After this minion moves, summon a Black Locust nearby");
	effects[SKILL_BLACK_LOCUST].OnMove = [](EffectContext context, Minion* minion, bool byEffect) {
		if (context.IsCardOnBoard(minion) && !byEffect) {
			BoardTile* tile = context.game->map.GetRandomEmptyNear(minion->curTile);
			if (tile != nullptr)
				context.game->SummonToken(CARD_BLACK_LOCUST, tile, context.card->owner);
		}
	};

	//Blaze Hound
	effects[SKILL_BLAZE_HOUND] = Effect(SKILL_BLAZE_HOUND, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Both players draw a card");
	effects[SKILL_BLAZE_HOUND].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->players[0].Draw();
		context.game->players[1].Draw();
	};

	//Blistering Skorn
	effects[SKILL_BLISTERING_SKORN] = Effect(SKILL_BLISTERING_SKORN, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to everything (including itself)");
	effects[SKILL_BLISTERING_SKORN].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (Minion* minion : context.game->minions)
			minion->DealDamage(context.card, 1);
		if (context.card->IsMinion())
			context.card->GetMinion()->DealDamage(context.card, 1);
	};

	//Blood Taura
	effects[SKILL_BLOOD_TAURA] = Effect(SKILL_BLOOD_TAURA, KEYWORD_PROVOKE, "{Provoke}|This minion's cost is equal to your General's Health");
	effects[SKILL_BLOOD_TAURA].fixedCost = 25;
	effects[SKILL_BLOOD_TAURA].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (target == context.card->owner->general)
			context.effect->fixedCost = max(target->hp, 0);
		context.card->UpdateStatBuffs();
	};
	effects[SKILL_BLOOD_TAURA].OnHealed = [](EffectContext context, Card* source, Minion* target, int heal) {
		if (target == context.card->owner->general)
			context.effect->fixedCost = max(target->hp, 0);
		context.card->UpdateStatBuffs();
	};
	effects[SKILL_BLOOD_TAURA].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsCard(card) && !fromDeck) {
			context.effect->fixedCost = max(context.card->owner->general->hp, 0);
			context.card->UpdateStatBuffs();
		}
	};

	//Bloodtear Alchemist
	effects[SKILL_BLOODTEAR_ALCHEMIST] = Effect(SKILL_BLOODTEAR_ALCHEMIST, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 1 damage to an enemy");
	effects[SKILL_BLOODTEAR_ALCHEMIST].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasMinion() && tile->minion->IsEnemy();
		}));
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasMinion())
					target->minion->DealDamage(context.card, 1);
			};
		}
	};

	//Bluetip Scorpion
	effects[SKILL_BLUETIP_SCORPION] = Effect(SKILL_BLUETIP_SCORPION, KEYWORD_NONE, "Deals double damage to minions");
	effects[SKILL_BLUETIP_SCORPION].OnWouldDealDamage = [](EffectContext context, Card* source, Minion* target, int& damage) {
		if (context.IsCard(source) && !target->IsGeneral())
			damage *= 2;
	};

	//Bonereaper
	effects[SKILL_BONEREAPER] = Effect(SKILL_BONEREAPER, KEYWORD_PROVOKE, "{Provoke}|At the end of your turn, deal 2 damage to all nearby enemy minions");
	effects[SKILL_BONEREAPER].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player)) {
			for (BoardTile* tile : context.game->map.GetAllNear(context.card->GetMinion()->curTile))
				if (tile->HasMinion() && !context.SharesOwner(tile->minion) && !tile->minion->IsGeneral())
					tile->minion->DealDamage(context.card, 2);
		}
	};

	//Captain Hank Hart
	effects[SKILL_CAPTAIN_HANK_HART] = Effect(SKILL_CAPTAIN_HANK_HART, KEYWORD_RANGED, "{Ranged}|Whenever this deals damage, restore that much Health to it");
	effects[SKILL_CAPTAIN_HANK_HART].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (context.IsCardOnBoard(source) && context.card->GetMinion()->hp > 0)
			context.card->GetMinion()->Heal(context.card, damage);
	};

	//Chakkram
	effects[SKILL_CHAKKRAM] = Effect(SKILL_CHAKKRAM, KEYWORD_NONE, "Costs 2 less if your General took damage on your opponent's last turn");
	effects[SKILL_CHAKKRAM].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (!context.IsOnBoard() && !context.IsOwnerTurn() && target == context.card->owner->general)
			context.card->AddEffect(EFFECT_CHAKKRAM, context.effect);
	};
	effects[SKILL_CHAKKRAM].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOwnedBy(player))
			context.card->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_CHAKKRAM].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (context.IsCard(minion))
			context.card->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_CHAKKRAM].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsCard(card) && !fromDeck) {
			for (Event* event : context.game->eventManager.GetMostRecentTurnEvents(context.card->owner->opponent)) {
				if (event->type == EVENT_DAMAGE_DEALT && event->target == context.card->owner->general) {
					context.card->AddEffect(EFFECT_CHAKKRAM, context.effect);
					break;
				}
			}
		}
	};
	effects[EFFECT_CHAKKRAM] = Effect(EFFECT_CHAKKRAM, KEYWORD_NONE, "{Chakkram}");
	effects[EFFECT_CHAKKRAM].costBuff = -2;

	//Chaos Elemental
	effects[SKILL_CHAOS_ELEMENTAL] = Effect(SKILL_CHAOS_ELEMENTAL, KEYWORD_NONE, "Whenever this minion takes damage, it randomly teleports");
	effects[SKILL_CHAOS_ELEMENTAL].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (context.IsCard(target)) {
			BoardTile* tile = context.game->map.GetRandomEmpty(target->curTile);
			if (tile != nullptr)
				target->MoveToTile(tile, true);
		}
	};

	//Crimson Oculus
	effects[SKILL_CRIMSON_OCULUS] = Effect(SKILL_CRIMSON_OCULUS, KEYWORD_NONE, "Whenever opponent summons a minion, this minion gets +1/+1");
	effects[SKILL_CRIMSON_OCULUS].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (context.IsOnBoard() && !context.SharesOwner(minion))
			context.card->AddEffect(EFFECT_CRIMSON_OCULUS, nullptr);
	};
	effects[EFFECT_CRIMSON_OCULUS] = Effect(EFFECT_CRIMSON_OCULUS, KEYWORD_NONE, "{Crimson Oculus}");
	effects[EFFECT_CRIMSON_OCULUS].atkBuff = 1;
	effects[EFFECT_CRIMSON_OCULUS].hpBuff = 1;

	//Crossbones
	effects[SKILL_CROSSBONES] = Effect(SKILL_CROSSBONES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Destroy an enemy minion with Ranged");
	effects[SKILL_CROSSBONES].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasMinion() && tile->minion->IsEnemy() && !tile->minion->IsGeneral() && tile->minion->HasKeywords(KEYWORD_RANGED);
		}));
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasMinion())
					target->minion->Destroy(context.card);
			};
		}
	};

	//Dancing Blades
	effects[SKILL_DANCING_BLADES] = Effect(SKILL_DANCING_BLADES, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to ANY minion in front of this");
	effects[SKILL_DANCING_BLADES].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		int x = tile->pos.x;
		&context.game->players[0] == context.card->owner ? ++x : --x;
		BoardTile* target = context.game->map.GetTile(x, tile->pos.y);
		if (target != nullptr && target->HasMinion() && !target->minion->IsGeneral())
			target->minion->DealDamage(context.card, 3);
	};

	//Dark Nemesis
	effects[SKILL_DARK_NEMESIS] = Effect(SKILL_DARK_NEMESIS, KEYWORD_NONE, "At the start of your turn, deal 4 damage to the enemy General and this minion gains +4 Attack");
	effects[SKILL_DARK_NEMESIS].OnTurnStart = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player)) {
			context.card->owner->opponent->general->DealDamage(context.card, 4);
			context.card->AddEffect(EFFECT_DARK_NEMESIS, nullptr);
		}
	};
	effects[EFFECT_DARK_NEMESIS] = Effect(EFFECT_DARK_NEMESIS, KEYWORD_NONE, "{Dark Nemesis}");
	effects[EFFECT_DARK_NEMESIS].atkBuff = 4;

	//Day Watcher
	effects[SKILL_DAY_WATCHER] = Effect(SKILL_DAY_WATCHER, KEYWORD_NONE, "Whenever a friendly minion attacks, restore 1 Health to your General");
	effects[SKILL_DAY_WATCHER].OnAttack = [](EffectContext context, Minion* source, Minion* target, bool counter) {
		if (context.IsOnBoard() && context.SharesOwner(source) && !source->IsGeneral() && !counter)
			context.card->owner->general->Heal(context.card, 1);
	};

	//Deathblighter
	effects[SKILL_DEATHBLIGHTER] = Effect(SKILL_DEATHBLIGHTER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to all enemy minions around it");
	effects[SKILL_DEATHBLIGHTER].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (BoardTile* tile : context.game->map.GetAllNear(tile))
			if (tile->HasMinion() && !context.SharesOwner(tile->minion) && !tile->minion->IsGeneral())
				tile->minion->DealDamage(context.card, 3);
	};

	//Decimus
	effects[SKILL_DECIMUS] = Effect(SKILL_DECIMUS, KEYWORD_NONE, "Whenever your opponent draws a card, deal 2 damage to the enemy General");
	effects[SKILL_DECIMUS].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsOnBoard() && !context.SharesOwner(card) && fromDeck)
			context.card->owner->opponent->general->DealDamage(context.card, 2);
	};

	//Dioltas
	effects[SKILL_DIOLTAS] = Effect(SKILL_DIOLTAS, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 0/8 Tombstone minion with Provoke near your General");
	effects[SKILL_DIOLTAS].OnDeath = [](EffectContext context, Minion* minion) {
		if (context.IsCard(minion)) {
			BoardTile* tile = context.game->map.GetRandomEmptyNear(context.card->owner->general->curTile);
			if (tile != nullptr)
				context.game->SummonToken(CARD_TOMBSTONE, tile, context.card->owner);
		}
	};

	//Dreamgazer
	effects[SKILL_DREAMGAZER] = Effect(SKILL_DREAMGAZER, KEYWORD_NONE, "When you replace this card, summon it nearby. Your General takes 2 damage");
	effects[SKILL_DREAMGAZER].OnReplace = [](EffectContext context, Card* card, bool& sendToDeck) {
		if (context.IsCard(card) && context.card->IsMinion()) {
			BoardTile* tile = context.game->map.GetRandomEmptyNear(context.card->owner->general->curTile);
			if (tile != nullptr) {
				context.game->Summon(context.card, tile, false);
				context.card->owner->general->DealDamage(context.card, 2);
				sendToDeck = false;
			}
		}
	};

	//Dust Wailer
	effects[SKILL_DUST_WAILER] = Effect(SKILL_DUST_WAILER, KEYWORD_FLYING | KEYWORD_OPENING_GAMBIT, "{Flying}|{Opening Gambit}: Deal 3 damage to all enemies in front of this");
	effects[SKILL_DUST_WAILER].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		int x = tile->pos.x;
		while (true) {
			&context.game->players[0] == context.card->owner ? ++x : --x;
			BoardTile* target = context.game->map.GetTile(x, tile->pos.y);
			if (target == nullptr)
				break;
			if (target->HasMinion() && !context.SharesOwner(target->minion))
				target->minion->DealDamage(context.card, 3);
		}
	};

	//Eclipse
	effects[SKILL_ECLIPSE] = Effect(SKILL_ECLIPSE, KEYWORD_NONE, "Whenever this minion takes damage, it deals that much damage to the enemy General");
	effects[SKILL_ECLIPSE].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (context.IsCardOnBoard(target))
			context.card->owner->opponent->general->DealDamage(context.card, damage);
	};

	//Emerald Rejuvenator
	effects[SKILL_EMERALD_REJUVENATOR] = Effect(SKILL_EMERALD_REJUVENATOR, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 4 Health to BOTH Generals");
	effects[SKILL_EMERALD_REJUVENATOR].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.card->owner->general->Heal(context.card, 4);
		context.card->owner->opponent->general->Heal(context.card, 4);
	};

	//Envybaer
	effects[SKILL_ENVYBAER] = Effect(SKILL_ENVYBAER, KEYWORD_NONE, "Whenever this minion damages an enemy, teleport that enemy to a random corner");
	effects[SKILL_ENVYBAER].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (context.IsCardOnBoard(source) && !context.SharesOwner(target)) {
			BoardTile* tile = context.game->map.GetRandomEmptyCorner();
			if (tile != nullptr)
				target->MoveToTile(tile, true);
		}
	};

	//Ephemeral Shroud
	effects[SKILL_EPHEMERAL_SHROUD] = Effect(SKILL_EPHEMERAL_SHROUD, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel 1 nearby space");
	effects[SKILL_EPHEMERAL_SHROUD].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, nullptr), tile);
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				target->SetFeature(TILE_NONE);
				if (target->HasMinion())
					target->minion->Dispel();
			};
		}
	};

	//E'Xun
	effects[SKILL_EXUN] = Effect(SKILL_EXUN, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever this minion attacks or is attacked, draw a card");
	effects[SKILL_EXUN].OnAttack = [](EffectContext context, Minion* source, Minion* target, bool counter) {
		if ((context.IsCardOnBoard(source) || context.IsCardOnBoard(target)) && !counter)
			context.card->owner->Draw();
	};

	//Facestriker
	effects[SKILL_FACESTRIKER] = Effect(SKILL_FACESTRIKER, KEYWORD_NONE, "Deals double damage to Generals");
	effects[SKILL_FACESTRIKER].OnWouldDealDamage = [](EffectContext context, Card* source, Minion* target, int& damage) {
		if (context.IsCard(source) && target->IsGeneral())
			damage *= 2;
	};

	//Firestarter
	effects[SKILL_FIRESTARTER] = Effect(SKILL_FIRESTARTER, KEYWORD_NONE, "Whenever you cast a spell, summon a 1/1 Spellspark with Rush on a random nearby space");
	effects[SKILL_FIRESTARTER].OnCast = [](EffectContext context, Card* card, BoardTile* tile) {
		if (context.IsOnBoard() && card->IsSpell() && context.SharesOwner(card)) {
			BoardTile* tile = context.game->map.GetRandomEmptyNear(context.card->GetMinion()->curTile);
			if (tile != nullptr)
				context.game->SummonToken(CARD_SPELLSPARK, tile, context.card->owner);
		}
	};

	//First Sword of Akrane
	effects[SKILL_FIRST_SWORD_OF_AKRANE] = Effect(SKILL_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "Your other minions have +1 Attack");
	effects[SKILL_FIRST_SWORD_OF_AKRANE].OnAddThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			if (context.IsAllied(minion) && !minion->IsGeneral())
				minion->AddEffect(EFFECT_FIRST_SWORD_OF_AKRANE, context.effect);
	};
	effects[SKILL_FIRST_SWORD_OF_AKRANE].OnRemoveThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			minion->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_FIRST_SWORD_OF_AKRANE].OnSummon = [](EffectContext context, Minion* minion, bool fromActionBar) {
		if (context.IsOnBoard() && context.IsAllied(minion))
			minion->AddEffect(EFFECT_FIRST_SWORD_OF_AKRANE, context.effect);
	};
	effects[EFFECT_FIRST_SWORD_OF_AKRANE] = Effect(EFFECT_FIRST_SWORD_OF_AKRANE, KEYWORD_NONE, "{Akrane's First Sword}");
	effects[EFFECT_FIRST_SWORD_OF_AKRANE].atkBuff = 1;

	//Flameblood Warlock
	effects[SKILL_FLAMEBLOOD_WARLOCK] = Effect(SKILL_FLAMEBLOOD_WARLOCK, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 3 damage to BOTH Generals");
	effects[SKILL_FLAMEBLOOD_WARLOCK].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.card->owner->general->DealDamage(context.card, 3);
		context.card->owner->opponent->general->DealDamage(context.card, 3);
	};

	//Frostbone Naga
	effects[SKILL_FROSTBONE_NAGA] = Effect(SKILL_FROSTBONE_NAGA, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to everything around it");
	effects[SKILL_FROSTBONE_NAGA].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (BoardTile* tile : context.game->map.GetAllNear(tile))
			if (tile->HasMinion())
				tile->minion->DealDamage(context.card, 2);
	};

	//Ghost Lynx
	effects[SKILL_GHOST_LYNX] = Effect(SKILL_GHOST_LYNX, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Teleport a nearby minion to a random space");
	effects[SKILL_GHOST_LYNX].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasMinion() && !tile->minion->IsGeneral();
		}), tile);
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasMinion()) {
					BoardTile* tile = context.game->map.GetRandomEmpty(source, target);
					if (tile != nullptr)
						target->minion->MoveToTile(tile, true);
				}
			};
		}
	};

	//Golden Justicar
	effects[SKILL_GOLDEN_JUSTICAR] = Effect(SKILL_GOLDEN_JUSTICAR, KEYWORD_PROVOKE, "{Provoke}|Your other minions with {Provoke} can move two additional spaces");
	effects[SKILL_GOLDEN_JUSTICAR].OnAddThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			if (context.IsAllied(minion) && !minion->IsGeneral() && minion->HasKeywords(KEYWORD_PROVOKE))
				minion->AddEffect(EFFECT_GOLDEN_JUSTICAR, context.effect);
	};
	effects[SKILL_GOLDEN_JUSTICAR].OnRemoveThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			minion->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_GOLDEN_JUSTICAR].OnSummon = [](EffectContext context, Minion* minion, bool fromActionBar) {
		if (context.IsOnBoard() && context.IsAllied(minion) && minion->HasKeywords(KEYWORD_PROVOKE))
			minion->AddEffect(EFFECT_GOLDEN_JUSTICAR, context.effect);
	};
	effects[SKILL_GOLDEN_JUSTICAR].OnEffectsChanged = [](EffectContext context, Card* card) {
		if (context.BothOnBoard(card) && context.IsAllied(card) && !card->IsGeneral()) {
			if (card->GetMinion()->HasKeywords(KEYWORD_PROVOKE))
				card->AddEffect(EFFECT_GOLDEN_JUSTICAR, context.effect);
			else
				card->RemoveEffectsFromSource(context.effect);
		}
	};
	effects[EFFECT_GOLDEN_JUSTICAR] = Effect(EFFECT_GOLDEN_JUSTICAR, KEYWORD_NONE, "{Golden Justicar}");
	effects[EFFECT_GOLDEN_JUSTICAR].moveBuff = 2;

	//Golem Metallurgist
	effects[SKILL_GOLEM_METALLURGIST] = Effect(SKILL_GOLEM_METALLURGIST, KEYWORD_NONE, "The first Golem you summon each turn costs 1 less");
	effects[SKILL_GOLEM_METALLURGIST].OnAddThis = [](EffectContext context) {
		for (Card* c : context.game->castThisTurn)
			if (c->IsMinion() && c->GetMinion()->tribe == TRIBE_GOLEM)
				return;
		if (context.effect->ApplyEffect)
			context.effect->ApplyEffect(context);
	};
	effects[SKILL_GOLEM_METALLURGIST].OnRemoveThis = [](EffectContext context) {
		if (context.effect->RemoveEffect)
			context.effect->RemoveEffect(context);
	};
	effects[SKILL_GOLEM_METALLURGIST].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (actionBar && context.SharesOwner(minion) && minion->tribe == TRIBE_GOLEM) {
			minion->RemoveEffectsFromSource(context.effect);
			if (context.effect->RemoveEffect)
				context.effect->RemoveEffect(context);
		}
	};
	effects[SKILL_GOLEM_METALLURGIST].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsOnBoard() && !fromDeck && context.SharesOwner(card) && card->IsMinion() && card->GetMinion()->tribe == TRIBE_GOLEM) {
			for (Card* c : context.game->castThisTurn)
				if (c->IsMinion() && c->GetMinion()->tribe == TRIBE_GOLEM)
					return;
			card->AddEffect(EFFECT_GOLEM_METALLURGIST, context.effect);
		}
	};
	effects[SKILL_GOLEM_METALLURGIST].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player))
			if (context.effect->ApplyEffect)
				context.effect->ApplyEffect(context);
	};
	effects[SKILL_GOLEM_METALLURGIST].ApplyEffect = [](EffectContext context) {
		for (Card* card : context.card->owner->hand)
			if (card->IsMinion() && card->GetMinion()->tribe == TRIBE_GOLEM)
				card->AddEffect(EFFECT_GOLEM_METALLURGIST, context.effect);
		for (Card* card : context.card->owner->deck)
			if (card->IsMinion() && card->GetMinion()->tribe == TRIBE_GOLEM)
				card->AddEffect(EFFECT_GOLEM_METALLURGIST, context.effect);
	};
	effects[SKILL_GOLEM_METALLURGIST].RemoveEffect = [](EffectContext context) {
		for (Card* card : context.card->owner->hand)
			card->RemoveEffectsFromSource(context.effect);
		for (Card* card : context.card->owner->deck)
			card->RemoveEffectsFromSource(context.effect);
	};
	effects[EFFECT_GOLEM_METALLURGIST] = Effect(EFFECT_GOLEM_METALLURGIST, KEYWORD_NONE, "{Metallurgy}");
	effects[EFFECT_GOLEM_METALLURGIST].costBuff = -1;

	//Golem Vanquisher
	effects[SKILL_GOLEM_VANQUISHER] = Effect(SKILL_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "{Provoke}|Your other Golem minions have {Provoke}");
	effects[SKILL_GOLEM_VANQUISHER].OnAddThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			if (context.IsAllied(minion) && minion->tribe == TRIBE_GOLEM)
				minion->AddEffect(EFFECT_GOLEM_VANQUISHER, context.effect);
	};
	effects[SKILL_GOLEM_VANQUISHER].OnRemoveThis = [](EffectContext context) {
		for (Minion* minion : context.game->minions)
			minion->RemoveEffectsFromSource(context.effect);
	};
	effects[SKILL_GOLEM_VANQUISHER].OnSummon = [](EffectContext context, Minion* minion, bool fromActionBar) {
		if (context.card->IsOnBoard() && context.IsAllied(minion) && minion->tribe == TRIBE_GOLEM)
			minion->AddEffect(EFFECT_GOLEM_VANQUISHER, context.effect);
	};
	effects[SKILL_GOLEM_VANQUISHER].OnEffectsChanged = [](EffectContext context, Card* card) {
		if (context.BothOnBoard(card) && context.IsAllied(card)) {
			if (card->GetMinion()->tribe == TRIBE_GOLEM)
				card->AddEffect(EFFECT_GOLEM_VANQUISHER, context.effect);
			else
				card->RemoveEffectsFromSource(context.effect);
		}
	};
	effects[EFFECT_GOLEM_VANQUISHER] = Effect(EFFECT_GOLEM_VANQUISHER, KEYWORD_PROVOKE, "{Golem Vanquisher}|{Provoke}");

	//Grove Lion
	effects[SKILL_GROVE_LION] = Effect(SKILL_GROVE_LION, KEYWORD_NONE, "While this minion is on the battlefield, your General has {Forcefield}");
	effects[SKILL_GROVE_LION].OnAddThis = [](EffectContext context) {
		context.card->owner->general->AddEffect(EFFECT_GROVE_LION, context.effect);
	};
	effects[SKILL_GROVE_LION].OnRemoveThis = [](EffectContext context) {
		context.card->owner->general->RemoveEffectsFromSource(context.effect);
	};
	effects[EFFECT_GROVE_LION] = Effect(EFFECT_GROVE_LION, KEYWORD_FORCEFIELD, "{Grove Lion}|{Forcefield}");

	//Healing Mystic
	effects[SKILL_HEALING_MYSTIC] = Effect(SKILL_HEALING_MYSTIC, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Restore 2 Health to anything");
	effects[SKILL_HEALING_MYSTIC].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
			return tile->HasMinion();
		}));
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasMinion())
					target->minion->Heal(context.card, 2);
			};
		}
	};

	//Ironclad
	effects[SKILL_IRONCLAD] = Effect(SKILL_IRONCLAD, KEYWORD_DYING_WISH, "{Dying Wish}: Dispel all enemy minions");
	effects[SKILL_IRONCLAD].OnDeath = [](EffectContext context, Minion* minion) {
		if (context.IsCard(minion))
			for (Minion* target : context.game->minions)
				if (!context.SharesOwner(target) && target->tribe != TRIBE_GENERAL)
					target->Dispel();
	};

	//Jax Truesight
	effects[SKILL_JAX_TRUESIGHT] = Effect(SKILL_JAX_TRUESIGHT, KEYWORD_OPENING_GAMBIT | KEYWORD_RANGED, "{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged} Mini-Jax in each corner");
	effects[SKILL_JAX_TRUESIGHT].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (BoardTile* target : context.game->map.GetEmptyCorners())
			if (target != tile)
				context.game->SummonToken(CARD_MINI_JAX, target, context.card->owner);
	};

	//Jaxi
	effects[SKILL_JAXI] = Effect(SKILL_JAXI, KEYWORD_DYING_WISH, "{Dying Wish}: Summon a 1/1 {Ranged} Mini-Jax in a random corner");
	effects[SKILL_JAXI].OnDeath = [](EffectContext context, Minion* minion) {
		if (context.IsCard(minion)) {
			BoardTile* tile = context.game->map.GetRandomEmptyCorner();
			if (tile != nullptr)
				context.game->SummonToken(CARD_MINI_JAX, tile, context.card->owner);
		}
	};

	//Keeper of the Vale
	effects[SKILL_KEEPER_OF_THE_VALE] = Effect(SKILL_KEEPER_OF_THE_VALE, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Summon a random non-token friendly minion destroyed this game nearby");
	effects[SKILL_KEEPER_OF_THE_VALE].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		BoardTile* target = context.game->map.GetRandomEmptyNear(tile);
		if (target != nullptr) {
			std::vector<Card*> minions;
			for (Card* destroyed : context.game->destroyedMinions)
				if (!destroyed->isToken && destroyed->IsMinion() && context.SharesOwner(destroyed))
					minions.push_back(destroyed);
			if (minions.size() > 0) {
				int i = rand() % minions.size();
				context.game->SummonToken(minions[i]->cardId, target, context.card->owner);
			}
		}
	};

	//Khymera
	effects[SKILL_KHYMERA] = Effect(SKILL_KHYMERA, KEYWORD_NONE, "Whenever this minion takes damage, summon a random token minion nearby");
	effects[SKILL_KHYMERA].OnDamageDealt = [](EffectContext context, Card* source, Minion* target, int damage) {
		if (context.IsCardOnBoard(target)) {
			BoardTile* tile = context.game->map.GetRandomEmptyNear(target->curTile);
			if (tile != nullptr)
				context.game->SummonToken(context.game->collections->GetRandomTokenMinion()->cardId, tile, context.card->owner);
		}
	};

	//Lightbender
	effects[SKILL_LIGHTBENDER] = Effect(SKILL_LIGHTBENDER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Dispel ALL spaces around it");
	effects[SKILL_LIGHTBENDER].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (BoardTile* tile : context.game->map.GetAllNear(tile)) {
			tile->SetFeature(TILE_NONE);
			if (tile->HasMinion())
				tile->minion->Dispel();
		}
	};

	//Lux Ignis
	effects[SKILL_LUX_IGNIS] = Effect(SKILL_LUX_IGNIS, KEYWORD_RANGED, "{Ranged}|At the end of your turn, restore 2 Health to all nearby friendly minions");
	effects[SKILL_LUX_IGNIS].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player))
			for (BoardTile* tile : context.game->map.GetAllNear(context.card->GetMinion()->curTile))
				if (tile->HasMinion() && context.IsAllied(tile->minion) && !tile->minion->IsGeneral())
					tile->minion->Heal(context.card, 2);
	};

	//Lady Locke
	effects[SKILL_LADY_LOCKE] = Effect(SKILL_LADY_LOCKE, KEYWORD_OPENING_GAMBIT | KEYWORD_PROVOKE, "{Provoke}|{Opening Gambit}: Other minions you summon this turn gain +1/+1 and gain {Provoke}");
	effects[SKILL_LADY_LOCKE].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.card->AddContinuousEffect(EFFECT_LADY_LOCKE_CONTINUOUS);
	};
	effects[EFFECT_LADY_LOCKE_CONTINUOUS] = Effect(EFFECT_LADY_LOCKE_CONTINUOUS, KEYWORD_NONE, "{Lady Locke}|Minions you summon this turn gain +1/+1 and gain {Provoke}");
	effects[EFFECT_LADY_LOCKE_CONTINUOUS].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (context.IsOnBoard() && context.IsAllied(minion))
			minion->AddEffect(EFFECT_LADY_LOCKE_BUFF, nullptr);
	};
	effects[EFFECT_LADY_LOCKE_CONTINUOUS].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player))
			context.card->RemoveEffectsFromSource(context.effect);
	};
	effects[EFFECT_LADY_LOCKE_BUFF] = Effect(EFFECT_LADY_LOCKE_BUFF, KEYWORD_PROVOKE, "{Lady Locke}|{Provoke}");
	effects[EFFECT_LADY_LOCKE_BUFF].atkBuff = 1;
	effects[EFFECT_LADY_LOCKE_BUFF].hpBuff = 1;
	
	//Manaforger
	effects[SKILL_MANAFORGER] = Effect(SKILL_MANAFORGER, KEYWORD_NONE, "The first non-Bloodborn spell you cast each turn costs 1 less");
	effects[SKILL_MANAFORGER].OnAddThis = [](EffectContext context) {
		for (Card* c : context.game->castThisTurn)
			if (c->IsSpell())
				return;
		if (context.effect->ApplyEffect)
			context.effect->ApplyEffect(context);
	};
	effects[SKILL_MANAFORGER].OnRemoveThis = [](EffectContext context) {
		if (context.effect->RemoveEffect)
			context.effect->RemoveEffect(context);
	};
	effects[SKILL_MANAFORGER].OnCast = [](EffectContext context, Card* card, BoardTile* tile) {
		if (context.SharesOwner(card) && card->IsSpell()) {
			card->RemoveEffectsFromSource(context.effect);
			if (context.effect->RemoveEffect)
				context.effect->RemoveEffect(context);
		}
	};
	effects[SKILL_MANAFORGER].OnDraw = [](EffectContext context, Card* card, bool fromDeck) {
		if (context.IsOnBoard() && !fromDeck && context.SharesOwner(card) && card->IsSpell()) {
			for (Card* c : context.game->castThisTurn)
				if (c->IsSpell())
					return;
			card->AddEffect(EFFECT_MANAFORGER, context.effect);
		}
	};
	effects[SKILL_MANAFORGER].OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.IsOnBoard() && context.IsOwnedBy(player))
			if (context.effect->ApplyEffect)
				context.effect->ApplyEffect(context);
	};
	effects[SKILL_MANAFORGER].ApplyEffect = [](EffectContext context) {
		for (Card* card : context.card->owner->hand)
			if (card->IsSpell())
				card->AddEffect(EFFECT_MANAFORGER, context.effect);
		for (Card* card : context.card->owner->deck)
			if (card->IsSpell())
				card->AddEffect(EFFECT_MANAFORGER, context.effect);
	};
	effects[SKILL_MANAFORGER].RemoveEffect = [](EffectContext context) {
		for (Card* card : context.card->owner->hand)
			card->RemoveEffectsFromSource(context.effect);
		for (Card* card : context.card->owner->deck)
			card->RemoveEffectsFromSource(context.effect);
	};
	effects[EFFECT_MANAFORGER] = Effect(EFFECT_MANAFORGER, KEYWORD_NONE, "{Manaforger}");
	effects[EFFECT_MANAFORGER].costBuff = -1;

	//Maw
	effects[SKILL_MAW] = Effect(SKILL_MAW, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Deal 2 damage to a nearby enemy minion");
	effects[SKILL_MAW].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, [](BoardTile* tile) {
			return tile->HasMinion() && tile->minion->IsEnemy() && !tile->minion->IsGeneral();
		}), tile);
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, tile);
			context.game->callback.Callback = [](EffectContext context, BoardTile* source, BoardTile* target) {
				if (target->HasMinion())
					target->minion->DealDamage(context.card, 2);
			};
		}
	};

	//Mindwarper
	effects[SKILL_MINDWARPER] = Effect(SKILL_MINDWARPER, KEYWORD_OPENING_GAMBIT, "{Opening Gambit}: Gain a copy of a random spell from your opponent's action bar");
	effects[SKILL_MINDWARPER].OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		std::vector<Card*> valid;
		for (Card* card : context.card->owner->opponent->hand)
			if (card->IsSpell())
				valid.push_back(card);
		if (valid.size() > 0) {
			int i = rand() % valid.size();
			context.card->owner->AddNewToHand(valid[i]->original);
		}
	};

	//Mirkblood Devourer
	effects[SKILL_MIRKBLOOD_DEVOURER] = Effect(SKILL_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "Friendly minions summoned nearby this minion gain +1/+1");
	effects[SKILL_MIRKBLOOD_DEVOURER].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (context.IsOnBoard() && context.IsAllied(minion) && context.card->GetMinion()->curTile->IsNear(minion->curTile))
			minion->AddEffect(EFFECT_MIRKBLOOD_DEVOURER, nullptr);
	};
	effects[EFFECT_MIRKBLOOD_DEVOURER] = Effect(EFFECT_MIRKBLOOD_DEVOURER, KEYWORD_NONE, "{Mirkblood Devourer}");
	effects[EFFECT_MIRKBLOOD_DEVOURER].atkBuff = 1;
	effects[EFFECT_MIRKBLOOD_DEVOURER].hpBuff = 1;

	//Mogwai
	effects[SKILL_MOGWAI] = Effect(SKILL_MOGWAI, KEYWORD_NONE, "After this moves, draw a card");
	effects[SKILL_MOGWAI].OnMove = [](EffectContext context, Minion* minion, bool byEffect) {
		if (context.IsCardOnBoard(minion) && !byEffect)
			context.card->owner->Draw();
	};

	//Necroseer
	effects[SKILL_NECROSEER] = Effect(SKILL_NECROSEER, KEYWORD_DYING_WISH, "{Dying Wish}: Draw a card");
	effects[SKILL_NECROSEER].OnDeath = [](EffectContext context, Minion* minion) {
		if (context.IsCard(minion))
			context.card->owner->Draw();
	};

	//Night Watcher
	effects[SKILL_NIGHT_WATCHER] = Effect(SKILL_NIGHT_WATCHER, KEYWORD_FORCEFIELD, "{Forcefield}|Whenever ANY player summons a minion with {Rush}, exhaust it");
	effects[SKILL_NIGHT_WATCHER].OnSummon = [](EffectContext context, Minion* minion, bool actionBar) {
		if (context.IsOnBoard() && minion->HasKeywords(KEYWORD_RUSH))
			minion->Exhaust();
	};

#pragma endregion

#pragma region Spells

	//Breath of The Unborn
	effects[SPELL_BREATH_OF_THE_UNBORN] = Effect(SPELL_BREATH_OF_THE_UNBORN, KEYWORD_NONE, "Deal 2 damage to all enemy minions. Fully heal all friendly minions");
	effects[SPELL_BREATH_OF_THE_UNBORN].OnResolveThis = [](EffectContext context, BoardTile* tile) {
		for (Minion* minion : context.game->minions) {
			if (!minion->IsGeneral()) {
				if (context.SharesOwner(minion))
					minion->Heal(context.card, 999);
				else
					minion->DealDamage(context.card, 2);
			}
		}
	};

	//Dark Seed
	effects[SPELL_DARK_SEED] = Effect(SPELL_DARK_SEED, KEYWORD_NONE, "Deal 1 damage to the enemy general for each card in the opponent's action bar");
	effects[SPELL_DARK_SEED].OnResolveThis = [](EffectContext context, BoardTile* tile) {
		if (tile->HasMinion()) {
			int damage = context.card->owner == &context.game->players[0] ? context.game->players[1].hand.size() : context.game->players[0].hand.size();
			tile->minion->DealDamage(context.card, damage);
		}
	};

#pragma endregion

#pragma endregion

#pragma region Cards

	//Generals
	minionList.push_back(Minion(CARD_ARGEON_HIGHMAYNE, FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "argeonhighmayne", "Argeon Highmayne"));

	//Minions
	minionList.push_back(Minion(CARD_ABJUDICATOR, FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator", FindEffect(SKILL_ABJUDICATOR)));
	minionList.push_back(Minion(CARD_AETHERMASTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster", FindEffect(SKILL_AETHERMASTER)));
	minionList.push_back(Minion(CARD_ALCUIN_LOREMASTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "alcuinloremaster", "Alcuin Loremaster", FindEffect(SKILL_ALCUIN_LOREMASTER)));
	minionList.push_back(Minion(CARD_ARAKI_HEADHUNTER, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter", FindEffect(SKILL_ARAKI_HEADHUNTER)));
	minionList.push_back(Minion(CARD_ARCHON_SPELLBINDER, FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 7, 7, "archonspellbinder", "Archon Spellbinder", FindEffect(SKILL_ARCHON_SPELLBINDER)));
	minionList.push_back(Minion(CARD_ARROW_WHISTLER, FACTION_NEUTRAL, TRIBE_WARMASTER, 4, 2, 4, "arrowwhistler", "Arrow Whistler", FindEffect(SKILL_ARROW_WHISTLER)));
	minionList.push_back(Minion(CARD_ASH_MEPHYT, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 3, "ashmephyt", "Ash Mephyt", FindEffect(SKILL_ASH_MEPHYT)));
	minionList.push_back(Minion(CARD_ASTRAL_CRUSADER, FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader", FindEffect(SKILL_ASTRAL_CRUSADER)));
	minionList.push_back(Minion(CARD_AZURE_HERALD, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald", FindEffect(SKILL_AZURE_HERALD)));
	minionList.push_back(Minion(CARD_AZURE_HORN_SHAMAN, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azurehornshaman", "Azure Horn Shaman", FindEffect(SKILL_AZURE_HORN_SHAMAN)));
	minionList.push_back(Minion(CARD_BASTION, FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion", FindEffect(SKILL_BASTION)));
	minionList.push_back(Minion(CARD_BLACK_LOCUST, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 2, "blacklocust", "Black Locust", FindEffect(SKILL_BLACK_LOCUST)));
	minionList.push_back(Minion(CARD_BLAZE_HOUND, FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound", FindEffect(SKILL_BLAZE_HOUND)));
	minionList.push_back(Minion(CARD_BLISTERING_SKORN, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn", FindEffect(SKILL_BLISTERING_SKORN)));
	minionList.push_back(Minion(CARD_BLOOD_TAURA, FACTION_NEUTRAL, TRIBE_NONE, 25, 12, 12, "bloodtaura", "Blood Taura", FindEffect(SKILL_BLOOD_TAURA)));
	minionList.push_back(Minion(CARD_BLOODSHARD_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodshardgolem", "Bloodshard Golem"));
	minionList.push_back(Minion(CARD_BLOODTEAR_ALCHEMIST, FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist", FindEffect(SKILL_BLOODTEAR_ALCHEMIST)));
	minionList.push_back(Minion(CARD_BLUETIP_SCORPION, FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion", FindEffect(SKILL_BLUETIP_SCORPION)));
	minionList.push_back(Minion(CARD_BONEREAPER, FACTION_NEUTRAL, TRIBE_NONE, 6, 2, 9, "bonereaper", "Bonereaper", FindEffect(SKILL_BONEREAPER)));
	minionList.push_back(Minion(CARD_BRIGHTMOSS_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "brightmossgolem", "Brightmoss Golem"));
	minionList.push_back(Minion(CARD_CAPTAIN_HANK_HART, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "captainhankhart", "Captain Hank Hart", FindEffect(SKILL_CAPTAIN_HANK_HART)));
	minionList.push_back(Minion(CARD_CHAKKRAM, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram", FindEffect(SKILL_CHAKKRAM)));
	minionList.push_back(Minion(CARD_CHAOS_ELEMENTAL, FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental", FindEffect(SKILL_CHAOS_ELEMENTAL)));
	minionList.push_back(Minion(CARD_CRIMSON_OCULUS, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "crimsonoculus", "Crimson Oculus", FindEffect(SKILL_CRIMSON_OCULUS)));
	minionList.push_back(Minion(CARD_CROSSBONES, FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "crossbones", "Crossbones", FindEffect(SKILL_CROSSBONES)));
	minionList.push_back(Minion(CARD_DAGGER_KIRI, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 8, "daggerkiri", "Dagger Kiri", FindEffect(SKILL_CELERITY)));
	minionList.push_back(Minion(CARD_DARK_NEMESIS, FACTION_NEUTRAL, TRIBE_NONE, 7, 4, 10, "darknemesis", "Dark Nemesis", FindEffect(SKILL_DARK_NEMESIS)));
	minionList.push_back(Minion(CARD_DANCING_BLADES, FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 6, "dancingblades", "Dancing Blades", FindEffect(SKILL_DANCING_BLADES)));
	minionList.push_back(Minion(CARD_DAY_WATCHER, FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "daywatcher", "Day Watcher", FindEffect(SKILL_DAY_WATCHER)));
	minionList.push_back(Minion(CARD_DEATHBLIGHTER, FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "deathblighter", "Deathblighter", FindEffect(SKILL_DEATHBLIGHTER)));
	minionList.push_back(Minion(CARD_DECIMUS, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "decimus", "Decimus", FindEffect(SKILL_DECIMUS)));
	minionList.push_back(Minion(CARD_DIAMOND_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 6, 5, 11, "diamondgolem", "Diamond Golem"));
	minionList.push_back(Minion(CARD_DIOLTAS, FACTION_NEUTRAL, TRIBE_NONE, 4, 5, 3, "dioltas", "Dioltas", FindEffect(SKILL_DIOLTAS)));
	minionList.push_back(Minion(CARD_DRAGONLARK, FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark", FindEffect(SKILL_FLYING)));
	minionList.push_back(Minion(CARD_DREAMGAZER, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "dreamgazer", "Dreangazer", FindEffect(SKILL_DREAMGAZER)));
	minionList.push_back(Minion(CARD_DRYBONE_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 7, 10, 10, "drybonegolem", "Drybone Golem"));
	minionList.push_back(Minion(CARD_DUST_WAILER, FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "dustwailer", "Dust Wailer", FindEffect(SKILL_DUST_WAILER)));
	minionList.push_back(Minion(CARD_ECLIPSE, FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 3, 7, "eclipse", "Eclipse", FindEffect(SKILL_ECLIPSE)));
	minionList.push_back(Minion(CARD_EMERALD_REJUVENATOR, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "emeraldrejuvenator", "Emerald Rejuvenator", FindEffect(SKILL_EMERALD_REJUVENATOR)));
	minionList.push_back(Minion(CARD_ENVYBAER, FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 10, "envybaer", "Envybaer", FindEffect(SKILL_ENVYBAER)));
	minionList.push_back(Minion(CARD_EPHEMERAL_SHROUD, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "ephemeralshroud", "Ephemeral Shroud", FindEffect(SKILL_EPHEMERAL_SHROUD)));
	minionList.push_back(Minion(CARD_EXUN, FACTION_NEUTRAL, TRIBE_NONE, 7, 5, 5, "exun", "E'Xun", FindEffect(SKILL_EXUN)));
	minionList.push_back(Minion(CARD_FACESTRIKER, FACTION_NEUTRAL, TRIBE_NONE, 6, 4, 6, "facestriker", "Facestriker", FindEffect(SKILL_FACESTRIKER)));
	minionList.push_back(Minion(CARD_FIREBLAZER, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "fireblazer", "Fireblazer", FindEffect(SKILL_PROVOKE)));
	minionList.push_back(Minion(CARD_FIRESTARTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 5, 3, 5, "firestarter", "Firestarter", FindEffect(SKILL_FIRESTARTER)));
	minionList.push_back(Minion(CARD_FIRE_SPITTER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "firespitter", "Fire Spitter", FindEffect(SKILL_RANGED)));
	minionList.push_back(Minion(CARD_FIRST_SWORD_OF_AKRANE, FACTION_NEUTRAL, TRIBE_NONE, 6, 7, 7, "firstswordofakrane", "First Sword of Akrane", FindEffect(SKILL_FIRST_SWORD_OF_AKRANE)));
	minionList.push_back(Minion(CARD_FLAMEBLOOD_WARLOCK, FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "flamebloodwarlock", "Flameblood Warlock", FindEffect(SKILL_FLAMEBLOOD_WARLOCK)));
	minionList.push_back(Minion(CARD_FROSTBONE_NAGA, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 3, "frostbonenaga", "Frostbone Naga", FindEffect(SKILL_FROSTBONE_NAGA)));
	minionList.push_back(Minion(CARD_GHOST_LYNX, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx", FindEffect(SKILL_GHOST_LYNX)));
	minionList.push_back(Minion(CARD_GOLDEN_JUSTICAR, FACTION_NEUTRAL, TRIBE_WARMASTER, 5, 4, 6, "goldenjusticar", "Golden Justicar", FindEffect(SKILL_GOLDEN_JUSTICAR)));
	minionList.push_back(Minion(CARD_GOLEM_METALLURGIST, FACTION_NEUTRAL, TRIBE_GOLEM, 2, 2, 3, "golemmetallurgist", "Golem Metallurgist", FindEffect(SKILL_GOLEM_METALLURGIST)));
	minionList.push_back(Minion(CARD_GOLEM_VANQUISHER, FACTION_NEUTRAL, TRIBE_GOLEM, 3, 2, 4, "golemvanquisher", "Golem Vanquisher", FindEffect(SKILL_GOLEM_VANQUISHER)));
	minionList.push_back(Minion(CARD_GROVE_LION, FACTION_NEUTRAL, TRIBE_NONE, 6, 5, 5, "grovelion", "Grove Lion", FindEffect(SKILL_GROVE_LION)));
	minionList.push_back(Minion(CARD_HAILSTONE_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 4, 4, 6, "hailstonegolem", "Hailstone Golem"));
	minionList.push_back(Minion(CARD_HEALING_MYSTIC, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "healingmystic", "Healing Mystic", FindEffect(SKILL_HEALING_MYSTIC)));
	minionList.push_back(Minion(CARD_IRONCLAD, FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 3, "ironclad", "Ironclad", FindEffect(SKILL_IRONCLAD)));
	minionList.push_back(Minion(CARD_JAX_TRUESIGHT, FACTION_NEUTRAL, TRIBE_NONE, 6, 1, 1, "jaxtruesight", "Jax Truesight", FindEffect(SKILL_JAX_TRUESIGHT)));
	minionList.push_back(Minion(CARD_JAXI, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "jaxi", "Jaxi", FindEffect(SKILL_JAXI)));
	minionList.push_back(Minion(CARD_KEEPER_OF_THE_VALE, FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 4, "keeperofthevale", "Keeper of the Vale", FindEffect(SKILL_KEEPER_OF_THE_VALE)));
	minionList.push_back(Minion(CARD_KHYMERA, FACTION_NEUTRAL, TRIBE_NONE, 8, 5, 12, "khymera", "Khymera", FindEffect(SKILL_KHYMERA)));
	minionList.push_back(Minion(CARD_KOMODO_CHARGER, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 3, "komodocharger", "Komodo Charger"));
	minionList.push_back(Minion(CARD_LADY_LOCKE, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "ladylocke", "Lady Locke", FindEffect(SKILL_LADY_LOCKE)));
	minionList.push_back(Minion(CARD_LIGHTBENDER, FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 3, 3, "lightbender", "Lightbender", FindEffect(SKILL_LIGHTBENDER)));
	minionList.push_back(Minion(CARD_LUX_IGNIS, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 5, "luxignis", "Lux Ignis", FindEffect(SKILL_LUX_IGNIS)));
	minionList.push_back(Minion(CARD_MANAFORGER, FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "manaforger", "Manaforger", FindEffect(SKILL_MANAFORGER)));
	minionList.push_back(Minion(CARD_MAW, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 2, "maw", "Maw", FindEffect(SKILL_MAW)));
	minionList.push_back(Minion(CARD_MINDWARPER, FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 4, 3, "mindwarper", "Mindwarper", FindEffect(SKILL_MINDWARPER)));
	minionList.push_back(Minion(CARD_MIRKBLOOD_DEVOURER, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 4, "mirkblooddevourer", "Mirkblood Devourer", FindEffect(SKILL_MIRKBLOOD_DEVOURER)));
	minionList.push_back(Minion(CARD_MOGWAI, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "mogwai", "Mogwai", FindEffect(SKILL_MOGWAI)));
	minionList.push_back(Minion(CARD_NECROSEER, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 4, "necroseer", "Necroseer", FindEffect(SKILL_NECROSEER)));
	minionList.push_back(Minion(CARD_NIGHT_WATCHER, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "nightwatcher", "Night Watcher", FindEffect(SKILL_NIGHT_WATCHER)));
	minionList.push_back(Minion(CARD_PIERCING_MANTIS, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 20, "piercingmantis", "Piercing Mantis", FindEffect(SKILL_FRENZY)));
	minionList.push_back(Minion(CARD_SABERSPINE_TIGER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "saberspinetiger", "Saberspine Tiger", FindEffect(SKILL_RUSH)));
	minionList.push_back(Minion(CARD_SAPPHIRE_SEER, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 2, "sapphireseer", "Sapphire Seer", FindEffect(SKILL_FORCEFIELD)));

	//Token Minions
	minionList.push_back(Minion(CARD_MINI_JAX, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "minijax", "Mini-Jax", true, FindEffect(SKILL_RANGED)));
	minionList.push_back(Minion(CARD_SPELLSPARK, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "spellspark", "Spellspark", true, FindEffect(SKILL_RUSH)));
	minionList.push_back(Minion(CARD_TOMBSTONE, FACTION_NEUTRAL, TRIBE_NONE, 3, 0, 10, "tombstone", "Tombstone", true, FindEffect(SKILL_PROVOKE)));

	//Spells
	spellList.push_back(Spell(CARD_BREATH_OF_THE_UNBORN, FACTION_ABYSSIAN, 4, "breathoftheunborn", "Breath of The Unborn", FindEffect(SPELL_BREATH_OF_THE_UNBORN)));
	spellList.back().targetMode = TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
		return tile->minion != nullptr;
	});
	spellList.push_back(Spell(CARD_DARK_SEED, FACTION_ABYSSIAN, 4, "darkseed", "Dark Seed", FindEffect(SPELL_DARK_SEED)));
	spellList.back().targetMode = TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
		return tile->minion != nullptr && tile->minion->IsEnemy() && tile->minion->tribe == TRIBE_GENERAL;
	});

	//Generate card map
	for (int i = 0; i < minionList.size(); ++i) {
		cards[minionList[i].cardId] = &minionList[i];
		cards[minionList[i].cardId]->original = cards[minionList[i].cardId];
		cardList.push_back(cards[minionList[i].cardId]);
	}
	for (int i = 0; i < spellList.size(); ++i) {
		cards[spellList[i].cardId] = &spellList[i];
		cards[spellList[i].cardId]->original = cards[spellList[i].cardId];
		cardList.push_back(cards[spellList[i].cardId]);
	}

	//Set original references
	for (int i = 0; i < cardList.size(); ++i) { cardList[i]->original = cardList[i]; }

	//Assign tokens
	cards[CARD_DIOLTAS]->effects[0]->token = cards[CARD_TOMBSTONE];
	cards[CARD_FIRESTARTER]->effects[0]->token = cards[CARD_SPELLSPARK];
	cards[CARD_JAX_TRUESIGHT]->effects[0]->token = cards[CARD_MINI_JAX];
	cards[CARD_JAXI]->effects[0]->token = cards[CARD_MINI_JAX];

#pragma endregion

	//Counts
	generalCount = 0;
	minionCount = 0;
	for (int i = 0; i < minionList.size(); ++i) {
		if (minionList[i].tribe == TRIBE_GENERAL)
			++generalCount;
		else
			++minionCount;
	}
	spellCount = spellList.size();
	cardCount = generalCount + minionCount + spellCount;

}
Collections::~Collections() {}

#pragma endregion

#pragma region Search Functions

//Find effect by enum
Effect* Collections::FindEffect(eEffect effect) {
	if (effects.contains(effect))
		return &effects[effect];
	return nullptr;
}

//Find card by name
Card* Collections::FindCard(eCard card) {
	if (cards.contains(card))
		return cards[card];
	return nullptr;
}

//Get random token minion
Minion* Collections::GetRandomTokenMinion() {
	std::vector<Minion*> valid;
	for (int i = 0; i < minionList.size(); ++i)
		if (minionList[i].isToken)
			valid.push_back(&minionList[i]);
	if (valid.size() > 0) {
		int i = rand() % valid.size();
		return valid[i];
	}
	return nullptr;
}

#pragma endregion