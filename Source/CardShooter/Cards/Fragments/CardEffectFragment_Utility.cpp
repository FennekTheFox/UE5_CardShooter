#include "CardEffectFragment_Utility.h"
#include "Cards/CardData.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Utility::GetLocalizableName_Implementation()
{
	UCardData* OwningCard = Cast<UCardData>(GetOuter());

	if (OwningCard || PrimaryUtilityCooldown == SecondaryUtilityCooldown)
	{
		if (!OwningCard->SecondaryExecution)
		{
			return FText::Format(LOCTEXT("UtilityAttrName", "Utility({0}s)"), PrimaryUtilityCooldown);
		}

		return FText::Format(LOCTEXT("UtilityAttrName", "Utility({0}s|{1}s)"), PrimaryUtilityCooldown, SecondaryUtilityCooldown);
	}


	return FText::GetEmpty();
}

FText UCardEffectFragment_Utility::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	UCardData* OwningCard = Cast<UCardData>(GetOuter());

	if (OwningCard)
	{
		if (!OwningCard->SecondaryExecution || PrimaryUtilityCooldown == SecondaryUtilityCooldown)
		{
			return FText::Format(LOCTEXT("UtilityAttrDesc", "This card can be used in a decks utility slot. If used in a utility slot, this card has a cooldown of {0}s."), PrimaryUtilityCooldown);
		}

		return FText::Format(LOCTEXT("UtilityAttrDesc", "This card can be used in a decks utility slot. If used in a utility slot, this card has a cooldown of {0}s for its primary execution, or {1}s for its secondary execution."), PrimaryUtilityCooldown, SecondaryUtilityCooldown);
	}


	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE