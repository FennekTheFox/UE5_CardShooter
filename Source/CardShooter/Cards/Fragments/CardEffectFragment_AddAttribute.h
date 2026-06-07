#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_AddAttribute.generated.h"

UCLASS()
class UCardExecutionInstance_AddAttributeToNextCards : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Add Attribute To Next"))
class UCardEffectFragment_Execution_AddAttributeToNextCards : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

	UCardEffectFragment_Execution_AddAttributeToNextCards();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TObjectPtr<UCardEffectFragment_ExecutionAttribute> Attribute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Count = 1;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};