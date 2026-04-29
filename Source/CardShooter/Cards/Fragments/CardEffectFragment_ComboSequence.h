#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_ComboSequence.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Combo Sequence"))
class UCardEffectFragment_Execution_ComboSequence : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

public:
	void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};