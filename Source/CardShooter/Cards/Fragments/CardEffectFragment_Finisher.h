#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Finisher.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Finisher"))
class UCardEffectFragment_Finisher : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	 //~UCardEffectFragment
};