#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Opener.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Opener"))
class UCardEffectFragment_Opener : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	 //~UCardEffectFragment

};