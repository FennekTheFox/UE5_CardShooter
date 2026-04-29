#pragma once

#include "ProjectGameplayAbility.h"
#include "ICardExecutionInfo.h"

#include "CardExecutionAbility.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayAbility_CardExecution : public UProjectGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityDesc;

	FText GetName() 
	{
		return AbilityName;
	}


	FText GetDescription() 
	{
		return AbilityDesc;
	}
};