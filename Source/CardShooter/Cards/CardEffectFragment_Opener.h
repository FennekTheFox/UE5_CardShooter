#pragma once

#include "CardAttributeFragment_Shuffle.h"

#include "CardEffectFragment_Opener.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Shuffle: Opener"))
class UCardEffectFragment_Opener : public UCardEffectFragment_ShuffleAttribute
{
	GENERATED_BODY()

public:
	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo

};