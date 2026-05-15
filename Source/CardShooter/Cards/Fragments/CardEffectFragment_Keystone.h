#pragma once

#include "CardAttributeFragment_DeckLimit.h"
#include "CardEffectFragment_Keystone.generated.h"


UCLASS(BlueprintType)
class UDeckLimitationViolation_KeystoneLimit : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};

//Keystone attributes limit the number of Keystone Attribute cards per deck, lowest number dominates
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Keystone"))
class UCardEffectFragment_Keystone : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 KeystoneLimit = 3;

	bool Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};