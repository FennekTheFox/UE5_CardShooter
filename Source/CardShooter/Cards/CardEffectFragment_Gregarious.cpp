#include "CardEffectFragment_Gregarious.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UDeckLimitationViolation_Gregarious::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Not enough cards in deck for {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

bool UCardEffectFragment_Gregarious::Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	bool bSuccess = Deck.Num() >= MinimumDeckSize;

	if (!bSuccess)
	{
		FailureReasons.Add(NewObject<UDeckLimitationViolation_Gregarious>(this));
	}
	return bSuccess;
}

FText UCardEffectFragment_Gregarious::GetLocalizableDescription_Implementation()
{
	return FText::Format(LOCTEXT("GregariousAttrDesc", "This card can only be part of decks with at least {0} cards."), FText::AsNumber(MinimumDeckSize));
}

FText UCardEffectFragment_Gregarious::GetLocalizableName_Implementation()
{
	return FText::Format(LOCTEXT("GregariousAttrName", "Gregarious({0})"), FText::AsNumber(MinimumDeckSize));
}

#undef LOCTEXT_NAMESPACE