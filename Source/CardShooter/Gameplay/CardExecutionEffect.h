#pragma once

#include "ICardExecutionInfo.h"

#include "CardExecutionEffect.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayEffect_CardExecution : public UGameplayEffect
	, public ICardExecutionInfo
{
	GENERATED_BODY()

public:
};