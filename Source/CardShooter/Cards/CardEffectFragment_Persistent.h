#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Persistent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Persistent"))
class UCardEffectFragment_Persistent : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};