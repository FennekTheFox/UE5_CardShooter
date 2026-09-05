#pragma once

#include "CardAttributeFragment_Shuffle.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_Stable.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Attribute_Stable);

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
