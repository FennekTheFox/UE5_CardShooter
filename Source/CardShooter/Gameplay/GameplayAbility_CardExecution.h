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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText AbilityDesc;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FString, TSubclassOf<class UGameplayEffect_CardExecution>> ReferencedEffects;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FCardExecution_AdditionalData AdditionalData;

	void GetFormatingArguments(FFormatNamedArguments& Args);

	FText GetName() 
	{
		return AbilityName;
	}


	FText GetDescription() 
	{
		FFormatNamedArguments Args;
		GetFormatingArguments(Args);

		return FText::Format(AbilityDesc, Args);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void PostAdditionalDataSet();
};
