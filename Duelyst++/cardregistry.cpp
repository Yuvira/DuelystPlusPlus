//Include
#include <ranges>
#include "card.h"
#include "carddefinition.h"
#include "collections.h"
#include "tile.h"

//Register card objects
void Collections::RegisterCards() {

#pragma region Generals

	//Argeon Highmayne
	cardDefinitions.try_emplace(
		CARD_ARGEON_HIGHMAYNE, CARD_ARGEON_HIGHMAYNE, RARITY_NONE, FACTION_LYONAR, TRIBE_GENERAL,
		"argeonhighmayne", "Argeon Highmayne",
		0, 2, 25, EFFECT_NONE
	);

#pragma endregion

#pragma region Minions

	//Abjudicator
	cardDefinitions.try_emplace(
		CARD_ABJUDICATOR, CARD_ABJUDICATOR, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"abjudicator", "Abjudicator",
		3, 3, 1, SKILL_ABJUDICATOR
	);

	//Aethermaster
	cardDefinitions.try_emplace(
		CARD_AETHERMASTER, CARD_AETHERMASTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"aethermaster", "Aethermaster",
		2, 1, 3, SKILL_AETHERMASTER
	);

	//Alcuin Loremaster
	cardDefinitions.try_emplace(
		CARD_ALCUIN_LOREMASTER, CARD_ALCUIN_LOREMASTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"alcuinloremaster", "Alcuin Loremaster",
		3, 3, 1, SKILL_ALCUIN_LOREMASTER
	);

	//Araki Headhunter
	cardDefinitions.try_emplace(
		CARD_ARAKI_HEADHUNTER, CARD_ARAKI_HEADHUNTER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"arakiheadhunter", "Araki Headhunter",
		2, 1, 3, SKILL_ARAKI_HEADHUNTER
	);

	//Archon Spellbinder
	cardDefinitions.try_emplace(
		CARD_ARCHON_SPELLBINDER, CARD_ARCHON_SPELLBINDER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"archonspellbinder", "Archon Spellbinder",
		6, 7, 7, SKILL_ARCHON_SPELLBINDER
	);

	//Arrow Whistler
	cardDefinitions.try_emplace(
		CARD_ARROW_WHISTLER, CARD_ARROW_WHISTLER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"arrowwhistler", "Arrow Whistler",
		4, 2, 4, SKILL_ARROW_WHISTLER
	);

	//Ash Mephyt
	cardDefinitions.try_emplace(
		CARD_ASH_MEPHYT, CARD_ASH_MEPHYT, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"ashmephyt", "Ash Mephyt",
		5, 2, 3, SKILL_ASH_MEPHYT
	);

	//Astral Crusader
	cardDefinitions.try_emplace(
		CARD_ASTRAL_CRUSADER, CARD_ASTRAL_CRUSADER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"astralcrusader", "Astral Crusader",
		7, 7, 6, SKILL_ASTRAL_CRUSADER
	);

	//Azure Herald
	cardDefinitions.try_emplace(
		CARD_AZURE_HERALD, CARD_AZURE_HERALD, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"azureherald", "Azure Herald",
		2, 1, 4, SKILL_AZURE_HERALD
	);

	//Azure Horn Shaman
	cardDefinitions.try_emplace(
		CARD_AZURE_HORN_SHAMAN, CARD_AZURE_HORN_SHAMAN, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"azurehornshaman", "Azure Horn Shaman",
		2, 1, 4, SKILL_AZURE_HORN_SHAMAN
	);

	//Bastion
	cardDefinitions.try_emplace(
		CARD_BASTION, CARD_BASTION, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_STRUCTURE,
		"bastion", "Bastion",
		3, 0, 5, SKILL_BASTION
	);

	//Black Locust
	cardDefinitions.try_emplace(
		CARD_BLACK_LOCUST, CARD_BLACK_LOCUST, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"blacklocust", "Black Locust",
		4, 2, 2, SKILL_BLACK_LOCUST
	);

	//Blaze Hound
	cardDefinitions.try_emplace(
		CARD_BLAZE_HOUND, CARD_BLAZE_HOUND, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"blazehound", "Blaze Hound",
		3, 4, 3, SKILL_BLAZE_HOUND
	);

	//Blistering Skorn
	cardDefinitions.try_emplace(
		CARD_BLISTERING_SKORN, CARD_BLISTERING_SKORN, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"blisteringskorn", "Blistering Skorn",
		4, 4, 5, SKILL_BLISTERING_SKORN
	);

	//Blood Taura
	cardDefinitions.try_emplace(
		CARD_BLOOD_TAURA, CARD_BLOOD_TAURA, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"bloodtaura", "Blood Taura",
		25, 12, 12, SKILL_BLOOD_TAURA
	);

	//Bloodshard Golem
	cardDefinitions.try_emplace(
		CARD_BLOODSHARD_GOLEM, CARD_BLOODSHARD_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"bloodshardgolem", "Bloodshard Golem",
		3, 4, 3, EFFECT_NONE
	);

	//Bloodtear Alchemist
	cardDefinitions.try_emplace(
		CARD_BLOODTEAR_ALCHEMIST, CARD_BLOODTEAR_ALCHEMIST, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"bloodtearalchemist", "Bloodtear Alchemist",
		1, 2, 1, SKILL_BLOODTEAR_ALCHEMIST
	);

	//Bluetip Scorpion
	cardDefinitions.try_emplace(
		CARD_BLUETIP_SCORPION, CARD_BLUETIP_SCORPION, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"bluetipscorpion", "Bluetip Scorpion",
		2, 3, 1, SKILL_BLUETIP_SCORPION
	);

	//Bonereaper
	cardDefinitions.try_emplace(
		CARD_BONEREAPER, CARD_BONEREAPER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"bonereaper", "Bonereaper",
		6, 2, 9, SKILL_BONEREAPER
	);

	//Brightmoss Golem
	cardDefinitions.try_emplace(
		CARD_BRIGHTMOSS_GOLEM, CARD_BRIGHTMOSS_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"brightmossgolem", "Brightmoss Golem",
		5, 4, 9, EFFECT_NONE
	);

	//Captain Hank Hart
	cardDefinitions.try_emplace(
		CARD_CAPTAIN_HANK_HART, CARD_CAPTAIN_HANK_HART, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"captainhankhart", "Captain Hank Hart",
		4, 2, 4, SKILL_CAPTAIN_HANK_HART
	);

	//Chakkram
	cardDefinitions.try_emplace(
		CARD_CHAKKRAM, CARD_CHAKKRAM, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"chakkram", "Chakkram",
		5, 5, 5, SKILL_CHAKKRAM
	);

	//Chaos Elemental
	cardDefinitions.try_emplace(
		CARD_CHAOS_ELEMENTAL, CARD_CHAOS_ELEMENTAL, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"chaoselemental", "Chaos Elemental",
		3, 4, 4, SKILL_CHAOS_ELEMENTAL
	);

	//Crimson Oculus
	cardDefinitions.try_emplace(
		CARD_CRIMSON_OCULUS, CARD_CRIMSON_OCULUS, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"crimsonoculus", "Crimson Oculus",
		3, 2, 3, SKILL_CRIMSON_OCULUS
	);

	//Crossbones
	cardDefinitions.try_emplace(
		CARD_CROSSBONES, CARD_CROSSBONES, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"crossbones", "Crossbones",
		3, 3, 3, SKILL_CROSSBONES
	);

	//Dagger Kiri
	cardDefinitions.try_emplace(
		CARD_DAGGER_KIRI, CARD_DAGGER_KIRI, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"daggerkiri", "Dagger Kiri",
		5, 2, 8, SKILL_CELERITY
	);

	//Dancing Blades
	cardDefinitions.try_emplace(
		CARD_DANCING_BLADES, CARD_DANCING_BLADES, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"dancingblades", "Dancing Blades",
		5, 4, 6, SKILL_DANCING_BLADES
	);

	//Dark Nemesis
	cardDefinitions.try_emplace(
		CARD_DARK_NEMESIS, CARD_DARK_NEMESIS, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"darknemesis", "Dark Nemesis",
		7, 4, 10, SKILL_DARK_NEMESIS
	);

	//Day Watcher
	cardDefinitions.try_emplace(
		CARD_DAY_WATCHER, CARD_DAY_WATCHER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"daywatcher", "Day Watcher",
		3, 3, 3, SKILL_DAY_WATCHER
	);

	//Deathblighter
	cardDefinitions.try_emplace(
		CARD_DEATHBLIGHTER, CARD_DEATHBLIGHTER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"deathblighter", "Deathblighter",
		6, 3, 4, SKILL_DEATHBLIGHTER
	);

	//Decimus
	cardDefinitions.try_emplace(
		CARD_DECIMUS, CARD_DECIMUS, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"decimus", "Decimus",
		4, 4, 4, SKILL_DECIMUS
	);

	//Diamond Golem
	cardDefinitions.try_emplace(
		CARD_DIAMOND_GOLEM, CARD_DIAMOND_GOLEM, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_GOLEM,
		"diamondgolem", "Diamond Golem",
		6, 5, 11, EFFECT_NONE
	);

	//Dioltas
	cardDefinitions.try_emplace(
		CARD_DIOLTAS, CARD_DIOLTAS, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"dioltas", "Dioltas",
		4, 5, 3, SKILL_DIOLTAS
	);

	//Dragonlark
	cardDefinitions.try_emplace(
		CARD_DRAGONLARK, CARD_DRAGONLARK, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"dragonlark", "Dragonlark",
		1, 2, 1, SKILL_FLYING
	);

	//Dreamgazer
	cardDefinitions.try_emplace(
		CARD_DREAMGAZER, CARD_DREAMGAZER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"dreamgazer", "Dreamgazer",
		1, 1, 1, SKILL_DREAMGAZER
	);

	//Drybone Golem
	cardDefinitions.try_emplace(
		CARD_DRYBONE_GOLEM, CARD_DRYBONE_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"drybonegolem", "Drybone Golem",
		7, 10, 10, EFFECT_NONE
	);

	//Dust Wailer
	cardDefinitions.try_emplace(
		CARD_DUST_WAILER, CARD_DUST_WAILER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"dustwailer", "Dust Wailer",
		6, 3, 4, SKILL_DUST_WAILER
	);

	//E'Xun
	cardDefinitions.try_emplace(
		CARD_EXUN, CARD_EXUN, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"exun", "E'Xun",
		7, 5, 5, SKILL_EXUN
	);

	//Eclipse
	cardDefinitions.try_emplace(
		CARD_ECLIPSE, CARD_ECLIPSE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"eclipse", "Eclipse",
		6, 3, 7, SKILL_ECLIPSE
	);

	//Emerald Rejuvenator
	cardDefinitions.try_emplace(
		CARD_EMERALD_REJUVENATOR, CARD_EMERALD_REJUVENATOR, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"emeraldrejuvenator", "Emerald Rejuvenator",
		4, 4, 4, SKILL_EMERALD_REJUVENATOR
	);

	//Envybaer
	cardDefinitions.try_emplace(
		CARD_ENVYBAER, CARD_ENVYBAER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"envybaer", "Envybaer",
		5, 3, 10, SKILL_ENVYBAER
	);

	//Ephemeral Shroud
	cardDefinitions.try_emplace(
		CARD_EPHEMERAL_SHROUD, CARD_EPHEMERAL_SHROUD, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"ephemeralshroud", "Ephemeral Shroud",
		2, 1, 1, SKILL_EPHEMERAL_SHROUD
	);

	//Facestriker
	cardDefinitions.try_emplace(
		CARD_FACESTRIKER, CARD_FACESTRIKER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"facestriker", "Facestriker",
		6, 4, 6, SKILL_FACESTRIKER
	);

	//Fire Spitter
	cardDefinitions.try_emplace(
		CARD_FIRE_SPITTER, CARD_FIRE_SPITTER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"firespitter", "Fire Spitter",
		4, 3, 2, SKILL_RANGED
	);

	//Fireblazer
	cardDefinitions.try_emplace(
		CARD_FIREBLAZER, CARD_FIREBLAZER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"fireblazer", "Fireblazer",
		5, 5, 5, SKILL_PROVOKE
	);

	//Firestarter
	cardDefinitions.try_emplace(
		CARD_FIRESTARTER, CARD_FIRESTARTER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"firestarter", "Firestarter",
		5, 3, 5, SKILL_FIRESTARTER
	);

	//First Sword of Akrane
	cardDefinitions.try_emplace(
		CARD_FIRST_SWORD_OF_AKRANE, CARD_FIRST_SWORD_OF_AKRANE, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"firstswordofakrane", "First Sword of Akrane",
		6, 7, 7, SKILL_FIRST_SWORD_OF_AKRANE
	);

	//Flameblood Warlock
	cardDefinitions.try_emplace(
		CARD_FLAMEBLOOD_WARLOCK, CARD_FLAMEBLOOD_WARLOCK, RARITY_RARE, FACTION_NEUTRAL, TRIBE_NONE,
		"flamebloodwarlock", "Flameblood Warlock",
		2, 3, 1, SKILL_FLAMEBLOOD_WARLOCK
	);

	//Frostbone Naga
	cardDefinitions.try_emplace(
		CARD_FROSTBONE_NAGA, CARD_FROSTBONE_NAGA, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"frostbonenaga", "Frostbone Naga",
		4, 3, 3, SKILL_FROSTBONE_NAGA
	);

	//Ghost Lynx
	cardDefinitions.try_emplace(
		CARD_GHOST_LYNX, CARD_GHOST_LYNX, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"ghostlynx", "Ghost Lynx",
		2, 2, 1, SKILL_GHOST_LYNX
	);

	//Golden Justicar
	cardDefinitions.try_emplace(
		CARD_GOLDEN_JUSTICAR, CARD_GOLDEN_JUSTICAR, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_WARMASTER,
		"goldenjusticar", "Golden Justicar",
		5, 4, 6, SKILL_GOLDEN_JUSTICAR
	);

	//Golem Metallurgist
	cardDefinitions.try_emplace(
		CARD_GOLEM_METALLURGIST, CARD_GOLEM_METALLURGIST, RARITY_RARE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"golemmetallurgist", "Golem Metallurgist",
		2, 2, 3, SKILL_GOLEM_METALLURGIST
	);

	//Golem Vanquisher
	cardDefinitions.try_emplace(
		CARD_GOLEM_VANQUISHER, CARD_GOLEM_VANQUISHER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_GOLEM,
		"golemvanquisher", "Golem Vanquisher",
		3, 2, 4, SKILL_GOLEM_VANQUISHER
	);

	//Grove Lion
	cardDefinitions.try_emplace(
		CARD_GROVE_LION, CARD_GROVE_LION, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"grovelion", "Grove Lion",
		6, 5, 5, SKILL_GROVE_LION
	);

	//Hailstone Golem
	cardDefinitions.try_emplace(
		CARD_HAILSTONE_GOLEM, CARD_HAILSTONE_GOLEM, RARITY_NONE, FACTION_NEUTRAL, TRIBE_GOLEM,
		"hailstonegolem", "Hailstone Golem",
		4, 4, 6, EFFECT_NONE
	);

	//Healing Mystic
	cardDefinitions.try_emplace(
		CARD_HEALING_MYSTIC, CARD_HEALING_MYSTIC, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"healingmystic", "Healing Mystic",
		2, 2, 3, SKILL_HEALING_MYSTIC
	);

	//Hollow Grovekeper
	cardDefinitions.try_emplace(
		CARD_HOLLOW_GROVEKEEPER, CARD_HOLLOW_GROVEKEEPER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"hollowgrovekeeper", "Hollow Grovekeper",
		5, 3, 4, SKILL_HOLLOW_GROVEKEEPER
	);

	//Ironclad
	cardDefinitions.try_emplace(
		CARD_IRONCLAD, CARD_IRONCLAD, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"ironclad", "Ironclad",
		5, 4, 3, SKILL_IRONCLAD
	);

	//Jax Truesight
	cardDefinitions.try_emplace(
		CARD_JAX_TRUESIGHT, CARD_JAX_TRUESIGHT, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"jaxtruesight", "Jax Truesight",
		6, 1, 1, SKILL_JAX_TRUESIGHT
	);

	//Jaxi
	cardDefinitions.try_emplace(
		CARD_JAXI, CARD_JAXI, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"jaxi", "Jaxi",
		2, 1, 1, SKILL_JAXI
	);

	//Keeper of the Vale
	cardDefinitions.try_emplace(
		CARD_KEEPER_OF_THE_VALE, CARD_KEEPER_OF_THE_VALE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"keeperofthevale", "Keeper of the Vale",
		5, 3, 4, SKILL_KEEPER_OF_THE_VALE
	);

	//Khymera
	cardDefinitions.try_emplace(
		CARD_KHYMERA, CARD_KHYMERA, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"khymera", "Khymera",
		8, 5, 12, SKILL_KHYMERA
	);

	//Komodo Charger
	cardDefinitions.try_emplace(
		CARD_KOMODO_CHARGER, CARD_KOMODO_CHARGER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"komodocharger", "Komodo Charger",
		1, 1, 3, EFFECT_NONE
	);

	//Lady Locke
	cardDefinitions.try_emplace(
		CARD_LADY_LOCKE, CARD_LADY_LOCKE, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"ladylocke", "Lady Locke",
		3, 2, 3, SKILL_LADY_LOCKE
	);

	//Lightbender
	cardDefinitions.try_emplace(
		CARD_LIGHTBENDER, CARD_LIGHTBENDER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"lightbender", "Lightbender",
		4, 3, 3, SKILL_LIGHTBENDER
	);

	//Lux Ignis
	cardDefinitions.try_emplace(
		CARD_LUX_IGNIS, CARD_LUX_IGNIS, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"luxignis", "Lux Ignis",
		5, 2, 5, SKILL_LUX_IGNIS
	);

	//Manaforger
	cardDefinitions.try_emplace(
		CARD_MANAFORGER, CARD_MANAFORGER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"manaforger", "Manaforger",
		2, 1, 3, SKILL_MANAFORGER
	);

	//Maw
	cardDefinitions.try_emplace(
		CARD_MAW, CARD_MAW, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"maw", "Maw",
		2, 2, 2, SKILL_MAW
	);

	//Mindwarper
	cardDefinitions.try_emplace(
		CARD_MINDWARPER, CARD_MINDWARPER, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"mindwarper", "Mindwarper",
		4, 4, 3, SKILL_MINDWARPER
	);

	//Mirkblood Devourer
	cardDefinitions.try_emplace(
		CARD_MIRKBLOOD_DEVOURER, CARD_MIRKBLOOD_DEVOURER, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"mirkblooddevourer", "Mirkblood Devourer",
		3, 2, 4, SKILL_MIRKBLOOD_DEVOURER
	);

	//Mogwai
	cardDefinitions.try_emplace(
		CARD_MOGWAI, CARD_MOGWAI, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"mogwai", "Mogwai",
		3, 2, 3, SKILL_MOGWAI
	);

	//Necroseer
	cardDefinitions.try_emplace(
		CARD_NECROSEER, CARD_NECROSEER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"necroseer", "Necroseer",
		5, 5, 4, SKILL_NECROSEER
	);

	//Night Watcher
	cardDefinitions.try_emplace(
		CARD_NIGHT_WATCHER, CARD_NIGHT_WATCHER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"nightwatcher", "Night Watcher",
		4, 2, 4, SKILL_NIGHT_WATCHER
	);

	//Owlbeast Sage
	cardDefinitions.try_emplace(
		CARD_OWLBEAST_SAGE, CARD_OWLBEAST_SAGE, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"owlbeastsage", "Owlbeast Sage",
		4, 4, 4, SKILL_OWLBEAST_SAGE
	);

	//Paddo
	cardDefinitions.try_emplace(
		CARD_PADDO, CARD_PADDO, RARITY_LEGENDARY, FACTION_NEUTRAL, TRIBE_NONE,
		"paddo", "Paddo",
		7, 12, 6, SKILL_PADDO
	);

	//Piercing Mantis
	cardDefinitions.try_emplace(
		CARD_PIERCING_MANTIS, CARD_PIERCING_MANTIS, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"piercingmantis", "Piercing Mantis",
		2, 2, 2, SKILL_FRENZY
	);

	//Planar Scout
	cardDefinitions.try_emplace(
		CARD_PLANAR_SCOUT, CARD_PLANAR_SCOUT, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"planarscout", "Planar Scout",
		1, 2, 1, SKILL_AIRDROP
	);

	//Primus Fist
	cardDefinitions.try_emplace(
		CARD_PRIMUS_FIST, CARD_PRIMUS_FIST, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"primusfist", "Primus Fist",
		2, 2, 3, SKILL_PRIMUS_FIST
	);

	//Primus Shieldmaster
	cardDefinitions.try_emplace(
		CARD_PRIMUS_SHIELDMASTER, CARD_PRIMUS_SHIELDMASTER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"primusshieldmaster", "Primus Shieldmaster",
		4, 3, 6, SKILL_PROVOKE
	);

	//Prismatic Illusionist
	cardDefinitions.try_emplace(
		CARD_PRISMATIC_ILLUSIONIST, CARD_PRISMATIC_ILLUSIONIST, RARITY_RARE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"prismaticillusionist", "Prismatic Illusionist",
		3, 2, 3, SKILL_PRISMATIC_ILLUSIONIST
	);

	//Purgato
	cardDefinitions.try_emplace(
		CARD_PURGATOS_THE_REALMKEEPER, CARD_PURGATOS_THE_REALMKEEPER, RARITY_EPIC, FACTION_NEUTRAL, TRIBE_NONE,
		"purgatostherealmkeeper", "Purgatos, the Realmkeeper",
		4, 3, 5, SKILL_PURGATOS_THE_REALMKEEPER
	);

	//Putrid Dreadflayer
	cardDefinitions.try_emplace(
		CARD_PUTRID_DREADFLAYER, CARD_PUTRID_DREADFLAYER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"putriddreadflayer", "Putrid Dreadflayer",
		3, 2, 4, SKILL_FLYING
	);

	//Saberspine Tiger
	cardDefinitions.try_emplace(
		CARD_SABERSPINE_TIGER, CARD_SABERSPINE_TIGER, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"saberspinetiger", "Saberspine Tiger",
		4, 3, 2, SKILL_RUSH
	);

	//Sapphire Seer
	cardDefinitions.try_emplace(
		CARD_SAPPHIRE_SEER, CARD_SAPPHIRE_SEER, RARITY_COMMON, FACTION_NEUTRAL, TRIBE_NONE,
		"sapphireseer", "Sapphire Seer",
		3, 2, 2, SKILL_FORCEFIELD
	);

#pragma endregion

#pragma region Tokens

	//Illusion (Prismatic Illusionist)
	cardDefinitions.try_emplace(
		CARD_ILLUSION, CARD_ILLUSION, RARITY_NONE, FACTION_NEUTRAL, TRIBE_ARCANYST,
		"illusion", "Illusion",
		1, 2, 1, EFFECT_NONE
	);
	cardDefinitions.at(CARD_ILLUSION).isToken = true;

	//Mini-Jax (Jax Truesight / Jaxi)
	cardDefinitions.try_emplace(
		CARD_MINI_JAX, CARD_MINI_JAX, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"minijax", "Mini-Jax",
		1, 1, 1, SKILL_RANGED
	);
	cardDefinitions.at(CARD_MINI_JAX).isToken = true;

	//Spellspark (Firestarter)
	cardDefinitions.try_emplace(
		CARD_SPELLSPARK, CARD_SPELLSPARK, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"spellspark", "Spellspark",
		1, 1, 1, SKILL_RUSH
	);
	cardDefinitions.at(CARD_SPELLSPARK).isToken = true;

	//Tombstone (Dioltas)
	cardDefinitions.try_emplace(
		CARD_TOMBSTONE, CARD_TOMBSTONE, RARITY_NONE, FACTION_NEUTRAL, TRIBE_NONE,
		"tombstone", "Tombstone",
		3, 0, 10, SKILL_PROVOKE
	);
	cardDefinitions.at(CARD_TOMBSTONE).isToken = true;

#pragma endregion

#pragma region Spells

	//Breath of the Unborn
	cardDefinitions.try_emplace(
		CARD_BREATH_OF_THE_UNBORN, CARD_BREATH_OF_THE_UNBORN, RARITY_COMMON, FACTION_ABYSSIAN,
		"breathoftheunborn", "Breath of The Unborn",
		4, SPELL_BREATH_OF_THE_UNBORN
	);
	cardDefinitions.at(CARD_BREATH_OF_THE_UNBORN).targetMode = TargetMode(
		TARGET_MODE_ALL,
		[](BoardTile* tile) { return tile->HasUnit(); }
	);

	//Dark Seed
	cardDefinitions.try_emplace(
		CARD_DARK_SEED, CARD_DARK_SEED, RARITY_RARE, FACTION_ABYSSIAN,
		"darkseed", "Dark Seed",
		4, SPELL_DARK_SEED
	);
	cardDefinitions.at(CARD_DARK_SEED).targetMode = TargetMode(
		TARGET_MODE_ALL,
		[](BoardTile* tile) { return tile->HasUnit() && tile->unit->IsEnemy() && tile->unit->tribe == TRIBE_GENERAL; }
	);

#pragma endregion

}