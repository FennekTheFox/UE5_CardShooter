#pragma once

#include "CardAttributeFragment_Shuffle.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_Opener.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Attribute_Opener)

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