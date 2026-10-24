//Include
#include <ranges>
#include "card.h"
#include "carddefinition.h"
#include "collections.h"

//Register card objects
void Collections::RegisterCards() {

#pragma region Generals

	//Generals
	cardDefinitions.try_emplace(
		CARD_ARGEON_HIGHMAYNE, CARD_ARGEON_HIGHMAYNE, RARITY_NONE, FACTION_LYONAR, TRIBE_GENERAL,
		"argeonhighmayne", "Argeon Highmayne", 0, 2, 25, EFFECT_NONE );

#pragma endregion

#pragma region Minions

	//Minion Cards
	cardDefinitions.try_emplace(
		CARD_ABJUDICATOR, CARD_ABJUDICATOR, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"abjudicator", "Abjudicator", 3, 3, 1, SKILL_ABJUDICATOR );
	cardDefinitions.try_emplace(
		CARD_AETHERMASTER, CARD_AETHERMASTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"aethermaster", "Aethermaster", 2, 1, 3, SKILL_AETHERMASTER );
	cardDefinitions.try_emplace(
		CARD_ALCUIN_LOREMASTER, CARD_ALCUIN_LOREMASTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"alcuinloremaster", "Alcuin Loremaster", 3, 3, 1, SKILL_ALCUIN_LOREMASTER );
	cardDefinitions.try_emplace(
		CARD_ARAKI_HEADHUNTER, CARD_ARAKI_HEADHUNTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"arakiheadhunter", "Araki Headhunter", 2, 1, 3, SKILL_ARAKI_HEADHUNTER );
	cardDefinitions.try_emplace(
		CARD_ARCHON_SPELLBINDER, CARD_ARCHON_SPELLBINDER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"archonspellbinder", "Archon Spellbinder", 6, 7, 7, SKILL_ARCHON_SPELLBINDER );
	cardDefinitions.try_emplace(
		CARD_ARROW_WHISTLER, CARD_ARROW_WHISTLER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"arrowwhistler", "Arrow Whistler", 4, 2, 4, SKILL_ARROW_WHISTLER );
	cardDefinitions.try_emplace(
		CARD_ASH_MEPHYT, CARD_ASH_MEPHYT, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"ashmephyt", "Ash Mephyt", 5, 2, 3, SKILL_ASH_MEPHYT );
	cardDefinitions.try_emplace(
		CARD_ASTRAL_CRUSADER, CARD_ASTRAL_CRUSADER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"astralcrusader", "Astral Crusader", 7, 7, 6, SKILL_ASTRAL_CRUSADER );
	cardDefinitions.try_emplace(
		CARD_AZURE_HERALD, CARD_AZURE_HERALD, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"azureherald", "Azure Herald", 2, 1, 4, SKILL_AZURE_HERALD );
	cardDefinitions.try_emplace(
		CARD_AZURE_HORN_SHAMAN, CARD_AZURE_HORN_SHAMAN, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"azurehornshaman", "Azure Horn Shaman", 2, 1, 4, SKILL_AZURE_HORN_SHAMAN );
	cardDefinitions.try_emplace(
		CARD_BASTION, CARD_BASTION, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_STRUCTURE,
		"bastion", "Bastion", 3, 0, 5, SKILL_BASTION );
	cardDefinitions.try_emplace(
		CARD_BLACK_LOCUST, CARD_BLACK_LOCUST, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"blacklocust", "Black Locust", 4, 2, 2, SKILL_BLACK_LOCUST );
	cardDefinitions.try_emplace(
		CARD_BLAZE_HOUND, CARD_BLAZE_HOUND, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"blazehound", "Blaze Hound", 3, 4, 3, SKILL_BLAZE_HOUND );
	cardDefinitions.try_emplace(
		CARD_BLISTERING_SKORN, CARD_BLISTERING_SKORN, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"blisteringskorn", "Blistering Skorn", 4, 4, 5, SKILL_BLISTERING_SKORN );
	cardDefinitions.try_emplace(
		CARD_BLOOD_TAURA, CARD_BLOOD_TAURA, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"bloodtaura", "Blood Taura", 25, 12, 12, SKILL_BLOOD_TAURA );
	cardDefinitions.try_emplace(
		CARD_BLOODSHARD_GOLEM, CARD_BLOODSHARD_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"bloodshardgolem", "Bloodshard Golem", 3, 4, 3, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_BLOODTEAR_ALCHEMIST, CARD_BLOODTEAR_ALCHEMIST, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"bloodtearalchemist", "Bloodtear Alchemist", 1, 2, 1, SKILL_BLOODTEAR_ALCHEMIST );
	cardDefinitions.try_emplace(
		CARD_BLUETIP_SCORPION, CARD_BLUETIP_SCORPION, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"bluetipscorpion", "Bluetip Scorpion", 2, 3, 1, SKILL_BLUETIP_SCORPION );
	cardDefinitions.try_emplace(
		CARD_BONEREAPER, CARD_BONEREAPER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"bonereaper", "Bonereaper", 6, 2, 9, SKILL_BONEREAPER );
	cardDefinitions.try_emplace(
		CARD_BRIGHTMOSS_GOLEM, CARD_BRIGHTMOSS_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"brightmossgolem", "Brightmoss Golem", 5, 4, 9, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_CAPTAIN_HANK_HART, CARD_CAPTAIN_HANK_HART, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"captainhankhart", "Captain Hank Hart", 4, 2, 4, SKILL_CAPTAIN_HANK_HART );
	cardDefinitions.try_emplace(
		CARD_CHAKKRAM, CARD_CHAKKRAM, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"chakkram", "Chakkram", 5, 5, 5, SKILL_CHAKKRAM );
	cardDefinitions.try_emplace(
		CARD_CHAOS_ELEMENTAL, CARD_CHAOS_ELEMENTAL, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"chaoselemental", "Chaos Elemental", 3, 4, 4, SKILL_CHAOS_ELEMENTAL );
	cardDefinitions.try_emplace(
		CARD_CRIMSON_OCULUS, CARD_CRIMSON_OCULUS, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"crimsonoculus", "Crimson Oculus", 3, 2, 3, SKILL_CRIMSON_OCULUS );
	cardDefinitions.try_emplace(
		CARD_CROSSBONES, CARD_CROSSBONES, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"crossbones", "Crossbones", 3, 3, 3, SKILL_CROSSBONES );
	cardDefinitions.try_emplace(
		CARD_DAGGER_KIRI, CARD_DAGGER_KIRI, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"daggerkiri", "Dagger Kiri", 5, 2, 8, SKILL_CELERITY );
	cardDefinitions.try_emplace(
		CARD_DANCING_BLADES, CARD_DANCING_BLADES, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"dancingblades", "Dancing Blades", 5, 4, 6, SKILL_DANCING_BLADES );
	cardDefinitions.try_emplace(
		CARD_DARK_NEMESIS, CARD_DARK_NEMESIS, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"darknemesis", "Dark Nemesis", 7, 4, 10, SKILL_DARK_NEMESIS );
	cardDefinitions.try_emplace(
		CARD_DAY_WATCHER, CARD_DAY_WATCHER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"daywatcher", "Day Watcher", 3, 3, 3, SKILL_DAY_WATCHER );
	cardDefinitions.try_emplace(
		CARD_DEATHBLIGHTER, CARD_DEATHBLIGHTER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"deathblighter", "Deathblighter", 6, 3, 4, SKILL_DEATHBLIGHTER );
	cardDefinitions.try_emplace(
		CARD_DECIMUS, CARD_DECIMUS, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"decimus", "Decimus", 4, 4, 4, SKILL_DECIMUS );
	cardDefinitions.try_emplace(
		CARD_DIAMOND_GOLEM, CARD_DIAMOND_GOLEM, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_GOLEM,
		"diamondgolem", "Diamond Golem", 6, 5, 11, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_DIOLTAS, CARD_DIOLTAS, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"dioltas", "Dioltas", 4, 5, 3, SKILL_DIOLTAS );
	cardDefinitions.try_emplace(
		CARD_DRAGONLARK, CARD_DRAGONLARK, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"dragonlark", "Dragonlark", 1, 2, 1, SKILL_FLYING );
	cardDefinitions.try_emplace(
		CARD_DREAMGAZER, CARD_DREAMGAZER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"dreamgazer", "Dreamgazer", 1, 1, 1, SKILL_DREAMGAZER );
	cardDefinitions.try_emplace(
		CARD_DRYBONE_GOLEM, CARD_DRYBONE_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"drybonegolem", "Drybone Golem", 7, 10, 10, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_DUST_WAILER, CARD_DUST_WAILER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"dustwailer", "Dust Wailer", 6, 3, 4, SKILL_DUST_WAILER );
	cardDefinitions.try_emplace(
		CARD_EXUN, CARD_EXUN, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"exun", "E'Xun", 7, 5, 5, SKILL_EXUN );
	cardDefinitions.try_emplace(
		CARD_ECLIPSE, CARD_ECLIPSE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"eclipse", "Eclipse", 6, 3, 7, SKILL_ECLIPSE );
	cardDefinitions.try_emplace(
		CARD_EMERALD_REJUVENATOR, CARD_EMERALD_REJUVENATOR, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"emeraldrejuvenator", "Emerald Rejuvenator", 4, 4, 4, SKILL_EMERALD_REJUVENATOR );
	cardDefinitions.try_emplace(
		CARD_ENVYBAER, CARD_ENVYBAER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"envybaer", "Envybaer", 5, 3, 10, SKILL_ENVYBAER );
	cardDefinitions.try_emplace(
		CARD_EPHEMERAL_SHROUD, CARD_EPHEMERAL_SHROUD, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"ephemeralshroud", "Ephemeral Shroud", 2, 1, 1, SKILL_EPHEMERAL_SHROUD );
	cardDefinitions.try_emplace(
		CARD_FACESTRIKER, CARD_FACESTRIKER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"facestriker", "Facestriker", 6, 4, 6, SKILL_FACESTRIKER );
	cardDefinitions.try_emplace(
		CARD_FIRE_SPITTER, CARD_FIRE_SPITTER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"firespitter", "Fire Spitter", 4, 3, 2, SKILL_RANGED );
	cardDefinitions.try_emplace(
		CARD_FIREBLAZER, CARD_FIREBLAZER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"fireblazer", "Fireblazer", 5, 5, 5, SKILL_PROVOKE );
	cardDefinitions.try_emplace(
		CARD_FIRESTARTER, CARD_FIRESTARTER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"firestarter", "Firestarter", 5, 3, 5, SKILL_FIRESTARTER );
	cardDefinitions.try_emplace(
		CARD_FIRST_SWORD_OF_AKRANE, CARD_FIRST_SWORD_OF_AKRANE, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"firstswordofakrane", "First Sword of Akrane", 6, 7, 7, SKILL_FIRST_SWORD_OF_AKRANE );
	cardDefinitions.try_emplace(
		CARD_FLAMEBLOOD_WARLOCK, CARD_FLAMEBLOOD_WARLOCK, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"flamebloodwarlock", "Flameblood Warlock", 2, 3, 1, SKILL_FLAMEBLOOD_WARLOCK );
	cardDefinitions.try_emplace(
		CARD_FROSTBONE_NAGA, CARD_FROSTBONE_NAGA, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"frostbonenaga", "Frostbone Naga", 4, 3, 3, SKILL_FROSTBONE_NAGA );
	cardDefinitions.try_emplace(
		CARD_GHOST_LYNX, CARD_GHOST_LYNX, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"ghostlynx", "Ghost Lynx", 2, 2, 1, SKILL_GHOST_LYNX );
	cardDefinitions.try_emplace(
		CARD_GOLDEN_JUSTICAR, CARD_GOLDEN_JUSTICAR, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"goldenjusticar", "Golden Justicar", 5, 4, 6, SKILL_GOLDEN_JUSTICAR );
	cardDefinitions.try_emplace(
		CARD_GOLEM_METALLURGIST, CARD_GOLEM_METALLURGIST, RARITY_RARE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"golemmetallurgist", "Golem Metallurgist", 2, 2, 3, SKILL_GOLEM_METALLURGIST );
	cardDefinitions.try_emplace(
		CARD_GOLEM_VANQUISHER, CARD_GOLEM_VANQUISHER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_GOLEM,
		"golemvanquisher", "Golem Vanquisher", 3, 2, 4, SKILL_GOLEM_VANQUISHER );
	cardDefinitions.try_emplace(
		CARD_GROVE_LION, CARD_GROVE_LION, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"grovelion", "Grove Lion", 6, 5, 5, SKILL_GROVE_LION );
	cardDefinitions.try_emplace(
		CARD_HAILSTONE_GOLEM, CARD_HAILSTONE_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"hailstonegolem", "Hailstone Golem", 4, 4, 6, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_HEALING_MYSTIC, CARD_HEALING_MYSTIC, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"healingmystic", "Healing Mystic", 2, 2, 3, SKILL_HEALING_MYSTIC );
	cardDefinitions.try_emplace(
		CARD_HOLLOW_GROVEKEEPER, CARD_HOLLOW_GROVEKEEPER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"hollowgrovekeeper", "Hollow Grovekeper", 5, 3, 4, SKILL_HOLLOW_GROVEKEEPER );
	cardDefinitions.try_emplace(
		CARD_IRONCLAD, CARD_IRONCLAD, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"ironclad", "Ironclad", 5, 4, 3, SKILL_IRONCLAD );
	cardDefinitions.try_emplace(
		CARD_JAX_TRUESIGHT, CARD_JAX_TRUESIGHT, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"jaxtruesight", "Jax Truesight", 6, 1, 1, SKILL_JAX_TRUESIGHT );
	cardDefinitions.try_emplace(
		CARD_JAXI, CARD_JAXI, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"jaxi", "Jaxi", 2, 1, 1, SKILL_JAXI );
	cardDefinitions.try_emplace(
		CARD_KEEPER_OF_THE_VALE, CARD_KEEPER_OF_THE_VALE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"keeperofthevale", "Keeper of the Vale", 5, 3, 4, SKILL_KEEPER_OF_THE_VALE );
	cardDefinitions.try_emplace(
		CARD_KHYMERA, CARD_KHYMERA, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"khymera", "Khymera", 8, 5, 12, SKILL_KHYMERA );
	cardDefinitions.try_emplace(
		CARD_KOMODO_CHARGER, CARD_KOMODO_CHARGER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"komodocharger", "Komodo Charger", 1, 1, 3, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_LADY_LOCKE, CARD_LADY_LOCKE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"ladylocke", "Lady Locke", 3, 2, 3, SKILL_LADY_LOCKE );
	cardDefinitions.try_emplace(
		CARD_LIGHTBENDER, CARD_LIGHTBENDER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"lightbender", "Lightbender", 4, 3, 3, SKILL_LIGHTBENDER );
	cardDefinitions.try_emplace(
		CARD_LUX_IGNIS, CARD_LUX_IGNIS, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"luxignis", "Lux Ignis", 5, 2, 5, SKILL_LUX_IGNIS );
	cardDefinitions.try_emplace(
		CARD_MANAFORGER, CARD_MANAFORGER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"manaforger", "Manaforger", 2, 1, 3, SKILL_MANAFORGER );
	cardDefinitions.try_emplace(
		CARD_MAW, CARD_MAW, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"maw", "Maw", 2, 2, 2, SKILL_MAW );
	cardDefinitions.try_emplace(
		CARD_MINDWARPER, CARD_MINDWARPER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"mindwarper", "Mindwarper", 4, 4, 3, SKILL_MINDWARPER );
	cardDefinitions.try_emplace(
		CARD_MIRKBLOOD_DEVOURER, CARD_MIRKBLOOD_DEVOURER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"mirkblooddevourer", "Mirkblood Devourer", 3, 2, 4, SKILL_MIRKBLOOD_DEVOURER );
	cardDefinitions.try_emplace(
		CARD_MOEBIUS, CARD_MOEBIUS, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"moebius", "Moebius", 4, 3, 5, SKILL_MOEBIUS );
	cardDefinitions.try_emplace(
		CARD_MOGWAI, CARD_MOGWAI, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"mogwai", "Mogwai", 3, 2, 3, SKILL_MOGWAI );
	cardDefinitions.try_emplace(
		CARD_NECROSEER, CARD_NECROSEER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"necroseer", "Necroseer", 5, 5, 4, SKILL_NECROSEER );
	cardDefinitions.try_emplace(
		CARD_NIGHT_WATCHER, CARD_NIGHT_WATCHER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"nightwatcher", "Night Watcher", 4, 2, 4, SKILL_NIGHT_WATCHER );
	cardDefinitions.try_emplace(
		CARD_OWLBEAST_SAGE, CARD_OWLBEAST_SAGE, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"owlbeastsage", "Owlbeast Sage", 4, 4, 4, SKILL_OWLBEAST_SAGE );
	cardDefinitions.try_emplace(
		CARD_PADDO, CARD_PADDO, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"paddo", "Paddo", 7, 12, 6, SKILL_PADDO );
	cardDefinitions.try_emplace(
		CARD_PIERCING_MANTIS, CARD_PIERCING_MANTIS, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"piercingmantis", "Piercing Mantis", 2, 2, 2, SKILL_FRENZY );
	cardDefinitions.try_emplace(
		CARD_PLANAR_SCOUT, CARD_PLANAR_SCOUT, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"planarscout", "Planar Scout", 1, 2, 1, SKILL_AIRDROP );
	cardDefinitions.try_emplace(
		CARD_PRIMUS_FIST, CARD_PRIMUS_FIST, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"primusfist", "Primus Fist", 2, 2, 3, SKILL_PRIMUS_FIST );
	cardDefinitions.try_emplace(
		CARD_PRIMUS_SHIELDMASTER, CARD_PRIMUS_SHIELDMASTER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"primusshieldmaster", "Primus Shieldmaster", 4, 3, 6, SKILL_PROVOKE );
	cardDefinitions.try_emplace(
		CARD_PRISMATIC_ILLUSIONIST, CARD_PRISMATIC_ILLUSIONIST, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"prismaticillusionist", "Prismatic Illusionist", 3, 2, 3, SKILL_PRISMATIC_ILLUSIONIST );
	cardDefinitions.try_emplace(
		CARD_PROPHET_OF_THE_WHITE_PALM, CARD_PROPHET_OF_THE_WHITE_PALM, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"prophetofthewhitepalm", "Prophet of the White Palm", 1, 1, 1, SKILL_PROPHET_OF_THE_WHITE_PALM );
	cardDefinitions.try_emplace(
		CARD_PURGATOS_THE_REALMKEEPER, CARD_PURGATOS_THE_REALMKEEPER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"purgatostherealmkeeper", "Purgatos, the Realmkeeper", 4, 3, 5, SKILL_PURGATOS_THE_REALMKEEPER );
	cardDefinitions.try_emplace(
		CARD_PUTRID_DREADFLAYER, CARD_PUTRID_DREADFLAYER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"putriddreadflayer", "Putrid Dreadflayer", 3, 2, 4, SKILL_FLYING );
	cardDefinitions.try_emplace(
		CARD_QUARTERMASTER_GAUJ, CARD_QUARTERMASTER_GAUJ, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"quartermastergauj", "Quartermaster Gauj", 6, 5, 2, SKILL_QUARTERMASTER_GAUJ );
	cardDefinitions.try_emplace(
		CARD_RED_SYNJA, CARD_RED_SYNJA, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"redsynja", "Red Synja", 7, 7, 7, SKILL_RED_SYNJA );
	cardDefinitions.try_emplace(
		CARD_REPULSOR_BEAST, CARD_REPULSOR_BEAST, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"repulsorbeast", "Repulsor Beast", 3, 2, 2, SKILL_REPULSOR_BEAST );
	cardDefinitions.try_emplace(
		CARD_ROCK_PULVERIZER, CARD_ROCK_PULVERIZER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"rockpulverizer", "Rock Pulverizer", 2, 1, 4, SKILL_PROVOKE );
	cardDefinitions.try_emplace(
		CARD_ROGUE_WARDEN, CARD_ROGUE_WARDEN, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"roguewarden", "Rogue Warden", 5, 4, 3, SKILL_RANGED );
	cardDefinitions.try_emplace(
		CARD_RUBY_RIFTER, CARD_RUBY_RIFTER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"rubyrifter", "Ruby Rifter", 6, 4, 6, SKILL_RUBY_RIFTER );
	cardDefinitions.try_emplace(
		CARD_SABERSPINE_TIGER, CARD_SABERSPINE_TIGER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"saberspinetiger", "Saberspine Tiger", 4, 3, 2, SKILL_RUSH );
	cardDefinitions.try_emplace(
		CARD_SAPPHIRE_SEER, CARD_SAPPHIRE_SEER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"sapphireseer", "Sapphire Seer", 3, 2, 2, SKILL_FORCEFIELD );
	cardDefinitions.try_emplace(
		CARD_SARLAC_THE_ETERNAL, CARD_SARLAC_THE_ETERNAL, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"sarlactheeternal", "Sarlac the Eternal", 3, 1, 1, SKILL_SARLAC_THE_ETERNAL );
	cardDefinitions.try_emplace(
		CARD_SERPENTI, CARD_SERPENTI, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"serpenti", "Serpenti", 6, 7, 4, SKILL_FRENZY );
	cardDefinitions.try_emplace(
		CARD_SHIRO_PUPPYDRAGON, CARD_SHIRO_PUPPYDRAGON, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"shiropuppydragon", "Shiro Puppydragon", 2, 1, 4, SKILL_SHIRO_PUPPYDRAGON );
	cardDefinitions.try_emplace(
		CARD_SILHOUETTE_TRACER, CARD_SILHOUETTE_TRACER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"silhouettetracer", "Silhouette Tracer", 4, 2, 6, SKILL_SILHOUETTE_TRACER );
	cardDefinitions.try_emplace(
		CARD_SILVERTONGUE_CORSAIR, CARD_SILVERTONGUE_CORSAIR, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"silvertonguecorsair", "Silvertongue Corsair", 3, 3, 3, SKILL_SILVERTONGUE_CORSAIR );
	cardDefinitions.try_emplace(
		CARD_SKYROCK_GOLEM, CARD_SKYROCK_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"skyrockgolem", "Skyrock Golem", 2, 3, 2, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_SKYWING, CARD_SKYWING, RARITY_RARE, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"skywing", "Skywing", 3, 3, 3, SKILL_SKYWING );
	cardDefinitions.try_emplace(
		CARD_SOJOURNER, CARD_SOJOURNER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"sojourner", "Sojourner", 3, 1, 5, SKILL_SOJOURNER );
	cardDefinitions.try_emplace(
		CARD_SONGWEAVER, CARD_SONGWEAVER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"songweaver", "Songweaver", 3, 3, 3, SKILL_SONGWEAVER );
	cardDefinitions.try_emplace(
		CARD_SPELLJAMMER, CARD_SPELLJAMMER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"spelljammer", "Spelljammer", 4, 3, 5, SKILL_SPELLJAMMER );
	cardDefinitions.try_emplace(
		CARD_STORM_ARATHA, CARD_STORM_ARATHA, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"stormaratha", "Storm Aratha", 6, 6, 5, SKILL_FLYING );
	cardDefinitions.try_emplace(
		CARD_STORMMETAL_GOLEM, CARD_STORMMETAL_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"stormmetalgolem", "Stormmetal Golem", 6, 8, 8, EFFECT_NONE );
	cardDefinitions.try_emplace(
		CARD_SUN_ELEMENTAL, CARD_SUN_ELEMENTAL, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"sunelemental", "Sun Elemental", 4, 3, 3, SKILL_SUN_ELEMENTAL );
	cardDefinitions.try_emplace(
		CARD_SUN_SEER, CARD_SUN_SEER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"sunseer", "Sun Seer", 3, 2, 4, SKILL_SUN_SEER );
	cardDefinitions.try_emplace(
		CARD_SUNSET_PARAGON, CARD_SUNSET_PARAGON, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"sunsetparagon", "Sunset Paragon", 5, 3, 2, SKILL_SUNSET_PARAGON );
	cardDefinitions.try_emplace(
		CARD_SUNSTEEL_DEFENDER, CARD_SUNSTEEL_DEFENDER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"sunsteeldefender", "Sunsteel Defender", 4, 3, 3, SKILL_FORCEFIELD );
	cardDefinitions.try_emplace(
		CARD_SWAMP_ENTANGLER, CARD_SWAMP_ENTANGLER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"swampentangler", "Swamp Entangler", 1, 0, 3, SKILL_PROVOKE );
	cardDefinitions.try_emplace(
		CARD_SWORN_AVENGER, CARD_SWORN_AVENGER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"swornavenger", "Sworn Avenger", 3, 1, 3, SKILL_SWORN_AVENGER);
	cardDefinitions.try_emplace(
		CARD_SWORN_DEFENDER, CARD_SWORN_DEFENDER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"sworndefender", "Sworn Defender", 5, 4, 7, SKILL_SWORN_DEFENDER);
	cardDefinitions.try_emplace(
		CARD_SYVREL_THE_EXILE, CARD_SYVREL_THE_EXILE, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"syvreltheexile", "Syvrel the Exile", 3, 1, 4, SKILL_SYVREL_THE_EXILE);
	cardDefinitions.try_emplace(
		CARD_TETHERMANCER, CARD_TETHERMANCER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"tethermancer", "Tethermancer", 4, 1, 6, SKILL_TETHERMANCER);
	cardDefinitions.try_emplace(
		CARD_THE_HIGH_HAND, CARD_THE_HIGH_HAND, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"thehighhand", "The High Hand", 5, 2, 3, SKILL_THE_HIGH_HAND);
	cardDefinitions.try_emplace(
		CARD_THORN_NEEDLER, CARD_THORN_NEEDLER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"thornneedler", "Thorn Needler", 4, 6, 4, EFFECT_NONE);
	cardDefinitions.try_emplace(
		CARD_TWILIGHT_SORCERER, CARD_TWILIGHT_SORCERER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"twilightsorcerer", "Twilight Sorcerer", 5, 3, 6, SKILL_TWILIGHT_SORCERER);
	cardDefinitions.try_emplace(
		CARD_UNSEVEN, CARD_UNSEVEN, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"unseven", "Unseven", 4, 2, 4, SKILL_UNSEVEN);
	cardDefinitions.try_emplace(
		CARD_VALE_HUNTER, CARD_VALE_HUNTER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"valehunter", "Vale Hunter", 2, 1, 2, SKILL_RANGED);
	cardDefinitions.try_emplace(
		CARD_VENOM_TOTH, CARD_VENOM_TOTH, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"venomtoth", "Venom Toth", 3, 3, 3, SKILL_VENOM_TOTH);
	cardDefinitions.try_emplace(
		CARD_VOID_HUNTER, CARD_VOID_HUNTER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"voidhunter", "Void Hunter", 3, 4, 2, SKILL_VOID_HUNTER);
	cardDefinitions.try_emplace(
		CARD_WAR_TALON, CARD_WAR_TALON, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"wartalon", "War Talon", 7, 4, 9, SKILL_WAR_TALON);
	cardDefinitions.try_emplace(
		CARD_WHISTLING_BLADE, CARD_WHISTLING_BLADE, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"whistlingblade", "Whistling Blade", 7, 2, 15, SKILL_PROVOKE);
	cardDefinitions.try_emplace(
		CARD_WHITE_WIDOW, CARD_WHITE_WIDOW, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"whitewidow", "White Widow", 4, 3, 4, SKILL_WHITE_WIDOW);
	cardDefinitions.try_emplace(
		CARD_WIND_RUNNER, CARD_WIND_RUNNER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"windrunner", "Wind Runner", 3, 3, 3, SKILL_WIND_RUNNER);
	cardDefinitions.try_emplace(
		CARD_WINGS_OF_PARADISE, CARD_WINGS_OF_PARADISE, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"wingsofparadise", "Wings of Paradise", 3, 3, 3, SKILL_WINGS_OF_PARADISE);
	cardDefinitions.try_emplace(
		CARD_WOODWEN, CARD_WOODWEN, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"woodwen", "Wood-Wen", 2, 2, 2, SKILL_WOODWEN);
	cardDefinitions.try_emplace(
		CARD_YOUNG_FLAMEWING, CARD_YOUNG_FLAMEWING, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"youngflamewing", "Young Flamewing", 4, 5, 4, SKILL_FLYING);
	cardDefinitions.try_emplace(
		CARD_ZURAEL_THE_LIFEGIVER, CARD_ZURAEL_THE_LIFEGIVER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"zuraelthelifegiver", "Zurael, the Lifegiver", 7, 4, 7, SKILL_ZURAEL_THE_LIFEGIVER);
	cardDefinitions.try_emplace(
		CARD_ZYX, CARD_ZYX, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"zyx", "Zyx", 2, 1, 2, SKILL_ZYX);

#pragma endregion

#pragma region Tokens

	//Tokens
	cardDefinitions.try_emplace(
		CARD_ILLUSION, CARD_ILLUSION, RARITY_NONE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"illusion", "Illusion", 1, 2, 1, EFFECT_NONE );
	cardDefinitions.at(CARD_ILLUSION).isToken = true;
	cardDefinitions.try_emplace(
		CARD_MINI_JAX, CARD_MINI_JAX, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"minijax", "Mini-Jax", 1, 1, 1, SKILL_RANGED );
	cardDefinitions.at(CARD_MINI_JAX).isToken = true;
	cardDefinitions.try_emplace(
		CARD_SPELLSPARK, CARD_SPELLSPARK, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"spellspark", "Spellspark", 1, 1, 1, SKILL_RUSH );
	cardDefinitions.at(CARD_SPELLSPARK).isToken = true;
	cardDefinitions.try_emplace(
		CARD_TOMBSTONE, CARD_TOMBSTONE, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"tombstone", "Tombstone", 3, 0, 10, SKILL_PROVOKE );
	cardDefinitions.at(CARD_TOMBSTONE).isToken = true;
	cardDefinitions.try_emplace(
		CARD_WRAITHLING, CARD_WRAITHLING, RARITY_NONE, FACTION_ABYSSIAN, TRIBE_NONE,
		"wraithling", "Wraithling", 1, 1, 1, EFFECT_NONE);
	cardDefinitions.at(CARD_WRAITHLING).isToken = true;

#pragma endregion

#pragma region Spells

	//Spells
	cardDefinitions.try_emplace(
		CARD_BREATH_OF_THE_UNBORN, CARD_BREATH_OF_THE_UNBORN, RARITY_COMMON, FACTION_ABYSSIAN,
		"breathoftheunborn", "Breath of The Unborn", 4, SPELL_BREATH_OF_THE_UNBORN );
	cardDefinitions.try_emplace(
		CARD_CONSUMING_REBIRTH, CARD_CONSUMING_REBIRTH, RARITY_EPIC, FACTION_ABYSSIAN,
		"consumingrebirth", "Consuming Rebirth", 2, SPELL_CONSUMING_REBIRTH );
	cardDefinitions.try_emplace(
		CARD_DAEMONIC_LURE, CARD_DAEMONIC_LURE, RARITY_NONE, FACTION_ABYSSIAN,
		"daemoniclure", "Daemonic Lure", 2, SPELL_DAEMONIC_LURE);
	cardDefinitions.try_emplace(
		CARD_DARK_SEED, CARD_DARK_SEED, RARITY_RARE, FACTION_ABYSSIAN,
		"darkseed", "Dark Seed", 4, SPELL_DARK_SEED );
	cardDefinitions.try_emplace(
		CARD_DARK_TRANSFORMATION, CARD_DARK_TRANSFORMATION, RARITY_NONE, FACTION_ABYSSIAN,
		"darktransformation", "Dark Transformation", 5, SPELL_DARK_TRANSFORMATION);
	cardDefinitions.try_emplace(
		CARD_DARKFIRE_SACRIFICE, CARD_DARKFIRE_SACRIFICE, RARITY_RARE, FACTION_ABYSSIAN,
		"darkfiresacrifice", "Darkfire Sacrifice", 0, SPELL_DARKFIRE_SACRIFICE);

#pragma endregion

#pragma region Tiles

	//Tiles
	cardDefinitions.try_emplace(
		CARD_MANA_SPRING, CARD_MANA_SPRING, RARITY_NONE, FACTION_NEUTRAL,
		"manaspring", "Mana Spring", TILE_MANA_SPRING);
	cardDefinitions.try_emplace(
		CARD_MANA_SPRING_EMPTY, CARD_MANA_SPRING_EMPTY, RARITY_NONE, FACTION_NEUTRAL,
		"manaspringempty", "Empty Mana Spring", EFFECT_NONE);

#pragma endregion

}