#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_Sequence.generated.h"

UCLASS()
class UCardExecutionInstance_Sequence : public UCardExecutionInstance_Compound
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;

	UPROPERTY()
	int32 Index = 0;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Compound: Sequence"))
class UCardEffectFragment_Execution_Sequence : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

public:
	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};