#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Finisher.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Finisher"))
class UCardEffectFragment_Finisher : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:
	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};