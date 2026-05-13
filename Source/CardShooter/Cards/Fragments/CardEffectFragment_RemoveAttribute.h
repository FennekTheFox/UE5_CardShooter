#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_RemoveAttribute.generated.h"


UCLASS()
class UCardExecutionInstance_RemoveAttributeFromNextCards : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};


UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Remove Attribute From Next"))
class UCardEffectFragment_Execution_RemoveAttributeFromNextCards : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCardEffectFragment_ExecutionAttribute> AttributeClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Count = 1;

	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};
