#include "CardEffectFragment_Limited.h"

#include "Cards/CardData.h"


#define LOCTEXT_NAMESPACE "CardEffects"


FText UDeckLimitationViolation_Limit::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Too many of {CardName} in deck.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

bool UCardEffectFragment_Limited::Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	int32 Count = 0;

	if (UCardData* OwningCard = Cast<UCardData>(GetOuter()))
	{
		for (UCardData* Card : Deck.UtilityCards)
		{
			if (Card == OwningCard)
			{
				Count += 1;

				if (Count > Limit)
				{
					FailureReasons.Add(NewObject<UDeckLimitationViolation_Limit>(this));
					return false;
				}
			}
		}
	}

	if (UCardData* OwningCard = Cast<UCardData>(GetOuter()))
	{
		for (UCardData* Card : Deck.CardList)
		{
			if (Card == OwningCard)
			{
				Count += 1;

				if (Count > Limit)
				{
					FailureReasons.Add(NewObject<UDeckLimitationViolation_Limit>(this));
					return false;
				}
			}
		}
	}

	return true;
}

FText UCardEffectFragment_Limited::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	if (UCardData* OwningCard = Cast<UCardData>(GetOuter()))
	{
		return FText::Format(LOCTEXT("LimitedAttrDesc", "A deck may contain only {0} or less copies of {1}."), FText::AsNumber(Limit), OwningCard->CardName);
	}
	return FText::GetEmpty();
}

FText UCardEffectFragment_Limited::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("LimitedAttrName", "Limited({0})"), FText::AsNumber(Limit));
}

#undef LOCTEXT_NAMESPACE