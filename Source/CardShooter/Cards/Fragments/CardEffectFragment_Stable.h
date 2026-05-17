#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Stable.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Stable"))
class UCardEffectFragment_Stable : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	 //~UCardEffectFragment
};