#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_Cycle.generated.h"

UCLASS()
class UCardExecutionInstance_Cycle : public UCardExecutionInstance_Compound
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;

	UPROPERTY()
	int32 Index = 0;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Compound: Cycle"))
class UCardEffectFragment_Execution_Cycle : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

	UCardEffectFragment_Execution_Cycle();

public:
	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};