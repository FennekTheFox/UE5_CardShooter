#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_DebugPrint.generated.h"

UCLASS()
class UCardExecutionInstance_DebugPrint : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Debug Print"))
class UCardEffectFragment_Execution_DebugPrint : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString Message;

	UPROPERTY(EditDefaultsOnly)
	FColor DisplayColor = FColor::Green;

	UPROPERTY(EditDefaultsOnly)
	float DisplayDuration = 2.f;

	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};