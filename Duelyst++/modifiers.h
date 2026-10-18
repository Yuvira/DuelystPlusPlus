#pragma once

//Include
#include <string>
#include <vector>

//Stat enums
enum eStat {
	STAT_ATK,
	STAT_HP,
	STAT_COST,
	STAT_MOVE
};

//Modifier enums
enum eModifier {
	MODTYPE_NONE,
	MODTYPE_REBASE,
	MODTYPE_BUFF,
	MODTYPE_OVERRIDE,
	MODTYPE_FIXED
};

//Stat modifiers
struct StatModifier {
	void SetValues(eModifier type, int value) {
		this->type = type;
		this->value = value;
	}
	eModifier type = MODTYPE_NONE;
	int value = 0;
};

//Modifier group
class ModifierGroup {
public:
	bool AddModifier(eStat stat, eModifier type, int value);
	bool UpdateModifier(eStat stat, int value);
	void HideValues() { hide = true; }
	void WriteValues(std::vector<std::string>& out) const;
	const StatModifier& ReadModifier(eStat stat) const;
private:
	void WriteValuesForType(eModifier type, std::vector<std::string>& out) const;
	void WriteValue(eStat stat, eModifier type, std::vector<std::string>& out) const;
	std::string GetContextStringForType(eModifier type) const;
	std::string GetNameStringForStat(int stat) const;
	StatModifier& GetModifier(eStat stat);
	StatModifier atk;
	StatModifier hp;
	StatModifier cost;
	StatModifier move;
	bool hide = false;
};