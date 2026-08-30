#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Opener.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Opener"))
class UCardEffectFragment_Opener : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

	UCardEffectFragment_Opener();

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment

};