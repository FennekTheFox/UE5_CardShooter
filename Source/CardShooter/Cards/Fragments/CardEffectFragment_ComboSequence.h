#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_ComboSequence.generated.h"


UCLASS()
class UCardExecutionInstance_ComboSequence : public UCardExecutionInstance_Compound
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Compound: Combo Sequence"))
class UCardEffectFragment_Execution_ComboSequence : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

	UCardEffectFragment_Execution_ComboSequence();

public:
	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};