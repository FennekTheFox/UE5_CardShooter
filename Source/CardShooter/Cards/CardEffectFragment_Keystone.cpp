#include "CardEffectFragment_Keystone.h"


#define LOCTEXT_NAMESPACE "CardEffects"


FText UDeckLimitationViolation_KeystoneLimit::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Would exceed keystone limit on {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME()

		return FText::Format(TextToFormat, FormatArgs);
}

FText UCardEffectFragment_Keystone::GetName_Implementation()
{
	return FText::Format(LOCTEXT("KeystoneName", "Keystone({0})"), FText::AsNumber(KeystoneLimit));
}

bool UCardEffectFragment_Keystone::Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	int32 KeystoneCount = 0;
	bool bSuccess = true;

	for (UCardData* Card : Deck)
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