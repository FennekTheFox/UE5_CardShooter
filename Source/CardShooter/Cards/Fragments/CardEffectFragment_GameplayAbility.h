#pragma once

#include "CardAttributeFragment_Execution.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_GameplayAbility.generated.h"

UCLASS()
class UCardExecutionInstance_GameplayAbility : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_GameplayAbility 
	: public UCardEffectFragment_ExecutionWithParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TargetDataID;

	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData);

	//~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};