//Include
#include "modifiers.h"
#include "utils.h"

//Add modifier for stat(s), return false if a modifier for a provided stat type already exists
bool ModifierGroup::AddModifier(eStat stat, eModifier type, int value) {
	if (GetModifier(stat).type != MODTYPE_NONE)
		return false;
	GetModifier(stat).SetValues(type, value);
	return true;
}

//Update value for modifier of a given stat type, return false if stat type match doesn't exist
bool ModifierGroup::UpdateModifier(eStat stat, int value) {
	if (GetModifier(stat).type == MODTYPE_NONE)
		return false;
	GetModifier(stat).value = value;
	return true;
}

#pragma region String Utils

//Write values to string list
void ModifierGroup::WriteValues(std::vector<std::string>& out) const {
	if (hide)
		return;
	WriteValuesForType(MODTYPE_REBASE, out);
	WriteValuesForType(MODTYPE_OVERRIDE, out);
	WriteValuesForType(MODTYPE_BUFF, out);
	WriteValuesForType(MODTYPE_FIXED, out);
}
void ModifierGroup::WriteValuesForType(eModifier type, std::vector<std::string>& out) const {
	if (atk.type == type && hp.type == type) {
		if (type == MODTYPE_BUFF)
			out.push_back("{" + Utils::ValueString(atk.value) + "/" + Utils::ValueString(hp.value) + "}");
		else
			out.push_back(GetContextStringForType(type) + "stats to {" + Utils::ValueString(atk.value) + "/" + Utils::ValueString(hp.value) + "}");
	}
	else {
		WriteValue(STAT_ATK, type, out);
		WriteValue(STAT_HP, type, out);
	}
	WriteValue(STAT_COST, type, out);
	WriteValue(STAT_MOVE, type, out);
}
void ModifierGroup::WriteValue(eStat stat, eModifier type, std::vector<std::string>& out) const {
	if (ReadModifier(stat).type != type)
		return;
	if (type == MODTYPE_BUFF)
		out.push_back("{" + Utils::ValueString(ReadModifier(stat).value) + " " + GetNameStringForStat(stat) + "}");
	else
		out.push_back(GetContextStringForType(type) + "{" + GetNameStringForStat(stat) + "} to {" + Utils::ValueString(ReadModifier(stat).value) + "}");
}

//Get context string from type enum
std::string ModifierGroup::GetContextStringForType(eModifier type) const {
	if (type == MODTYPE_REBASE)
		return "Rebased ";
	if (type == MODTYPE_OVERRIDE)
		return "Changed ";
	if (type == MODTYPE_FIXED)
		return "Fixed ";
	return "";
}

//Get name string from stat enum
std::string ModifierGroup::GetNameStringForStat(int stat) const {
	if (stat == STAT_ATK)
		return "Attack";
	if (stat == STAT_HP)
		return "Health";
	if (stat == STAT_COST)
		return "Cost";
	if (stat == STAT_MOVE)
		return "Movement";
	return "";
}

#pragma endregion

//Get const modifier reference by enum
const StatModifier& ModifierGroup::ReadModifier(eStat stat) const {
	switch (stat) {
	case STAT_ATK:
		return atk;
	case STAT_HP:
		return hp;
	case STAT_COST:
		return cost;
	case STAT_MOVE:
		return move;
	}
}

//Get modifier reference by enum
StatModifier& ModifierGroup::GetModifier(eStat stat) {
	switch (stat) {
	case STAT_ATK:
		return atk;
	case STAT_HP:
		return hp;
	case STAT_COST:
		return cost;
	case STAT_MOVE:
		return move;
	}
}