#include "CardAttributeFragments_DeckLimit.h"

#include "CardData.h"


#define FAILURE_REASON_FORMAT_CARD_NAME()\
FFormatOrderedArguments FormatArgs;\
if (auto* OwningAttribute = GetOuter())\
{\
	if (UCardData* OwningCard = Cast<UCardData>(OwningAttribute->GetOuter()))\
	{\
		FormatArgs.Add(FFormatArgumentValue(OwningCard->CardName));\
	}\
	else\
	{\
		ensure(false);\
	}\
}\
else\
{\
	ensure(false);\
}

FText UDeckLimitationViolation_KeystoneLimit::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Would exceed keystone limit on {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME()

	return FText::Format(TextToFormat, FormatArgs);
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

FText UDeckLimitationViolation_Reclusive::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Too many cards in deck for {CardName}.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
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


FText UDeckLimitationViolation_Limit::GetFailureReasonText()
{
	FText TextToFormat = FText::FromString("Too many of {CardName} in deck.");

	FAILURE_REASON_FORMAT_CARD_NAME();

	return FText::Format(TextToFormat, FormatArgs);
}

bool UCardEffectFragment_Limited::Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	int32 Count = 0;

	if (auto* OwningAttribute = GetOuter())
	{
		if (UCardData* OwningCard = Cast<UCardData>(OwningAttribute->GetOuter()))
		{
			for (UCardData* Card : Deck)
			{
				if (Card->GetClass() == OwningCard->GetClass())
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
	}

	return true;
}
