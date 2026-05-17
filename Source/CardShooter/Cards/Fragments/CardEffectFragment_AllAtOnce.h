#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_AllAtOnce.generated.h"

UCLASS()
class UCardExecutionInstance_AllAtOnce : public UCardExecutionInstance_Compound
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Compound: AllAtOnce"))
class UCardEffectFragment_Execution_AllAtOnce : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

	UCardEffectFragment_Execution_AllAtOnce();

public:
	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	 //~UCardEffectFragment
};