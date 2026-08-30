#include "CardEffectFragment_Reclusive.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UDeckLimitationViolation_Reclusive::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Too many cards in deck for {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

FText UCardEffectFragment_Reclusive::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("ReclusiveAttrName", "Reclusive({0})"), FText::AsNumber(MaximumDeckSize));
}

UCardEffectFragment_Reclusive::UCardEffectFragment_Reclusive()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Reclusivee", "Reclusive");
}

bool UCardEffectFragment_Reclusive::Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	bool bSuccess = Deck.CardList.Num() <= MaximumDeckSize;

	if (!bSuccess)
	{
		FailureReasons.Add(NewObject<UDeckLimitationViolation_Reclusive>(this));
	}
	return bSuccess;
}

FText UCardEffectFragment_Reclusive::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return FText::Format(LOCTEXT("ReclusiveAttrDesc", "This card can only be part of decks with no more than {0} cards."), FText::AsNumber(MaximumDeckSize));
}

#undef LOCTEXT_NAMESPACE