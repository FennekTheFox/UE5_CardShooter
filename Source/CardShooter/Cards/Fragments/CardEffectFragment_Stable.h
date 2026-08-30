#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Stable.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Stable"))
class UCardEffectFragment_Stable : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

	UCardEffectFragment_Stable();

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};
