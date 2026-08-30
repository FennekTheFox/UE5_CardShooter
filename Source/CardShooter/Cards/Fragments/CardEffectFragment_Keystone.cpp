#include "CardEffectFragment_Keystone.h"


#define LOCTEXT_NAMESPACE "CardEffects"


FText UDeckLimitationViolation_KeystoneLimit::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Would exceed keystone limit on {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME()

		return FText::Format(TextToFormat, FormatArgs);
}

FText UCardEffectFragment_Keystone::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("KeystoneAttrName", "Keystone({0})"), FText::AsNumber(KeystoneLimit));
}

FText UCardEffectFragment_Keystone::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return FText::Format(LOCTEXT("KeystoneAttrDesc", "A deck with this card can only contain {0} cards with the Keystone attribute."), FText::AsNumber(KeystoneLimit));
}

UCardEffectFragment_Keystone::UCardEffectFragment_Keystone()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Keystone", "Keystone");
}

bool UCardEffectFragment_Keystone::Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	int32 KeystoneCount = 0;
	bool bSuccess = true;

	for (UCardData* Card : Deck.UtilityCards)
	{
		if (!Card->GetAllAttributesOfClass(UCardEffectFragment_Keystone::StaticClass()).IsEmpty())
		{
			KeystoneCount += 1;
		}
	}

	for (UCardData* Card : Deck.CardList)
	{
		if (!Card->GetAllAttributesOfClass(UCardEffectFragment_Keystone::StaticClass()).IsEmpty())
		{
			KeystoneCount += 1;
		}
	}

	if (KeystoneCount <= KeystoneLimit)
	{
		return true;
	}
	else
	{
		FailureReasons.Add(NewObject<UDeckLimitationViolation_KeystoneLimit>(this));
		return false;
	}
}

#undef LOCTEXT_NAMESPACE