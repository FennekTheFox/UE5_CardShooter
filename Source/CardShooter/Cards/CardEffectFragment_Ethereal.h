#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Ethereal.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Ethereal"))
class UCardEffectFragment_Ethereal : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};