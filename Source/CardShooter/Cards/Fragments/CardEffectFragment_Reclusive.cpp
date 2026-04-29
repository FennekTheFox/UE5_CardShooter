#include "CardEffectFragment_Reclusive.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UDeckLimitationViolation_Reclusive::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Too many cards in deck for {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

FText UCardEffectFragment_Reclusive::GetLocalizableName_Implementation()
{
	return FText::Format(LOCTEXT("ReclusiveAttrName", "Reclusive({0})"), FText::AsNumber(MaximumDeckSize));
}

bool UCardEffectFragment_Reclusive::Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	bool bSuccess = Deck.Num() <= MaximumDeckSize;

	if (!bSuccess)
	{
		FailureReasons.Add(NewObject<UDeckLimitationViolation_Reclusive>(this));
	}
	return bSuccess;
}

FText UCardEffectFragment_Reclusive::GetLocalizableDescription_Implementation()
{
	return FText::Format(LOCTEXT("ReclusiveAttrDesc", "This card can only be part of decks with no more than {0} cards."), FText::AsNumber(MaximumDeckSize));
}

#undef LOCTEXT_NAMESPACE