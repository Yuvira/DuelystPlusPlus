//Include
#include <ranges>
#include "card.h"
#include "collections.h"
#include "tile.h"

//Register card objects
void Collections::RegisterCards() {

	//Generals
	unitList.push_back(Unit(CARD_ARGEON_HIGHMAYNE, FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "argeonhighmayne", "Argeon Highmayne"));

	//Minions
	unitList.push_back(Unit(CARD_ABJUDICATOR, FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator", FindEffect(SKILL_ABJUDICATOR)));
	unitList.push_back(Unit(CARD_AETHERMASTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster", FindEffect(SKILL_AETHERMASTER)));
	unitList.push_back(Unit(CARD_ALCUIN_LOREMASTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "alcuinloremaster", "Alcuin Loremaster", FindEffect(SKILL_ALCUIN_LOREMASTER)));
	unitList.push_back(Unit(CARD_ARAKI_HEADHUNTER, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter", FindEffect(SKILL_ARAKI_HEADHUNTER)));
	unitList.push_back(Unit(CARD_ARCHON_SPELLBINDER, FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 7, 7, "archonspellbinder", "Archon Spellbinder", FindEffect(SKILL_ARCHON_SPELLBINDER)));
	unitList.push_back(Unit(CARD_ARROW_WHISTLER, FACTION_NEUTRAL, TRIBE_WARMASTER, 4, 2, 4, "arrowwhistler", "Arrow Whistler", FindEffect(SKILL_ARROW_WHISTLER)));
	unitList.push_back(Unit(CARD_ASH_MEPHYT, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 3, "ashmephyt", "Ash Mephyt", FindEffect(SKILL_ASH_MEPHYT)));
	unitList.push_back(Unit(CARD_ASTRAL_CRUSADER, FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader", FindEffect(SKILL_ASTRAL_CRUSADER)));
	unitList.push_back(Unit(CARD_AZURE_HERALD, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald", FindEffect(SKILL_AZURE_HERALD)));
	unitList.push_back(Unit(CARD_AZURE_HORN_SHAMAN, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azurehornshaman", "Azure Horn Shaman", FindEffect(SKILL_AZURE_HORN_SHAMAN)));
	unitList.push_back(Unit(CARD_BASTION, FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion", FindEffect(SKILL_BASTION)));
	unitList.push_back(Unit(CARD_BLACK_LOCUST, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 2, "blacklocust", "Black Locust", FindEffect(SKILL_BLACK_LOCUST)));
	unitList.push_back(Unit(CARD_BLAZE_HOUND, FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound", FindEffect(SKILL_BLAZE_HOUND)));
	unitList.push_back(Unit(CARD_BLISTERING_SKORN, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn", FindEffect(SKILL_BLISTERING_SKORN)));
	unitList.push_back(Unit(CARD_BLOOD_TAURA, FACTION_NEUTRAL, TRIBE_NONE, 25, 12, 12, "bloodtaura", "Blood Taura", FindEffect(SKILL_BLOOD_TAURA)));
	unitList.push_back(Unit(CARD_BLOODSHARD_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodshardgolem", "Bloodshard Golem"));
	unitList.push_back(Unit(CARD_BLOODTEAR_ALCHEMIST, FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist", FindEffect(SKILL_BLOODTEAR_ALCHEMIST)));
	unitList.push_back(Unit(CARD_BLUETIP_SCORPION, FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion", FindEffect(SKILL_BLUETIP_SCORPION)));
	unitList.push_back(Unit(CARD_BONEREAPER, FACTION_NEUTRAL, TRIBE_NONE, 6, 2, 9, "bonereaper", "Bonereaper", FindEffect(SKILL_BONEREAPER)));
	unitList.push_back(Unit(CARD_BRIGHTMOSS_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "brightmossgolem", "Brightmoss Golem"));
	unitList.push_back(Unit(CARD_CAPTAIN_HANK_HART, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "captainhankhart", "Captain Hank Hart", FindEffect(SKILL_CAPTAIN_HANK_HART)));
	unitList.push_back(Unit(CARD_CHAKKRAM, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram", FindEffect(SKILL_CHAKKRAM)));
	unitList.push_back(Unit(CARD_CHAOS_ELEMENTAL, FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental", FindEffect(SKILL_CHAOS_ELEMENTAL)));
	unitList.push_back(Unit(CARD_CRIMSON_OCULUS, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "crimsonoculus", "Crimson Oculus", FindEffect(SKILL_CRIMSON_OCULUS)));
	unitList.push_back(Unit(CARD_CROSSBONES, FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "crossbones", "Crossbones", FindEffect(SKILL_CROSSBONES)));
	unitList.push_back(Unit(CARD_DAGGER_KIRI, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 8, "daggerkiri", "Dagger Kiri", FindEffect(SKILL_CELERITY)));
	unitList.push_back(Unit(CARD_DARK_NEMESIS, FACTION_NEUTRAL, TRIBE_NONE, 7, 4, 10, "darknemesis", "Dark Nemesis", FindEffect(SKILL_DARK_NEMESIS)));
	unitList.push_back(Unit(CARD_DANCING_BLADES, FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 6, "dancingblades", "Dancing Blades", FindEffect(SKILL_DANCING_BLADES)));
	unitList.push_back(Unit(CARD_DAY_WATCHER, FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "daywatcher", "Day Watcher", FindEffect(SKILL_DAY_WATCHER)));
	unitList.push_back(Unit(CARD_DEATHBLIGHTER, FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "deathblighter", "Deathblighter", FindEffect(SKILL_DEATHBLIGHTER)));
	unitList.push_back(Unit(CARD_DECIMUS, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "decimus", "Decimus", FindEffect(SKILL_DECIMUS)));
	unitList.push_back(Unit(CARD_DIAMOND_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 6, 5, 11, "diamondgolem", "Diamond Golem"));
	unitList.push_back(Unit(CARD_DIOLTAS, FACTION_NEUTRAL, TRIBE_NONE, 4, 5, 3, "dioltas", "Dioltas", FindEffect(SKILL_DIOLTAS)));
	unitList.push_back(Unit(CARD_DRAGONLARK, FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark", FindEffect(SKILL_FLYING)));
	unitList.push_back(Unit(CARD_DREAMGAZER, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "dreamgazer", "Dreamgazer", FindEffect(SKILL_DREAMGAZER)));
	unitList.push_back(Unit(CARD_DRYBONE_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 7, 10, 10, "drybonegolem", "Drybone Golem"));
	unitList.push_back(Unit(CARD_DUST_WAILER, FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "dustwailer", "Dust Wailer", FindEffect(SKILL_DUST_WAILER)));
	unitList.push_back(Unit(CARD_ECLIPSE, FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 3, 7, "eclipse", "Eclipse", FindEffect(SKILL_ECLIPSE)));
	unitList.push_back(Unit(CARD_EMERALD_REJUVENATOR, FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "emeraldrejuvenator", "Emerald Rejuvenator", FindEffect(SKILL_EMERALD_REJUVENATOR)));
	unitList.push_back(Unit(CARD_ENVYBAER, FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 10, "envybaer", "Envybaer", FindEffect(SKILL_ENVYBAER)));
	unitList.push_back(Unit(CARD_EPHEMERAL_SHROUD, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "ephemeralshroud", "Ephemeral Shroud", FindEffect(SKILL_EPHEMERAL_SHROUD)));
	unitList.push_back(Unit(CARD_EXUN, FACTION_NEUTRAL, TRIBE_NONE, 7, 5, 5, "exun", "E'Xun", FindEffect(SKILL_EXUN)));
	unitList.push_back(Unit(CARD_FACESTRIKER, FACTION_NEUTRAL, TRIBE_NONE, 6, 4, 6, "facestriker", "Facestriker", FindEffect(SKILL_FACESTRIKER)));
	unitList.push_back(Unit(CARD_FIREBLAZER, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "fireblazer", "Fireblazer", FindEffect(SKILL_PROVOKE)));
	unitList.push_back(Unit(CARD_FIRESTARTER, FACTION_NEUTRAL, TRIBE_ARCANYST, 5, 3, 5, "firestarter", "Firestarter", FindEffect(SKILL_FIRESTARTER)));
	unitList.push_back(Unit(CARD_FIRE_SPITTER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "firespitter", "Fire Spitter", FindEffect(SKILL_RANGED)));
	unitList.push_back(Unit(CARD_FIRST_SWORD_OF_AKRANE, FACTION_NEUTRAL, TRIBE_NONE, 6, 7, 7, "firstswordofakrane", "First Sword of Akrane", FindEffect(SKILL_FIRST_SWORD_OF_AKRANE)));
	unitList.push_back(Unit(CARD_FLAMEBLOOD_WARLOCK, FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "flamebloodwarlock", "Flameblood Warlock", FindEffect(SKILL_FLAMEBLOOD_WARLOCK)));
	unitList.push_back(Unit(CARD_FROSTBONE_NAGA, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 3, "frostbonenaga", "Frostbone Naga", FindEffect(SKILL_FROSTBONE_NAGA)));
	unitList.push_back(Unit(CARD_GHOST_LYNX, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx", FindEffect(SKILL_GHOST_LYNX)));
	unitList.push_back(Unit(CARD_GOLDEN_JUSTICAR, FACTION_NEUTRAL, TRIBE_WARMASTER, 5, 4, 6, "goldenjusticar", "Golden Justicar", FindEffect(SKILL_GOLDEN_JUSTICAR)));
	unitList.push_back(Unit(CARD_GOLEM_METALLURGIST, FACTION_NEUTRAL, TRIBE_GOLEM, 2, 2, 3, "golemmetallurgist", "Golem Metallurgist", FindEffect(SKILL_GOLEM_METALLURGIST)));
	unitList.push_back(Unit(CARD_GOLEM_VANQUISHER, FACTION_NEUTRAL, TRIBE_GOLEM, 3, 2, 4, "golemvanquisher", "Golem Vanquisher", FindEffect(SKILL_GOLEM_VANQUISHER)));
	unitList.push_back(Unit(CARD_GROVE_LION, FACTION_NEUTRAL, TRIBE_NONE, 6, 5, 5, "grovelion", "Grove Lion", FindEffect(SKILL_GROVE_LION)));
	unitList.push_back(Unit(CARD_HAILSTONE_GOLEM, FACTION_NEUTRAL, TRIBE_GOLEM, 4, 4, 6, "hailstonegolem", "Hailstone Golem"));
	unitList.push_back(Unit(CARD_HEALING_MYSTIC, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "healingmystic", "Healing Mystic", FindEffect(SKILL_HEALING_MYSTIC)));
	unitList.push_back(Unit(CARD_HOLLOW_GROVEKEEPER, FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 4, "hollowgrovekeeper", "Hollow Grovekeper", FindEffect(SKILL_HOLLOW_GROVEKEEPER)));
	unitList.push_back(Unit(CARD_IRONCLAD, FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 3, "ironclad", "Ironclad", FindEffect(SKILL_IRONCLAD)));
	unitList.push_back(Unit(CARD_JAX_TRUESIGHT, FACTION_NEUTRAL, TRIBE_NONE, 6, 1, 1, "jaxtruesight", "Jax Truesight", FindEffect(SKILL_JAX_TRUESIGHT)));
	unitList.push_back(Unit(CARD_JAXI, FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "jaxi", "Jaxi", FindEffect(SKILL_JAXI)));
	unitList.push_back(Unit(CARD_KEEPER_OF_THE_VALE, FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 4, "keeperofthevale", "Keeper of the Vale", FindEffect(SKILL_KEEPER_OF_THE_VALE)));
	unitList.push_back(Unit(CARD_KHYMERA, FACTION_NEUTRAL, TRIBE_NONE, 8, 5, 12, "khymera", "Khymera", FindEffect(SKILL_KHYMERA)));
	unitList.push_back(Unit(CARD_KOMODO_CHARGER, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 3, "komodocharger", "Komodo Charger"));
	unitList.push_back(Unit(CARD_LADY_LOCKE, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "ladylocke", "Lady Locke", FindEffect(SKILL_LADY_LOCKE)));
	unitList.push_back(Unit(CARD_LIGHTBENDER, FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 3, 3, "lightbender", "Lightbender", FindEffect(SKILL_LIGHTBENDER)));
	unitList.push_back(Unit(CARD_LUX_IGNIS, FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 5, "luxignis", "Lux Ignis", FindEffect(SKILL_LUX_IGNIS)));
	unitList.push_back(Unit(CARD_MANAFORGER, FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "manaforger", "Manaforger", FindEffect(SKILL_MANAFORGER)));
	unitList.push_back(Unit(CARD_MAW, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 2, "maw", "Maw", FindEffect(SKILL_MAW)));
	unitList.push_back(Unit(CARD_MINDWARPER, FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 4, 3, "mindwarper", "Mindwarper", FindEffect(SKILL_MINDWARPER)));
	unitList.push_back(Unit(CARD_MIRKBLOOD_DEVOURER, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 4, "mirkblooddevourer", "Mirkblood Devourer", FindEffect(SKILL_MIRKBLOOD_DEVOURER)));
	unitList.push_back(Unit(CARD_MOGWAI, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "mogwai", "Mogwai", FindEffect(SKILL_MOGWAI)));
	unitList.push_back(Unit(CARD_NECROSEER, FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 4, "necroseer", "Necroseer", FindEffect(SKILL_NECROSEER)));
	unitList.push_back(Unit(CARD_NIGHT_WATCHER, FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "nightwatcher", "Night Watcher", FindEffect(SKILL_NIGHT_WATCHER)));
	unitList.push_back(Unit(CARD_OWLBEAST_SAGE, FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 4, 4, "owlbeastsage", "Owlbeast Sage", FindEffect(SKILL_OWLBEAST_SAGE)));
	unitList.push_back(Unit(CARD_PADDO, FACTION_NEUTRAL, TRIBE_NONE, 7, 12, 6, "paddo", "Paddo", FindEffect(SKILL_PADDO)));
	unitList.push_back(Unit(CARD_PIERCING_MANTIS, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 2, "piercingmantis", "Piercing Mantis", FindEffect(SKILL_FRENZY)));
	unitList.push_back(Unit(CARD_PLANAR_SCOUT, FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "planarscout", "Planar Scout", FindEffect(SKILL_AIRDROP)));
	unitList.push_back(Unit(CARD_PRIMUS_FIST, FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "primusfist", "Primus Fist", FindEffect(SKILL_PRIMUS_FIST)));
	unitList.push_back(Unit(CARD_PRIMUS_SHIELDMASTER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 6, "primusshieldmaster", "Primus Shieldmaster", FindEffect(SKILL_PROVOKE)));
	unitList.push_back(Unit(CARD_PRISMATIC_ILLUSIONIST, FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 2, 3, "prismaticillusionist", "Prismatic Illusionist", FindEffect(SKILL_PRISMATIC_ILLUSIONIST)));
	unitList.push_back(Unit(CARD_PURGATOS_THE_REALMKEEPER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 5, "purgatostherealmkeeper", "Purgatos, the Realmkeeper", FindEffect(SKILL_PURGATOS_THE_REALMKEEPER)));
	unitList.push_back(Unit(CARD_PUTRID_DREADFLAYER, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 4, "putriddreadflayer", "Putrid Dreadflayer", FindEffect(SKILL_FLYING)));
	unitList.push_back(Unit(CARD_SABERSPINE_TIGER, FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "saberspinetiger", "Saberspine Tiger", FindEffect(SKILL_RUSH)));
	unitList.push_back(Unit(CARD_SAPPHIRE_SEER, FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 2, "sapphireseer", "Sapphire Seer", FindEffect(SKILL_FORCEFIELD)));

	//Token Minions
	unitList.push_back(Unit(CARD_ILLUSION, FACTION_NEUTRAL, TRIBE_ARCANYST, 1, 2, 1, "illusion", "Illusion", true));
	unitList.push_back(Unit(CARD_MINI_JAX, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "minijax", "Mini-Jax", true, FindEffect(SKILL_RANGED)));
	unitList.push_back(Unit(CARD_SPELLSPARK, FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "spellspark", "Spellspark", true, FindEffect(SKILL_RUSH)));
	unitList.push_back(Unit(CARD_TOMBSTONE, FACTION_NEUTRAL, TRIBE_NONE, 3, 0, 10, "tombstone", "Tombstone", true, FindEffect(SKILL_PROVOKE)));

	//Spells
	spellList.push_back(Spell(CARD_BREATH_OF_THE_UNBORN, FACTION_ABYSSIAN, 4, "breathoftheunborn", "Breath of The Unborn", FindEffect(SPELL_BREATH_OF_THE_UNBORN)));
	spellList.back().SetTargetMode(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
		return tile->HasUnit();
	}));
	spellList.push_back(Spell(CARD_DARK_SEED, FACTION_ABYSSIAN, 4, "darkseed", "Dark Seed", FindEffect(SPELL_DARK_SEED)));
	spellList.back().SetTargetMode(TargetMode(TARGET_MODE_ALL, [](BoardTile* tile) {
		return tile->HasUnit() && tile->unit->IsEnemy() && tile->unit->tribe == TRIBE_GENERAL;
	}));

}