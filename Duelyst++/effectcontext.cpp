//Include
#include "card.h"
#include "effectcontext.h"
#include "game.h"

//Conversions
bool EffectContext::IsAuraContext() { return effect->IsAura(); }
bool EffectContext::IsSpellContext() { return effect->IsSpellEffect() && card->IsSpell(); }
AuraContext EffectContext::GetAuraContext() { return AuraContext(effect->GetAura(), card, game); }
SpellContext EffectContext::GetSpellContext() { return SpellContext(effect->GetSpellEffect(), card->GetSpell(), game); }

//EffectContext Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::IsInOwnerHand() { return card->owner->HasCardInHand(card); }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return game->GetTurnPlayer() == card->owner; }
Player* EffectContext::GetOwner() { return card->owner; }
Player* EffectContext::GetOpponent() { return card->owner->opponent; }
Unit* EffectContext::GetUnit() { return card->GetUnit(); }
Spell* EffectContext::GetSpell() { return card->GetSpell(); }
Unit* EffectContext::GetAllyGeneral() { return card->owner->general; }
Unit* EffectContext::GetEnemyGeneral() { return card->owner->opponent->general; }
BoardTile* EffectContext::GetTile() { return card->IsOnBoard() ? card->GetCardOnBoard()->curTile : nullptr; }
Map* EffectContext::GetMap() { return &game->map; }

//AuraContext Utilities
bool AuraContext::CardIsAuraSource() { return card == aura->GetSourceCard(); }
bool AuraContext::CardSharesAuraOwner() { return card->owner == aura->GetOwner(); }
bool AuraContext::CardIsFriendOfAuraSource() { return card->owner == aura->GetSourceCard()->owner && card != aura->GetSourceCard(); }
bool AuraContext::IsAuraSource(Card* compare) { return compare == aura->GetSourceCard(); }
bool AuraContext::SharesAuraOwner(Card* compare) { return compare->owner == aura->GetOwner(); }