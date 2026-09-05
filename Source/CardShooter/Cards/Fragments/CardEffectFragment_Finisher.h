#pragma once

#include "CardAttributeFragment_Shuffle.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_Finisher.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Attribute_Finisher);

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Finisher"))
class UCardEffectFragment_Finisher : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

	UCardEffectFragment_Finisher();

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};