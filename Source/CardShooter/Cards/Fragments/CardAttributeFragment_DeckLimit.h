#pragma once

#include "CardAttributeFragment_Base.h"

#include "Cards/CardData.h"
#include "Cards/Deck.h"

#include "CardAttributeFragment_DeckLimit.generated.h"


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

class UCardData;

UCLASS(BlueprintType)
class UDeckLimitationViolation : public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	virtual FText GetFailureReasonText() { return FText::GetEmpty(); };
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_DeckLimitation : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:
	virtual bool Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons) { return false; };
};