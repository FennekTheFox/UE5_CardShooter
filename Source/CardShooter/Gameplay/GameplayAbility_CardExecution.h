#pragma once

#include "ProjectGameplayAbility.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

#include "GameplayAbility_CardExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayAbility_CardExecution : public UProjectGameplayAbility
{
	GENERATED_BODY()

	UGameplayAbility_CardExecution();

public:
	UFUNCTION(BlueprintPure)
	float GetSetByCallerValue(FGameplayTag Tag) const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityDesc;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FCardExecution_AdditionalData AdditionalData;

	FText GetName() 
	{
		return AbilityName;
	}


	FText GetDescription(const FCardIdentity& CardIdentity) 
	{
		return AbilityDesc;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void PostAdditionalDataSet();
};
