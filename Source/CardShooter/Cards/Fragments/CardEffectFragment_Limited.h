#pragma once

#include "CardAttributeFragment_DeckLimit.h"

#include "CardEffectFragment_Limited.generated.h"


UCLASS(BlueprintType)
class UDeckLimitationViolation_Limit : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};


//prominent cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Limited"))
class UCardEffectFragment_Limited : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

	UCardEffectFragment_Limited();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Limit = 3;

	bool Verify(const FCardIdentity& CardIdentity, const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};