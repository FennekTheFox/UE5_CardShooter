#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_DebugPrint.generated.h"

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

	void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};