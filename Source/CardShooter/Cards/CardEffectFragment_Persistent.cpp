#include "CardEffectFragment_Persistent.h"

#include "CardData.h"
#include "CardEffectFragment_EffectToSelf.h"
#include "Gameplay/CardExecutionEffect.h"


#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Persistent::GetLocalizableName_Implementation()
{
	return LOCTEXT("PersistentAttrName", "Persistent");
}

FText UCardEffectFragment_Persistent::GetLocalizableDescription_Implementation()
{
	if (!TrackedEffect)
	{
		return FText::FromString("Missing Tracked Effect class ref.");
	}

	return FText::Format(LOCTEXT("PersistentAttrDesc", "While the {0} effect granted by this card is active, the card will not be shuffled back into the deck."), 
		TrackedEffect.GetDefaultObject()->GetSimpleLocalizableName());

	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE