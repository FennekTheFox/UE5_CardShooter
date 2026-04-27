#pragma once

#include "CardAttributeFragment_DeckLimit.h"

#include "CardEffectFragment_Gregarious.generated.h"

UCLASS(BlueprintType)
class UDeckLimitationViolation_Gregarious : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};

//obscure cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Gregarious"))
class UCardEffectFragment_Gregarious : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MinimumDeckSize = 10;

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;

	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};