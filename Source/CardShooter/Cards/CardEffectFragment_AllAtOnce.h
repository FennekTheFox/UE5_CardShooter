#pragma 

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_AllAtOnce.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "AllAtOnce"))
class UCardEffectFragment_Execution_AllAtOnce : public UCardEffectFragment_Execution_Compound
{
	GENERATED_BODY()

public:
	void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};