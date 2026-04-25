#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardAttributeFragments_Shuffle.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_ShuffleAttribute : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:

};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Finisher"))
class UCardEffectFragment_Finisher : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:

};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Opener"))
class UCardEffectFragment_Opener : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:

};