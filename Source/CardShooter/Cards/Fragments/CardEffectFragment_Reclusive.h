#pragma once

#include "CardAttributeFragment_DeckLimit.h"

#include "CardEffectFragment_Reclusive.generated.h"

UCLASS(BlueprintType)
class UDeckLimitationViolation_Reclusive : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};


//prominent cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Reclusive"))
class UCardEffectFragment_Reclusive : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

	UCardEffectFragment_Reclusive();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaximumDeckSize = 10;

	bool Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};
