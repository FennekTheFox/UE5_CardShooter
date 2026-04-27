#pragma once

#include "ProjectGameplayAbility.h"
#include "ICardExecutionInfo.h"

#include "CardExecutionAbility.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayAbility_CardExecution : public UProjectGameplayAbility
	, public ICardExecutionInfo
{
	GENERATED_BODY()

public:
};