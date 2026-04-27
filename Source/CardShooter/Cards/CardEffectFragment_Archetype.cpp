#include "CardEffectFragment_Archetype.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UDeckLimitationViolation_Archetype::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Conflicting Archetype on {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

bool UCardEffectFragment_Archetype::Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	for (UCardData* Card : Deck)
	{
		TArray<UCardEffectFragment_Attribute*> Attributes = Card->GetAllAttributesOfClass(UCardEffectFragment_Archetype::StaticClass());

		for (UCardEffectFragment_Attribute* Attribute : Attributes)
		{
			if (auto* AsArchetypeAttribute = Cast<UCardEffectFragment_Archetype>(Attribute))
			{
				if (AsArchetypeAttribute->Archetype != Archetype)
				{
					FailureReasons.Add(NewObject<UDeckLimitationViolation_Archetype>(this));
					return false;
				}
			}
		}
	}

	return true;
}

FText UCardEffectFragment_Archetype::GetName_Implementation()
{
	if (!ensure(Archetype))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("ArchetypeAttrName", "Archetype({0})"), Archetype->ArchetypeName);
}

#undef LOCTEXT_NAMESPACE