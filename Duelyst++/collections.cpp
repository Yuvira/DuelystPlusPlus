//Include
#include "game.h"

#pragma region Constructor

//Card list constructor
Collections::Collections() {

#pragma region Effects

	//Keyword skills
	effectList.push_back(Effect(SKILL_CELERITY, KEYWORD_CELERITY, 0, 0, 0, "{Celerity}"));
	effectList.push_back(Effect(SKILL_FLYING, KEYWORD_FLYING, 0, 0, 0, "{Flying}"));
	effectList.push_back(Effect(SKILL_FORCEFIELD, KEYWORD_FORCEFIELD, 0, 0, 0, "{Forcefield}"));
	effectList.push_back(Effect(SKILL_PROVOKE, KEYWORD_PROVOKE, 0, 0, 0, "{Provoke}"));
	effectList.push_back(Effect(SKILL_RANGED, KEYWORD_RANGED, 0, 0, 0, "{Ranged}"));
	effectList.push_back(Effect(SKILL_RUSH, KEYWORD_RUSH, 0, 0, 0, "{Rush}"));

	//Dispelled
	effectList.push_back(Effect(EFFECT_DISPELLED, KEYWORD_NONE, 0, 0, 0, "{Dispelled}"));

#pragma region Minions

	//Abjudicator
	effectList.push_back(Effect(SKILL_ABJUDICATOR, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Lower the cost of all|spells in your action bar by 1"));
	effectList.back().OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (int i = 0; i < context.card->owner->hand.size(); ++i)
			if (context.card->owner->hand[i]->cardType == CARD_SPELL)
				context.card->owner->hand[i]->AddEffect(context.game->collections->FindEffect(EFFECT_ABJUDICATOR), context.effect);
	};
	effectList.push_back(Effect(EFFECT_ABJUDICATOR, KEYWORD_NONE, -1, 0, 0, "Abjudicator"));

	//Aethermaster
	effectList.push_back(Effect(SKILL_AETHERMASTER, KEYWORD_NONE, 0, 0, 0, "You may replace an additional card|each turn"));
	effectList.back().OnDispelThis = [](EffectContext context) {
		context.card->owner->replaces = max(context.card->owner->replaces - 1, 0);
	};
	effectList.back().OnSummon = [](EffectContext context, Minion* source, bool fromActionBar) {
		if (context.card == source)
			++context.card->owner->replaces;
	};
	effectList.back().OnDeath = [](EffectContext context, Minion* source) {
		if (context.card == source)
			context.card->owner->replaces = max(context.card->owner->replaces - 1, 0);
	};
	effectList.back().OnTurnEnd = [](EffectContext context, Player* player) {
		if (context.card->GetMinion() != nullptr && context.card->GetMinion()->curTile != nullptr && context.card->owner == player)
			++context.card->owner->replaces;
	};

	//Alcuin Loremaster
	effectList.push_back(Effect(SKILL_ALCUIN_LOREMASTER, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Put a copy of the most|recently cast spell into your|action bar"));
	effectList.back().OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		for (int i = context.game->spellHistory.size() - 1; i >= 0; --i) {
			if (context.game->spellHistory[i]->cardType == CARD_SPELL) {
				context.card->owner->AddToHand(context.game->spellHistory[i]->original, true);
				break;
			}
		}
	};

	//Ash Mephyt
	effectList.push_back(Effect(SKILL_ASH_MEPHYT, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Summon two copies of|this minion on random spaces"));
	effectList.back().OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		if (context.card->IsMinion()) {
			for (int i = 0; i < 2; ++i) {
				BoardTile* newTile = context.game->map.GetRandom(tile);
				if (newTile != nullptr) {
					Minion* copy = new Minion(*(context.card->original->GetMinion()));
					context.game->SetContext(copy, context.card->owner);
					context.game->Summon(copy, newTile, false);
				}
			}
		}
	};

	//Bloodtear Alchemist
	effectList.push_back(Effect(SKILL_BLOODTEAR_ALCHEMIST, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Deal 1 damage to|an enemy"));
	effectList.back().OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_ALL, TARGET_FILTER_ENEMY));
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, nullptr);
			context.game->callback.callback = [](EffectContext context, BoardTile* tile) {
				if (tile->minion != nullptr)
					tile->minion->DealDamage(context.card->GetMinion(), 1);
			};
		}
	};

	//Ephemeral Shroud
	effectList.push_back(Effect(SKILL_EPHEMERAL_SHROUD, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Dispel 1 nearby space"));
	effectList.back().OnPreCastThis = [](EffectContext context, BoardTile* tile) {
		context.game->HighlightSelectable(TargetMode(TARGET_MODE_NEAR_TILE, TARGET_FILTER_NONE), tile);
		if (context.game->selectable.size() > 0) {
			context.game->callback = EffectCallback(context, nullptr);
			context.game->callback.callback = [](EffectContext context, BoardTile* tile) {
				tile->SetFeature(TILE_NONE);
				if (tile->minion != nullptr)
					tile->minion->Dispel();
			};
		}
	};

#pragma endregion

#pragma region Spells

	//Breath of The Unborn
	effectList.push_back(Effect(SPELL_BREATH_OF_THE_UNBORN, KEYWORD_NONE, 0, 0, 0, "Deal 2 damage to all enemy|minions. Fully heal all friendly|minions"));
	effectList.back().OnResolveThis = [](EffectContext context, BoardTile* tile) {
		for (int i = 0; i < context.game->minions.size(); ++i) {
			if (context.game->minions[i]->tribe != TRIBE_GENERAL) {
				if (context.game->minions[i]->owner == context.card->owner) { context.game->minions[i]->DealDamage(nullptr, -999); }
				else { context.game->minions[i]->DealDamage(nullptr, 2); }
			}
		}
	};

	//Dark Seed
	effectList.push_back(Effect(SPELL_DARK_SEED, KEYWORD_NONE, 0, 0, 0, "Deal 1 damage to the enemy general|for each card in the opponent's|action bar"));
	effectList.back().OnResolveThis = [](EffectContext context, BoardTile* tile) {
		if (tile->minion != nullptr) {
			int damage = context.card->owner == &context.game->players[0] ? context.game->players[1].hand.size() : context.game->players[0].hand.size();
			tile->minion->DealDamage(nullptr, damage);
		}
	};

#pragma endregion

	/*

	//Minion skills
	skillList.push_back(Skill(SKILL_ALTER_REXX));
	skillList.back().GenerateSprite("Whenever you summon MECHAZ0R, put a|MECHAZ0R in your action bar");
	skillList.push_back(Skill(SKILL_ARAKI_HEADHUNTER));
	skillList.back().GenerateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack");
	buffList.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 0, 2, 0, true));
	buffList.back().GenerateSprite("Headhunter");
	skillList.push_back(Skill(SKILL_ARCHON_SPELLBINDER));
	skillList.back().GenerateSprite("Your opponent's non-Bloodborn spells|cost 1 more to cast");
	buffList.push_back(Buff(BUFF_ARCHON_SPELLBINDER, 1, 0, 0, true));
	buffList.back().GenerateSprite("Spellbound");
	skillList.push_back(Skill(SKILL_ARROW_WHISTLER));
	skillList.back().GenerateSprite("{Ranged}|Your other minions with {Ranged}|have +1 Attack");
	buffList.push_back(Buff(BUFF_ARROW_WHISTLER, 0, 1, 0, true));
	buffList.back().GenerateSprite("Whistling Arrows");
	skillList.push_back(Skill(SKILL_ASTRAL_CRUSADER));
	skillList.back().GenerateSprite("Whenever you replace this card, it|costs 3 less and gains +3/+3");
	buffList.push_back(Buff(BUFF_ASTRAL_CRUSADER, -3, 3, 3, true));
	buffList.back().GenerateSprite("Astral Crusader");
	skillList.push_back(Skill(SKILL_AZURE_HERALD));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 3 Health to|your General");
	skillList.push_back(Skill(SKILL_AZURE_HORN_SHAMAN));
	skillList.back().GenerateSprite("{Dying Wish}: Give +4 Health to friendly|minions around it");
	buffList.push_back(Buff(BUFF_AZURE_HORN_SHAMAN, 0, 0, 4, true));
	buffList.back().GenerateSprite("Azure Horn Shaman");
	skillList.push_back(Skill(SKILL_BASTION));
	skillList.back().GenerateSprite("At the end of your turn, give other|friendly minions +1 Health");
	buffList.push_back(Buff(BUFF_BASTION, 0, 0, 1, true));
	buffList.back().GenerateSprite("Bastion");
	skillList.push_back(Skill(SKILL_BLACK_LOCUST));
	skillList.back().GenerateSprite("{Flying}|After this minion moves, summon|a Black Locust nearby");
	skillList.push_back(Skill(SKILL_BLAZE_HOUND));
	skillList.back().GenerateSprite("{Opening Gambit}: Both players draw|a card");
	skillList.push_back(Skill(SKILL_BLISTERING_SKORN));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 1 damage to|everything (including itself)");
	skillList.push_back(Skill(SKILL_BLOOD_TAURA));
	skillList.back().GenerateSprite("{Provoke}|This minion's cost is equal to your|General's Health");
	buffList.push_back(Buff(BUFF_BLOOD_TAURA, -1, 0, 0, true));
	buffList.back().GenerateSprite("Blood Taura");
	skillList.push_back(Skill(SKILL_BLUETIP_SCORPION));
	skillList.back().GenerateSprite("Deals double damage to minions");
	skillList.push_back(Skill(SKILL_BONEREAPER));
	skillList.back().GenerateSprite("{Provoke}|At the end of your turn, deal 2 damage|to all nearby enemy minions");
	skillList.push_back(Skill(SKILL_CAPTAIN_HANK_HART));
	skillList.back().GenerateSprite("{Ranged}|Whenever this deals damage, restore|that much health to it");
	skillList.push_back(Skill(SKILL_CHAKKRAM));
	skillList.back().GenerateSprite("Costs 2 less if your General took|damage on your opponent's|last turn");
	buffList.push_back(Buff(BUFF_CHAKKRAM, -2, 0, 0, false));
	buffList.back().GenerateSprite("Chakkram");
	skillList.push_back(Skill(SKILL_CHAOS_ELEMENTAL));
	skillList.back().GenerateSprite("Whenever this minion takes damage,|it randomly teleports");
	skillList.push_back(Skill(SKILL_CRIMSON_OCULUS));
	skillList.back().GenerateSprite("Whenever your opponent summons a|minion, this gains +1/+1");
	buffList.push_back(Buff(BUFF_CRIMSON_OCULUS, 0, 1, 1, true));
	buffList.back().GenerateSprite("Crimson Oculus");
	skillList.push_back(Skill(SKILL_CROSSBONES));
	skillList.back().GenerateSprite("{Opening Gambit}: Destroy an enemy|minion with {Ranged}");
	skillList.push_back(Skill(SKILL_DANCING_BLADES));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|ANY minion in front of this");
	skillList.push_back(Skill(SKILL_DARK_NEMESIS));
	skillList.back().GenerateSprite("At the start of your turn, deal 4|damage to the enemy General and this|gains +4 Attack");
	buffList.push_back(Buff(BUFF_DARK_NEMESIS, 0, 4, 0, true));
	buffList.back().GenerateSprite("Dark Nemesis");
	skillList.push_back(Skill(SKILL_DAY_WATCHER));
	skillList.back().GenerateSprite("Whenever a friendly minion attacks,|restore 1 Health to your General");
	skillList.push_back(Skill(SKILL_DEATHBLIGHTER));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|all enemy minions around this");
	skillList.push_back(Skill(SKILL_DECIMUS));
	skillList.back().GenerateSprite("Whenever your opponent draws a card,|deal 2 damage to the enemy General");
	skillList.push_back(Skill(SKILL_DIOLTAS));
	skillList.back().GenerateSprite("{Dying Wish}: Summon a 0/8 Tombstone with|Provoke near your General");
	skillList.push_back(Skill(SKILL_DREAMGAZER));
	skillList.back().GenerateSprite("When you replace this card, summon it|nearby. Your General takes 2 damage");
	skillList.push_back(Skill(SKILL_DUST_WAILER));
	skillList.back().GenerateSprite("{Flying}|{Opening Gambit}: Deal 3 damage to all|enemies in front of this");
	skillList.push_back(Skill(SKILL_ECLIPSE));
	skillList.back().GenerateSprite("Whenever this minion takes damage, it|deals that much damage to the|enemy General");
	skillList.push_back(Skill(SKILL_EMERALD_REJUVENATOR));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 4 Health to|BOTH Generals");
	skillList.push_back(Skill(SKILL_ENVYBAER));
	skillList.back().GenerateSprite("Whenever this minion damages an enemy,|teleport that enemy to a random corner");
	skillList.push_back(Skill(SKILL_EXUN));
	skillList.back().GenerateSprite("{Forcefield}|Whenever this minion attacks or is|attacked, draw a card");
	skillList.push_back(Skill(SKILL_FACESTRIKER));
	skillList.back().GenerateSprite("Deals double damage to Generals");
	skillList.push_back(Skill(SKILL_FIRESTARTER));
	skillList.back().GenerateSprite("Whenever you cast a spell, summon a|1/1 Spellspark with {Rush} nearby");
	skillList.push_back(Skill(SKILL_FIRST_SWORD_OF_AKRANE));
	skillList.back().GenerateSprite("Your other minions have +1 Attack");
	buffList.push_back(Buff(BUFF_FIRST_SWORD_OF_AKRANE, 0, 1, 0, true));
	buffList.back().GenerateSprite("Might of Akrane");
	skillList.push_back(Skill(SKILL_FLAMEBLOOD_WARLOCK));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|BOTH Generals");
	skillList.push_back(Skill(SKILL_FROSTBONE_NAGA));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 2 damage to everything around this");
	skillList.push_back(Skill(SKILL_GHOST_LYNX));
	skillList.back().GenerateSprite("{Opening Gambit}: Teleport a nearby|minion to a random space");
	skillList.push_back(Skill(SKILL_GOLDEN_JUSTICAR));
	skillList.back().GenerateSprite("{Provoke}|Your other minions with {Provoke}|can move two additional spaces");
	effectList.push_back(Effect(EFFECT_GOLDEN_JUSTICAR));
	effectList.back().GenerateSprite("Golden Justicar|Can move two extra spaces");
	skillList.push_back(Skill(SKILL_GOLEM_METALLURGIST));
	skillList.back().GenerateSprite("The first Golem you summon each turn|costs 1 less");
	buffList.push_back(Buff(BUFF_GOLEM_METALLURGIST, -1, 0, 0, true));
	buffList.back().GenerateSprite("Golem Metallurgist");
	skillList.push_back(Skill(SKILL_GOLEM_VANQUISHER));
	skillList.back().GenerateSprite("{Provoke}|Your other Golem minions have {Provoke}");
	effectList.push_back(Effect(EFFECT_GOLEM_VANQUISHER));
	effectList.back().GenerateSprite("Golem Vanquisher|Provoke");
	skillList.push_back(Skill(SKILL_GROVE_LION));
	skillList.back().GenerateSprite("While this minion is on the battlefield,|your general has {Forcefield}");
	effectList.push_back(Effect(EFFECT_GROVE_LION));
	effectList.back().GenerateSprite("Grove Lion|Forcefield");
	skillList.push_back(Skill(SKILL_HEALING_MYSTIC));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 2 Health|to anything");
	skillList.push_back(Skill(SKILL_IRONCLAD));
	skillList.back().GenerateSprite("{Dying Wish}: Dispel all enemy minions");
	skillList.push_back(Skill(SKILL_JAX_TRUESIGHT));
	skillList.back().GenerateSprite("{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged}|Mini-Jax in each corner");
	skillList.push_back(Skill(SKILL_JAXI));
	skillList.back().GenerateSprite("{Dying Wish}: Summon a 1/1 {Ranged}|Mini-Jax in a random corner");
	skillList.push_back(Skill(SKILL_KEEPER_OF_THE_VALE));
	skillList.back().GenerateSprite("{Opening Gambit}: Summon a random friendly|minion destroyed this game nearby this|minion");
	skillList.push_back(Skill(SKILL_KHYMERA));
	skillList.back().GenerateSprite("Whenever this minion takes damage,|summon a random token minion nearby");
	skillList.push_back(Skill(SKILL_LADY_LOCKE));
	skillList.back().GenerateSprite("{Provoke}|{Opening Gambit}: Other minions you summon|this turn gain +1/+1 and {Provoke}");
	effectList.push_back(Effect(EFFECT_LADY_LOCKE_A));
	effectList.back().GenerateSprite("Lady Locke|Summoned minions gain +1/+1 and Provoke");
	effectList.push_back(Effect(EFFECT_LADY_LOCKE_B));
	effectList.back().GenerateSprite("Lady Locke|Provoke");
	buffList.push_back(Buff(BUFF_LADY_LOCKE, 0, 1, 1, true));
	buffList.back().GenerateSprite("Lady Locke");
	skillList.push_back(Skill(SKILL_LIGHTBENDER));
	skillList.back().GenerateSprite("{Opening Gambit}: Dispel ALL spaces|around it");
	skillList.push_back(Skill(SKILL_LUX_IGNIS));
	skillList.back().GenerateSprite("{Ranged}|At the end of your turn, restore|2 Health to all nearby friendly minions");

	//Spell effects
	spellList.push_back(SpellEffect(SPELL_CONSUMING_REBIRTH));
	spellList.back().GenerateSprite("Destroy a friendly minion. At the end|of your turn, re-summon it on|the same space and give it +1/+1");
	buffList.push_back(Buff(BUFF_CONSUMING_REBIRTH, 0, 1, 1, true));
	buffList.back().GenerateSprite("Consuming Rebirth");
	spellList.push_back(SpellEffect(SPELL_DAEMONIC_LURE));
	spellList.back().GenerateSprite("Deal 1 damage to an enemy minion and|teleport it anywhere");
	spellList.push_back(SpellEffect(SPELL_DARK_TRANSFORMATION));
	spellList.back().GenerateSprite("Destroy an enemy minion and summon|a 1/1 Wraithling on that space");
	spellList.push_back(SpellEffect(SPELL_DARKFIRE_SACRIFICE));
	spellList.back().GenerateSprite("Destroy a friendly minion. Your next|minion costs 2 less");
	effectList.push_back(Effect(EFFECT_DARKFIRE_SACRIFICE));
	effectList.back().GenerateSprite("Darkfire Sacrifice|Your next minion costs 2 less");
	buffList.push_back(Buff(BUFF_DARKFIRE_SACRIFICE, -2, 0, 0, true));
	buffList.back().GenerateSprite("Darkfire Sacrifice");

	*/

#pragma endregion

#pragma region Cards

	//Generals
	generalList.push_back(Minion(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "argeonhighmayne", "Argeon Highmayne"));

	//Minions
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator", FindEffect(SKILL_ABJUDICATOR)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster", FindEffect(SKILL_AETHERMASTER)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "alcuinloremaster", "Alcuin Loremaster", FindEffect(SKILL_ALCUIN_LOREMASTER)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 3, "ashmephyt", "Ash Mephyt", FindEffect(SKILL_ASH_MEPHYT)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist", FindEffect(SKILL_BLOODTEAR_ALCHEMIST)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark", FindEffect(SKILL_FLYING)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "fireblazer", "Fireblazer", FindEffect(SKILL_PROVOKE)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "firespitter", "Fire Spitter", FindEffect(SKILL_RANGED)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 3, "komodocharger", "Komodo Charger"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "saberspinetiger", "Saberspine Tiger", FindEffect(SKILL_RUSH)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 2, "sapphireseer", "Sapphire Seer", FindEffect(SKILL_FORCEFIELD)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "ephemeralshroud", "Ephemeral Shroud", FindEffect(SKILL_EPHEMERAL_SHROUD)));

	//Spells
	spellList.push_back(Spell(FACTION_ABYSSIAN, TargetMode(TARGET_MODE_ALL, TARGET_FILTER_UNIT), 4, "breathoftheunborn", "Breath of The Unborn", FindEffect(SPELL_BREATH_OF_THE_UNBORN)));
	spellList.push_back(Spell(FACTION_ABYSSIAN, TargetMode(TARGET_MODE_ALL, TARGET_FILTER_ENEMY | TARGET_FILTER_GENERAL), 4, "darkseed", "Dark Seed", FindEffect(SPELL_DARK_SEED)));

	/*

	//Units
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_MECH, 5, 5, 5, "alterrexx", "Alter Rexx"));
	minionList.back().skill = effectList.Find(SKILL_ALTER_REXX);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	minionList.back().skill = effectList.Find(SKILL_ARAKI_HEADHUNTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 7, 7, "archonspellbinder", "Archon Spellbinder"));
	minionList.back().skill = effectList.Find(SKILL_ARCHON_SPELLBINDER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_WARMASTER, 4, 2, 4, "arrowwhistler", "Arrow Whistler"));
	minionList.back().skill = effectList.Find(SKILL_ARROW_WHISTLER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader"));
	minionList.back().skill = effectList.Find(SKILL_ASTRAL_CRUSADER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	minionList.back().skill = effectList.Find(SKILL_AZURE_HERALD);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azurehornshaman", "Azure Horn Shaman"));
	minionList.back().skill = effectList.Find(SKILL_AZURE_HORN_SHAMAN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion"));
	minionList.back().skill = effectList.Find(SKILL_BASTION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 2, "blacklocust", "Black Locust"));
	minionList.back().skill = effectList.Find(SKILL_BLACK_LOCUST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound"));
	minionList.back().skill = effectList.Find(SKILL_BLAZE_HOUND);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn"));
	minionList.back().skill = effectList.Find(SKILL_BLISTERING_SKORN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodshardgolem", "Bloodshard Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 25, 12, 12, "bloodtaura", "Blood Taura"));
	minionList.back().skill = effectList.Find(SKILL_BLOOD_TAURA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist"));
	minionList.back().skill = effectList.Find(SKILL_BLOODTEAR_ALCHEMIST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion"));
	minionList.back().skill = effectList.Find(SKILL_BLUETIP_SCORPION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 2, 9, "bonereaper", "Bonereaper"));
	minionList.back().skill = effectList.Find(SKILL_BONEREAPER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "brightmossgolem", "Brightmoss Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "captainhankhart", "Captain Hank Hart"));
	minionList.back().skill = effectList.Find(SKILL_CAPTAIN_HANK_HART);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram"));
	minionList.back().skill = effectList.Find(SKILL_CHAKKRAM);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental"));
	minionList.back().skill = effectList.Find(SKILL_CHAOS_ELEMENTAL);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "crimsonoculus", "Crimson Oculus"));
	minionList.back().skill = effectList.Find(SKILL_CRIMSON_OCULUS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "crossbones", "Crossbones"));
	minionList.back().skill = effectList.Find(SKILL_CROSSBONES);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 8, "daggerkiri", "Dagger Kiri"));
	minionList.back().skill = effectList.Find(SKILL_CELERITY);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 6, "dancingblades", "Dancing Blades"));
	minionList.back().skill = effectList.Find(SKILL_DANCING_BLADES);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 4, 10, "darknemesis", "Dark Nemesis"));
	minionList.back().skill = effectList.Find(SKILL_DARK_NEMESIS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "daywatcher", "Day Watcher"));
	minionList.back().skill = effectList.Find(SKILL_DAY_WATCHER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "deathblighter", "Deathblighter"));
	minionList.back().skill = effectList.Find(SKILL_DEATHBLIGHTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "decimus", "Decimus"));
	minionList.back().skill = effectList.Find(SKILL_DECIMUS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 6, 5, 11, "diamondgolem", "Diamond Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 5, 3, "dioltas", "Dioltas"));
	minionList.back().skill = effectList.Find(SKILL_DIOLTAS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "dreamgazer", "Dreamgazer"));
	minionList.back().skill = effectList.Find(SKILL_DREAMGAZER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 7, 10, 10, "drybonegolem", "Drybone Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "dustwailer", "Dust Wailer"));
	minionList.back().skill = effectList.Find(SKILL_DUST_WAILER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 3, 7, "eclipse", "Eclipse"));
	minionList.back().skill = effectList.Find(SKILL_ECLIPSE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "emeraldrejuvenator", "Emerald Rejuvenator"));
	minionList.back().skill = effectList.Find(SKILL_EMERALD_REJUVENATOR);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 10, "envybaer", "Envybaer"));
	minionList.back().skill = effectList.Find(SKILL_ENVYBAER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 5, 5, "exun", "E'Xun"));
	minionList.back().skill = effectList.Find(SKILL_EXUN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 4, 6, "facestriker", "Facestriker"));
	minionList.back().skill = effectList.Find(SKILL_FACESTRIKER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 5, 3, 5, "firestarter", "Firestarter"));
	minionList.back().skill = effectList.Find(SKILL_FIRESTARTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 7, 7, "firstswordofakrane", "First Sword of Akrane"));
	minionList.back().skill = effectList.Find(SKILL_FIRST_SWORD_OF_AKRANE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "flamebloodwarlock", "Flameblood Warlock"));
	minionList.back().skill = effectList.Find(SKILL_FLAMEBLOOD_WARLOCK);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 3, "frostbonenaga", "Frostbone Naga"));
	minionList.back().skill = effectList.Find(SKILL_FROSTBONE_NAGA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	minionList.back().skill = effectList.Find(SKILL_GHOST_LYNX);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_WARMASTER, 5, 4, 6, "goldenjusticar", "Golden Justicar"));
	minionList.back().skill = effectList.Find(SKILL_GOLDEN_JUSTICAR);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 2, 2, 3, "golemmetallurgist", "Golem Metallurgist"));
	minionList.back().skill = effectList.Find(SKILL_GOLEM_METALLURGIST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 2, 4, "golemvanquisher", "Golem Vanquisher"));
	minionList.back().skill = effectList.Find(SKILL_GOLEM_VANQUISHER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 5, 5, "grovelion", "Grove Lion"));
	minionList.back().skill = effectList.Find(SKILL_GROVE_LION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 4, 4, 6, "hailstonegolem", "Hailstone Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "healingmystic", "Healing Mystic"));
	minionList.back().skill = effectList.Find(SKILL_HEALING_MYSTIC);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 3, "ironclad", "Ironclad"));
	minionList.back().skill = effectList.Find(SKILL_IRONCLAD);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 1, 1, "jaxtruesight", "Jax Truesight"));
	minionList.back().skill = effectList.Find(SKILL_JAX_TRUESIGHT);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "jaxi", "Jaxi"));
	minionList.back().skill = effectList.Find(SKILL_JAXI);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 4, "keeperofthevale", "Keeper of The Vale"));
	minionList.back().skill = effectList.Find(SKILL_KEEPER_OF_THE_VALE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 8, 5, 12, "khymera", "Khymera"));
	minionList.back().skill = effectList.Find(SKILL_KHYMERA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "ladylocke", "Lady Locke"));
	minionList.back().skill = effectList.Find(SKILL_LADY_LOCKE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 3, 3, "lightbender", "Lightbender"));
	minionList.back().skill = effectList.Find(SKILL_LIGHTBENDER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 5, "luxignis", "Lux Ignis"));
	minionList.back().skill = effectList.Find(SKILL_LUX_IGNIS);

	//Token units
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "minijax", "Mini-Jax"));
	minionTokenList.back().skill = effectList.Find(SKILL_RANGED);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "spellspark", "Spellspark"));
	minionTokenList.back().skill = effectList.Find(SKILL_RUSH);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 0, 10, "tombstone", "Tombstone"));
	minionTokenList.back().skill = effectList.Find(SKILL_PROVOKE);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_ABYSSIAN, TRIBE_NONE, 1, 1, 1, "wraithling", "Wraithling"));
	minionTokenList.back().isToken = true;

	//Spells
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 2, "consumingrebirth", "Consuming Rebirth"));
	spellList.back().spell = effectList.Find(SPELL_CONSUMING_REBIRTH);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 2, "daemoniclure", "Daemonic Lure"));
	spellList.back().spell = effectList.Find(SPELL_DAEMONIC_LURE);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 5, "darktransformation", "Dark Transformation"));
	spellList.back().spell = effectList.Find(SPELL_DARK_TRANSFORMATION);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 0, "darkfiresacrifice", "Darkfire Sacrifice"));
	spellList.back().spell = effectList.Find(SPELL_DARKFIRE_SACRIFICE);

	//Token spells

	//Add tokens
	Find("Dark Transformation")->token = Find("Wraithling");
	Find("Dioltas")->token = Find("Tombstone");
	Find("Firestarter")->token = Find("Spellspark");
	Find("Jax Truesight")->token = Find("Mini-Jax");
	Find("Jaxi")->token = Find("Mini-Jax");

	*/

	//Generate card list
	for (int i = 0; i < generalList.size(); ++i) { cardList.push_back(&generalList[i]); }
	for (int i = 0; i < minionList.size(); ++i) { cardList.push_back(&minionList[i]); }
	for (int i = 0; i < minionTokenList.size(); ++i) { cardList.push_back(&minionTokenList[i]); }
	for (int i = 0; i < spellList.size(); ++i) { cardList.push_back(&spellList[i]); }
	for (int i = 0; i < spellTokenList.size(); ++i) { cardList.push_back(&spellTokenList[i]); }

	//Set original references
	for (int i = 0; i < cardList.size(); ++i) { cardList[i]->original = cardList[i]; }

#pragma endregion

}
Collections::~Collections() {}

#pragma endregion

#pragma region Search Functions

//Find effect by enum
Effect Collections::FindEffect(eEffect effect) {
	for (int i = 0; i < effectList.size(); ++i)
		if (effectList[i].effect == effect)
			return effectList[i];
}

//Find card by name
Card* Collections::FindCard(std::string name) {
	for (int i = 0; i < cardList.size(); ++i)
		if (cardList[i]->name == name)
			return cardList[i];
	return cardList[0];
}

#pragma endregion