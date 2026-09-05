#include "CardEffectFragment_Archetype.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UDeckLimitationViolation_Archetype::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Conflicting Archetype on {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

UCardEffectFragment_Archetype::UCardEffectFragment_Archetype()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Archetype", "Archetype");
}

bool UCardEffectFragment_Archetype::Verify(const FCardIdentity& CardIdentity, const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	for (UCardData* Card : Deck.UtilityCards)
	{
		TArray<UCardEffectFragment_Attribute*> Attributes = Card->GetAllAttributesOfClass(CardIdentity,UCardEffectFragment_Archetype::StaticClass());

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

	for (UCardData* Card : Deck.CardList)
	{
		TArray<UCardEffectFragment_Attribute*> Attributes = Card->GetAllAttributesOfClass(CardIdentity,UCardEffectFragment_Archetype::StaticClass());

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

FText UCardEffectFragment_Archetype::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return LOCTEXT("ArchetypeAttrDesc", "Your deck can only contain cards of one archetype.");
}

FText UCardEffectFragment_Archetype::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	if (!ensure(Archetype))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("ArchetypeAttrName", "Archetype({0})"), Archetype->ArchetypeName);
}

#undef LOCTEXT_NAMESPACE