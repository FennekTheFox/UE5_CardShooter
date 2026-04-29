#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_AddAttribute.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Add Attribute To Next"))
class UCardEffectFragment_Execution_AddAttributeToNextCards : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TObjectPtr<UCardEffectFragment_ExecutionAttribute> Attribute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Count = 1;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};