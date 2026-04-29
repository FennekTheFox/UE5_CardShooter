#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_GameplayAbility.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_GameplayAbility 
	: public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;


	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};